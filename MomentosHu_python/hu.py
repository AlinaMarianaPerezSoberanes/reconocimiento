import cv2
import numpy as np
from PIL import Image
import os
import math

def mimagen(img):
    n=0
    for y in range(height):
        for x in range(width):
            aux=img[y][x]
            if aux==255:
                n=0
            if aux==0:
                n=255
            img[y][x]=n
    return img

#Primer momeno m00 obtiene el area del objeto de la forma SUM(x^i y^j)(I(x,y))<<-Img binarizada
def momento(img,i,j):
    #Arreglo al sacar el momento de la imagen
    arr=[]
    sum=0
    cont=0
    for y in range(height):
        for x in range(width):
            aux2=img[y][x]
            if aux2==255:
                aux=(pow(x,i)*pow(y,j))
                arr.append(aux)
                sum+=aux
        
    #Sumatoria de (x^i y^j)(I(x,y))
    #for ii in range(len(arr)):
    #    sum=sum+arr[ii]
    return sum

def momento_central(img,i,j):
    Cx=(momento(img,0,1))/(momento(img,0,0))
    Cy=(momento(img,1,0))/(momento(img,0,0))
    #print("cx=",Cx,"Cy=",Cy)
    res=[]
    #Multiplicacion de momentos centrales
    for y in range (height):
        for x in range(width):
            aux=img[y][x]
            if aux==255:
                auxp=(pow((x-Cx),i))*(pow((y-Cy),j))
                res.append(auxp)
    #Suma de la multiplicacion de momentos centrales por imagen bin
    sum=0 
    for ii in range(len(res)):
        sum=sum+res[ii]
    return sum

def normalizar(img,i,j):
    m1=momento_central(img,i,j)
    m2=momento_central(img,0,0)
    #print(m1,m2)
    #Momento central normalizados op=p+q/2
    operacion=1+((i+j)/2)
    #print("Operacion=",operacion)
    mnormal=m1/(pow(m2,operacion))
    #print("normal=",mnormal)
    return mnormal



def momentos4_hu(img):
    I2=pow((normalizar(img,2,0)+normalizar(img,0,2)),2) + pow(4*normalizar(img,1,1),2)
    print("I2=",I2)
    I3=pow((normalizar(img,3,0)+ 3*normalizar(img,1,2)),2) + pow((3*normalizar(img,2,1))- normalizar(img,0,3),2)
    print("I3=",I3)
    I4=pow(normalizar(img,3,0)*normalizar(img,1,2),2) + pow(normalizar(img,2,1)- normalizar(img,0,3),2)
    print("I4=",I4)

    I5=(normalizar(img,3,0)- 3*normalizar(img,1,2))*(normalizar(img,3,0)+normalizar(img,1,2))*(pow(normalizar(img,3,0)+normalizar(img,1,2),2) - 3*pow(normalizar(img,2,1)+normalizar(img,0,3),2)) +(3*normalizar(img,2,1) - normalizar(img,0,3))*(normalizar(img,2,1)+normalizar(img,0,3))*(3*pow(normalizar(img,3,0)+normalizar(img,1,2),2)) - pow(normalizar(img,2,1)+normalizar(img,0,3),2)
    print("I5=",I5)

    I6=(normalizar(img,2,0)-normalizar(img,0,2))*(pow(normalizar(img,3,0)-normalizar(img,1,2),2)) - pow(normalizar(img,2,1)-normalizar(img,0,3),2)+4*(normalizar(img,1,1))*normalizar(img,3,0)+normalizar(img,1,2)*normalizar(img,2,1)+normalizar(img,0,3)
    print("I6=",I6)

    I7=(3*normalizar(img,2,1)- normalizar(img,3,0))*(normalizar(img,3,0)+ normalizar(img,1,2))*(pow(normalizar(img,3,0)+ normalizar(img,1,2),2)) - 3*pow(normalizar(img,2,1)+ normalizar(img,0,3),2)+(3*normalizar(img,1,2)- normalizar(img,3,0))*(normalizar(img,2,1)+ normalizar(img,0,3))*((3*pow(normalizar(img,3,0)- normalizar(img,1,2),2)) - pow(normalizar(img,2,1)+ normalizar(img,0,3),2) )
    print("I7=",I7)

def momento1(img):
    Cx=(momento(img,1,0))/(momento(img,0,0))
    Cy=(momento(img,0,1))/(momento(img,0,0))
    n20=momento(img,2,0)-(Cx*momento(img,1,0))
    n02=momento(img,0,2)-(Cy*momento(img,0,1))
    print("n20, n02",n20,n02)
    n2=n20/(120627*120627)
    n0=n02/(120627*120627)
    print("m1=",n2+n0)

def momento2(img):
    Cx=(momento(img,1,0))/(momento(img,0,0))
    Cy=(momento(img,0,1))/(momento(img,0,0))
    n20=momento(img,2,0)-(Cx*momento(img,1,0))
    n02=momento(img,0,2)-(Cy*momento(img,0,1))
    n11=momento(img,1,1)-(Cx*momento(img,0,1))
    print("n20: ",n20)
    print("n02: ",n02)
    print("n11: ",n11)

def momento3(img):
    n30=momento(img,3,0)-(3*Cx*momento(img,2,0))+(2*momento(img,1,0)*Cx*Cx)
    n03=momento(img,0,3)-(3*Cy*momento(img,0,2))+(2*momento(img,0,1)*Cy*Cy)
    n12=momento(img,1,2)-(2*Cy*momento(img,1,1))-(Cx*momento(img,0,2))+(2*momento(img,1,0)*Cy*Cy)
    n21=momento(img,2,1)-(2*Cx*momento(img,1,1))-(Cy*momento(img,2,0))+(2*momento(img,0,1)*Cx*Cx)
    print("n30: ",n30)
    print("n03: ",n03)
    print("n12: ",n12)
    print("n21: ",n21)


#Abrir hoja
img2 = cv2.imread('hoja.png',0)
# Imagen binarizada
ret1,bin = cv2.threshold(img2,180,255,cv2.THRESH_BINARY)
#Dimensiones de la imagen
dim=img2.shape
height = img2.shape[0]
width = img2.shape[1]
print("Tamano de imagen: "+str(width)+"x"+str(height))
#Invertir imagen Sossa maneja 1 objeto 0 fondo, varia un poco en el calculo
#img2=mimagen(bin)
#cv2.imwrite('bin2.jpg',img2)

m=momento(img2,0,0)
print("Area del objeto",m)
Cx=(momento(img2,1,0))/(momento(img2,0,0))
Cy=(momento(img2,0,1))/(momento(img2,0,0))
print("x,y-",Cx,Cy)

#momentos de orden 2
rx=math.sqrt((momento(img2,2,0)/momento(img2,0,0)))
ry=math.sqrt((momento(img2,0,2)/momento(img2,0,0)))
print("rx,ry-",rx,ry)

#momentos de orden 3
Sx=momento(img2,3,0)/math.sqrt(pow(momento(img2,2,0),3))
Sy=momento(img2,0,3)/math.sqrt(pow(momento(img2,0,2),3))
print("sx,sy-",Sx,Sy)

momento1(img2)
momento2(img2)
momento3(img2)

