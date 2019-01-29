
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "imagen.h"
#include <math.h>
#define DIMASK 3
#define NUM_HILOS 4

//Un pixel ocupa un byte para reducir el tamaño de la imagen
unsigned char* RGBToGray(unsigned char *imagen,uint32_t width, uint32_t height);
void GraytoRGB(unsigned char* imagenRGB, unsigned char* imagenGray,uint32_t width, uint32_t height);
void brilloImagen(unsigned char* imagenGray,uint32_t width, uint32_t height);
unsigned char* reservarMemoria(uint32_t width, uint32_t height);
void * funHilo(void *arg);
int thotsu(unsigned char * imagen, uint32_t width, uint32_t height );
unsigned char * binarizar(unsigned char * imagen, int umbral, uint32_t width, uint32_t height);
void sustraccion(unsigned char* imagenBase, unsigned char* imagenRep,uint32_t width, uint32_t height);
int areaObjeto(unsigned char *imagen,uint32_t width, uint32_t height);
int momento(unsigned char *imagen,int i,int j);
void momentoHu(unsigned char * img);
double normalizar(unsigned char *imagen,int momento,int p,int q);
void* subImagen(void *arg);
void* sauvola(void *arg);
void * dilata(void *arg);
/*FUNCIONES CON HILOS*/
void * momento1h(void *arg);
void * momento2h(void *arg);
void * momento3h(void *arg);
void * momento4h(void *arg);
void * momento5h(void *arg);
void * momento6h(void *arg);
void * momento7h(void *arg);
/*
FUNCIONES SIN HILOS
double momento1(unsigned char * imagen);
double momento2(unsigned char * imagen);
double momento3(unsigned char * imagen);
double momento4(unsigned char * imagen);
double momento5(unsigned char * imagen);
double momento6(unsigned char * imagen);
double momento7(unsigned char * imagen);
*/

/*VARIABLES GLOBALES*/
bmpInfoHeader info;
unsigned char *imagenRGB;                   //para leer la imagen a colores en valores RGB dentro de la imagen
unsigned char *imagenGray;                  // en esta imagen se guarda con un tamaño pequeño, cada pixel ocupa 1 byte en memoria
unsigned char *imagenAux;                   //imagen en escala de grises
unsigned char *imgBin;                      //imagen segmentada por otsu
unsigned char *imagenSustrac;               //imagen de sustracion de fondo
unsigned char *nueva;
unsigned char *nuevaGray;
unsigned char *nuevaAux;
unsigned char *nuevaBin;
unsigned char *otsu;
unsigned char *imgSau;

int main(){
    int *hilo,i,k,nhs[NUM_HILOS];                                            //Arreglo de hilos
    pthread_t tids[NUM_HILOS];                                               //Creación de hilos

    imagenRGB=abrirBMP("base10.bmp",&info);
    nueva=abrirBMP("nueva10.bmp",&info);
    
    displayInfo(&info);
    imagenGray=RGBToGray(imagenRGB,info.width,info.height);
    brilloImagen(imagenGray,info.width,info.height);

    nuevaGray=RGBToGray(nueva,info.width,info.height);
    brilloImagen(nuevaGray,info.width,info.height);

    imagenAux=reservarMemoria(info.width,info.height);
    nuevaAux=reservarMemoria(info.width,info.height);
    imagenSustrac=reservarMemoria(info.width,info.height);                  //Reservar memoria
    imgBin = reservarMemoria(info.width,info.height);
    nuevaBin=reservarMemoria(info.width,info.height);
    otsu=reservarMemoria(info.width,info.height);
    imgSau=reservarMemoria(info.width,info.height);
    //filtroPB(imagenGray,imagenAux,info.width,info.height);

    for(i=0;i<4;i++){
        nhs[i]=i;                                                            //creacion de hilos
        pthread_create(&tids[i],NULL,funHilo,(void *)&nhs[i]);               //crea varios hilos
    }
    for(k=0;k<4;k++){
         pthread_join(tids[k],(void**)&hilo);                                //espera a que termine la ejecucion del hilo
         printf("\nHilo %d terminado\n",*hilo);
    }
    
    sustraccion(imagenAux,nuevaAux,info.width,info.height); 
    int umbral=thotsu(imagenSustrac,info.width,info.height);
    otsu=binarizar(imagenSustrac,umbral,info.width,info.height);               //la binarizacion de la imagen depende no exactamente Ostu*/

    for(i=0;i<4;i++){
        nhs[i]=i;                                                            //creacion de hilos
        pthread_create(&tids[i],NULL,subImagen,(void *)&nhs[i]);               //crea varios hilos
    }
    for(k=0;k<4;k++){
         pthread_join(tids[k],(void**)&hilo);                                //espera a que termine la ejecucion del hilo
         printf("\nHilo %d terminado\n",*hilo);
    }
    for(i=0;i<4;i++){
        nhs[i]=i;                                                            //creacion de hilos
        pthread_create(&tids[i],NULL,sauvola,(void *)&nhs[i]);               //crea varios hilos
    }
    for(k=0;k<4;k++){
         pthread_join(tids[k],(void**)&hilo);                                //espera a que termine la ejecucion del hilo
         printf("\nHilo %d terminado\n",*hilo);
    }
    /*
     for(i=0;i<4;i++){
        nhs[i]=i;                                                            //creacion de hilos
        pthread_create(&tids[i],NULL,dilata,(void *)&nhs[i]);               //crea varios hilos
    }
    for(k=0;k<4;k++){
         pthread_join(tids[k],(void**)&hilo);                                //espera a que termine la ejecucion del hilo
         printf("\nHilo %d terminado\n",*hilo);
    }*/


    //MOMENTO 1 CON HILO
    int h[7];
    pthread_t tidh[7];
    pthread_create(&tidh[0],NULL,momento1h,(void *)&h[0]); 
    pthread_create(&tidh[1],NULL,momento2h,(void *)&h[2]); 
    pthread_create(&tidh[2],NULL,momento3h,(void *)&h[3]); 
    pthread_create(&tidh[3],NULL,momento4h,(void *)&h[4]); 
    pthread_create(&tidh[4],NULL,momento5h,(void *)&h[5]); 
    pthread_create(&tidh[5],NULL,momento6h,(void *)&h[6]); 
    pthread_create(&tidh[6],NULL,momento7h,(void *)&h[7]); 
    for(int i=0;i<7;i++){
        pthread_join(tidh[i],(void**)&hilo);                                //espera a que termine la ejecucion del hilo
    }
    //momentoHu(imgBin);*/

    GraytoRGB(imagenRGB,imgBin,info.width,info.height);
    guardarBMP("imagen_process.bmp",&info,imagenRGB);
    GraytoRGB(imagenRGB,imagenSustrac,info.width,info.height);
    guardarBMP("imagen_sustrac.bmp",&info,imagenRGB);
    GraytoRGB(imagenRGB,imgSau,info.width,info.height);
    guardarBMP("imagen_sauvola.bmp",&info,imagenRGB);

    //liberar memoria
    free(nueva);
    free(nuevaAux);
    free(nuevaBin);
    free(nuevaGray);
    free(imagenRGB); 
    free(imagenGray);
    free(imagenAux);  
    free(imgBin);  
}


/*  FUNCION POR BLOQUES FILTRO GAUSSIANO*/
void * funHilo(void *arg){
    register int y,x,ym,xm;
    int conv,conv2, indiceI,indiceM;
    unsigned char mascara[DIMASK*DIMASK]={1,2,1,
                                          2,4,2,
                                          1,2,1};
    int nh=*(int*)arg;
    int tamBloque=info.height/NUM_HILOS;                                    //bloques por hilo
    int iniBloque=nh*tamBloque;                                             //inicio del bloque
    int finBloque=iniBloque+tamBloque;                                      //fin del bloque
  
        for(y=iniBloque-(nh*DIMASK); y<finBloque-DIMASK;y++){               //recorrer mascara en la imagen
            for(x=0; x<info.width-DIMASK; x++){                             //recorrer mascara en la imagen
                conv=0;
                conv2=0;
                for(ym=y; ym<y+DIMASK; ym++){                               //recorrer mascara para operaciones
                    for(xm=x; xm<x+DIMASK; xm++){                           //recorrer mascara con operaraciones
                        indiceI=(ym)*info.width+xm;                           //indice de la imagen
                        indiceM=(ym-y)*DIMASK+(xm-x);                       //indice de la mascara
                        conv+=imagenGray[indiceI] * mascara[indiceM];       //convolucion
                        conv2+=nuevaGray[indiceI] * mascara[indiceM];
                    }                                                
                }
            conv/=16;
            conv2/=16;                                                       //Dividir la máscara  
            indiceI=(y+1)*info.width+(x+1);
            imagenAux[indiceI]=conv;
            nuevaAux[indiceI]=conv2;
            }
        }
    pthread_exit(arg);
}


/* AGREGAR BRILLO A LA IMAGEN */
void brilloImagen(unsigned char* imagenGray,uint32_t width, uint32_t height){
    register int x,y;
    int indice,brillo;
    for(y=0;y<height;y++){
        for(x=0;x<width;x++){
            indice=(y*width+x);
            brillo=imagenGray[indice]+40;
            imagenGray[indice]=(brillo>255)?255:brillo;

        }
    }
}
/*CONVERTIR IMAGEN DE GRISES */
void GraytoRGB(unsigned char* imagenRGB, unsigned char* imagenGray,uint32_t width, uint32_t height){
   register int x,y;
    int indiceRGB,indiceGray;
    unsigned char nivelGris;

    for(y=0;y<height;y++){
        for(x=0;x<width;x++){
            indiceGray=(y*width+x);
            indiceRGB=indiceGray*3; //pixel en forma lineal se le suma el tamaño del wodth  y se multiplica por 3 por los pixeles rgb
            //compensasion por la longitud de onda de cada color
            nivelGris=imagenGray[indiceGray];
            imagenRGB[indiceRGB]=nivelGris;
            imagenRGB[indiceRGB+1]=nivelGris;
            imagenRGB[indiceRGB+2]=nivelGris;
        }
    } 
}

/*CONVERTIR A NIVELES DE GRISES */
unsigned char* RGBToGray(unsigned char *imagenRGB,uint32_t width, uint32_t height){
    register int x,y;
    int indiceRGB,indiceGray;
    unsigned char nivelGris;
    unsigned char *imgGray;
    imgGray=reservarMemoria(width,height);
    for(y=0;y<height;y++){
        for(x=0;x<width;x++){
            indiceGray=(y*width+x);
            indiceRGB=indiceGray*3; //pixel en forma lineal se le suma el tamaño del wodth  y se multiplica por 3 por los pixeles rgb
            //compensasion por la longitud de onda de cada color
            nivelGris=((imagenRGB[indiceRGB]*11)+(imagenRGB[indiceRGB+1]*59)+(imagenRGB[indiceRGB+2]*30))/100; //se guarda como bgr 
            imgGray[indiceGray]=nivelGris;
        }
    }
    return imgGray;
}

/* SECCION DE SEGMENTACION DE IMAGEN*/

/*
Etiquetado de regiones obtenidas del algoritmo sauvola
Conectividad-4 vecinos, horizontal y vertical

*/
/*
void etiquetado(unsigned char * imagen, uint32_t width, uint32_t height){       //Segmentacion por umbral
    register int x,y;
    int indice,aux;
    int etiqueta=0; //Gurda y asigna el numero de etiqueta
    int equivalencia[2]={0,0}; //Guarda la equivalencia
    unsigned char *img=imagen;
    
    for(y=0;y<height;y++){
        for(x=0;x<width;x++){
            indice=(y*width+x); //ubicacion pixel actual
            if (img[indice]==1){
                if(img[y*width+(x-1)]==0 && img[(y-1)*width+x]==0){
                    img[indice]=etiqueta;
                    etiqueta++;
                }
                if(img[y*width+(x-1)]==1 && img[(y-1)*width+x]==0){
                    img[indice]=img[y*width+(x-1)];
                }
                if(img[y*width+(x-1)]==0 && img[(y-1)*width+x]==1){
                    img[indice]=img[(y-1)*width+x];
                }
                if(img[y*width+(x-1)]==0 && img[(y-1)*width+x]==1){
                    img[indice]=img[img[y*width+(x-1)];
                    equivalencia[0]=y*width+(x-1);
                    equivalencia[1]=(y-1)*width+x;
                }if() //Arreglo igual break
            }
        }
    }

    printf("Se encontraron %d objetos", etiqueta);

}*/

/*ELEMENTO ESTRUCTURA
Elimina los objetos que son de área pequeña en la imagen
se aplica en la imagen resultante de Souvola una vez que es dilatada
*/
void * dilata(void *arg){
    register int y,x,ym,xm;
    int bloque=200;
    int b2=80;
    int conv,indiceI,indiceM,aux;
    int nh=*(int*)arg;
    //int temp[bloque*bloque];
    int tamBloque=info.height/NUM_HILOS;                                    //bloques por hilo
    int iniBloque=nh*tamBloque;                                             //inicio del bloque
    int finBloque=iniBloque+tamBloque;                                      //fin del bloque
  
        for(y=iniBloque-(nh*b2); y<finBloque-b2;y++){               //recorrer imagen
            for(x=0; x<info.width-bloque; x++){                             
                conv=0;
                for(ym=y; ym<y+b2; ym++){                               //Recorre vecindad
                    for(xm=x; xm<x+bloque; xm++){                           
                        indiceI=(ym)*info.width+xm;                           
                        aux=imgSau[indiceI];
                        if(aux==255)
                            conv++;
                    }                                                
                }
            if(conv<=20){
                for(ym=y; ym<y+b2; ym++){                              
                    for(xm=x; xm<x+bloque; xm++){                          
                        indiceI=(ym)*info.width+xm;                           
                        aux=imgSau[indiceI]*0;                       
                        imgSau[indiceI]=0;
                    }                                                
                }
            }
            }
        }
    pthread_exit(arg);
}




/*UMBRALIZACION LOCAL METODO2
Souvola ayuda a rellenar regiones dentro de obtejos o engrosarlos en imagenes binarias
por lo que se calcula la media de pixeles y la desviacion estandar
Se utiliza la media de la imagen umbralizada localmente con la mediana (imgBin) para engorsar la zona deseada
tomando como base la imagen sustraccion
https://www.tamps.cinvestav.mx/~wgomez/diapositivas/AID/Clase19.pdf <- Metodo Souvola
*/
void* sauvola(void *arg){
register int y,x,ym,xm;
    int bloque=15;
    float media,desv;
    int a,k=0.2;                                                            //Los valores de k 0.2 o 0.5
    double umbral;
    int indiceI;
    int nh=*(int*)arg;
    int tamBloque=info.height/NUM_HILOS;                                    //bloques por hilo
    int iniBloque=nh*tamBloque;                                             //inicio del bloque
    int finBloque=iniBloque+tamBloque;   
  
        for(y=iniBloque-(nh*bloque); y<finBloque-bloque;y++){               //recorrer imagen
            for(x=0; x<info.width-bloque; x++){                             
                media=0.0;
                desv=0.0;
                umbral=0;
                a=0;
                for(ym=y; ym<y+bloque; ym++){                               //recorrer vecindad segun bloque
                    for(xm=x; xm<x+bloque; xm++){                           
                        indiceI=(ym)*info.width+xm;                         
                        media=media+imgBin[indiceI];                        //calculo de media
                        a=a+pow(imagenSustrac[indiceI],2);                  //calculo de desviacion
                    }                                                
                }
            //CALCULO DE MEDIA Y DESV
            media=media/(bloque*bloque);
            desv=sqrt(a/(bloque*bloque)-pow(media,2));

            umbral=media*(1+(k*(desv/128)-1));                              //Calculo de umbral segun Souvola

            indiceI=(y+1)*info.width+(x+1);
            imgSau[indiceI]=((imagenSustrac[indiceI])>=(umbral))?0:255;
            }
        }
     pthread_exit(arg); 
}


/*UMBRALIZACION LOCAL
Se obtiene una subimagen de tamaño bloque para umbralizar la imagen de forma local
para obtener un mejor resultado se utilizan los pixeles para la media de la imagen umbralizada general con otsu
así se obtiene la media de los pixeles en 0 y 1 para obtener una imagen dilatada.

*/
void* subImagen(void *arg){
    register int y,x,ym,xm,i,j;
    int bloque=5;
    int indiceI,temp[bloque*bloque],cont,aux;
    int nh=*(int*)arg;
    int tamBloque=info.height/NUM_HILOS;                                    //bloques por hilo
    int iniBloque=nh*tamBloque;                                             //inicio del bloque
    int finBloque=iniBloque+tamBloque;   

  
        for(y=iniBloque-(nh*bloque); y<finBloque-bloque;y++){               //recorrer imagen
            for(x=0; x<info.width-bloque; x++){                             
                cont=0;
                for(ym=y; ym<y+bloque; ym++){                               //recorrer subimagen
                    for(xm=x; xm<x+bloque; xm++){                           
                        indiceI=(ym)*info.width+xm;                         //indice de la subimagen
                        temp[cont]=otsu[indiceI];                           //Obtiene los valores de los pixeles en la subimagen
                        cont++;
                    }                                                
                }
            //ordenamiento de valores
            for (i=1;i<(bloque*bloque);i++)
                for (j=0;j<(bloque*bloque)-1;j++)
                    if (temp[j]>temp[j+1]){
                        aux=temp[j];
                        temp[j]=temp[j+1];
                        temp[j+1]=aux;
                    }
            indiceI=(y+1)*info.width+(x+1);
            imgBin[indiceI]=((imagenSustrac[indiceI])>=(temp[5]))?0:255;     //Se obtiene el valor de la media de los pixeles ordenados
            }
        }
    
     pthread_exit(arg);   
}

/* METODO DE UMBRALIZACION DE OTSU */
int thotsu(unsigned char* imagen, uint32_t width, uint32_t height){
    int i;
    int histograma[255];
    register int x,y;
    int ValorMax = 0;
    int indice,h=0,sum=0;
    float sumB = 0;
	int wB = 0,wF = 0;
	float varMax = 0;
	int threshold = 0;

    for(i=0; i<256; i++){                                                     //Valores de histograma en cero
        histograma[i]=0;
    }
    
    for(y=0;y<height;y++){                                                    //Creacion del histograma por clases de pixeñes
        for(x=0;x<width;x++){
            indice=(y*width+x);
            h=imagen[indice];
            histograma[h]+=1;
            if (histograma[h] > ValorMax)                                     //Valor maximo de pixeles contabilizados 
                ValorMax = histograma[h];
        }
    }

    for(int t=0;t<256;t++){ 
        sum+=t*histograma[t];
    }
	for(int t=0;t<256;t++){
		wB += histograma[t];					                              // Clase 1
		if (wB == 0) 
            continue;
		wF = (height*width) - wB;						                      // Clase 2
		if (wF == 0) 
            break;
		sumB += (float) (t * histograma[t]);
		float mB = sumB / wB;				                                  // Media Clase 1
		float mF = (sum - sumB) / wF;		                                  // Media Clase 2
		float varBetween = (float)wB * (float)wF * (mB - mF) * (mB - mF);	  // Calculo de varianza

		if (varBetween > varMax){                                             // Umbral maximizado
			varMax = varBetween;
			threshold = t;   
		}
	}
    //printf("Umbralizacion OTSU: %d \n",threshold);
    return threshold;   
}

/*METODO DE UMBRALIZACION POR SEGMENTOS DE IMAGEN*/
unsigned char * binarizar(unsigned char * imagen, int umbral, uint32_t width, uint32_t height){     
    register int x,y;
    int indice;
    unsigned char * bina;
    bina=reservarMemoria(width,height);
    for(y=0;y<height;y++){
        for(x=0;x<width;x++){
            indice=(y*width+x);
            bina[indice]=((imagen[indice])>=umbral)?255:0;                      //Binarizacion de imagen

        }
    }
    return bina;
}


/*METODO DE SUSTRACCION DE IMAGENES (IMAGEN BASE - IMAGEN OBTENIDA)*/
void sustraccion(unsigned char* imagenBase, unsigned char* imagenRep,uint32_t width, uint32_t height){
    register int x,y;
    int indice,aux,cambio,i;
    for(y=0;y<height;y++){
        for(x=0;x<width;x++){
            indice=(y*width+x);
            aux=imagenBase[indice]-imagenRep[indice];
            if(aux>=255)
                aux=255;
            if(aux<=0)
                aux=0;
            imagenSustrac[indice]=aux;
            

        }
    }
}

unsigned char* reservarMemoria(uint32_t width, uint32_t height ){
    unsigned char * imagen;
    //reservamos espacio para la nueva imagen del tamaño de los piexeles contenidos
    imagen=(unsigned char*)malloc(width*height*sizeof(unsigned char));
    if(imagen==NULL){
        perror("Error");
        exit(EXIT_FAILURE);
    }
    return imagen;
}


/* SECCIÓN DE EXTRACCION DE RASGOS (MOMENTOS DE HU) */

/*OBTENER AREA*/
int areaObjeto(unsigned char *imagen,uint32_t width, uint32_t height){
    register int x,y;
    int cont=0,indice;
    for(y=0;y<height;y++){
        for(x=0;x<width;x++){
            indice=(y*width+x);
            if(imagen[indice]==255)                                                     //Area de pixeles 255
                cont++;
        }
    }
    //printf("Area objeto: %d",cont);
    return cont;  
}

/* MOMENTOS GEOMETRICOS */
int momento(unsigned char *imagen,int i,int j){
    register int x,y;
    int indice;
    int aux,tam,cont;
    int *arreglo;
    tam=areaObjeto(imagen,info.width,info.height);                                       //tamaño de arreglo
    arreglo = (int*)malloc(tam*sizeof(int));                                             //reservar memoria
	cont=0;
    //Saca el momento de la imagen
    for(y=0;y<info.height;y++){
        for(x=0;x<info.width;x++){
            indice=(y*info.width+x);
            if(imagen[indice]==255){
                aux=(pow(x,i)*pow(info.height-(y+1),j));
                arreglo[cont]=aux;
                cont++;
            }
        }
    }
    //Sumatoria de (x^i y^j)
    int sum=0;
    for(x=0; x<tam; x++){
        sum=sum+arreglo[x];
    }
    return sum;
}

/* MOMENTO NORMALIZADO*/
double normalizar(unsigned char *imagen, int mn,int p,int q){
    double mnormal;
    double r;
    r=1+((p+q)/2);
	int tam=areaObjeto(imagen,info.width,info.height);                                  //El momento M00 es igual al áera de imagen
    mnormal=mn/(pow(tam,r));
    return mnormal;
}

/*MOMENTO 1*/
void * momento1h(void *arg){
    int cx,cy,u20,u2;
    double n02,n20,m1;
    unsigned char *imagen=imgBin;                                                        //imagen umbralizada
    //momentos estandar
    cx=(momento(imagen,1,0))/(momento(imagen,0,0));                                      //momento estandar ẍ
    cy=(momento(imagen,0,1))/(momento(imagen,0,0)); 
    printf("Cx %d, Cy %d",cx,cy);                                  //momento estandar ÿ
    //printf("\n ---- MOMENTO 1 ---- \nMomentos Estandar: ẍ=%d , ÿ=%d ",cx,cy);
    //calculo de momentos centrales
    u20=momento(imagen,2,0)-(cx*momento(imagen,1,0));
    u2=momento(imagen,0,2)-(cy*momento(imagen,0,1));
	//printf("\nMomentos centrales: u20=%d, u02=%d ",u2,u20);
	//Calculo de momentos normalizados
    n20=normalizar(imagen,u20,2,0);
    n02=normalizar(imagen,u2,0,2);
	//printf("\nMomentos normales: n20=%f, n02=%f ",n20,n02);
    m1=n20+n02;
    printf("\n\nθ1: %f\n",m1);
    pthread_exit(arg);
}
/*MOMENTO 2*/
void * momento2h(void *arg){
    unsigned char * imagen=imgBin;
    int cx,cy,u20,u02,u11;
    double n02,n20,n11,m2;
    //momentos estandar
    cx=(momento(imagen,1,0))/(momento(imagen,0,0));                                      //momento estandar ẍ
    cy=(momento(imagen,0,1))/(momento(imagen,0,0));                                      //momento estandar ÿ
    //printf("\n ---- MOMENTO 2 ---- \nMomentos Estandar: ẍ=%d , ÿ=%d ",cx,cy);
    //calculo de momentos centrales
    u20=momento(imagen,2,0)-(cx*momento(imagen,1,0));
    u02=momento(imagen,0,2)-(cy*momento(imagen,0,1));
    u11=momento(imagen,1,1)-(cx*momento(imagen,0,1));
	printf("\nMomentos centrales: u20=%d, u02=%d , u11=%d ",u20,u02,u11);
	//Calculo de momentos normalizados
    n20=normalizar(imagen,u20,2,0);
    n02=normalizar(imagen,u02,0,2);
    n11=normalizar(imagen,u11,1,1);
	//printf("\nMomentos normales: n20=%f, n02=%f, n11= %f ",n20,n02,n11);
    
    m2=pow(n20-n02,2)+(4*pow(n11,2));
    printf("\n\nθ2: %f\n",m2);
    pthread_exit(arg);
}

/*MOMENTO 3*/
void * momento3h(void *arg){
    double cx,cy,u30,u03,u21,u12;
    double n30,n03,n21,n12,m3;
    unsigned char * imagen=imgBin;
    //momentos de orden 3
    cx=(momento(imagen,1,0))/(momento(imagen,0,0));                 //momento estandar ẍ
    cy=(momento(imagen,0,1))/(momento(imagen,0,0));
    //printf("\n ---- MOMENTO 3 ---- \nMomentos Estandar: S=%d , Sÿ=%d ",Sx,Sy);
    //calculo de momentos centrales
    u30=momento(imagen,3,0)-3*(cx*momento(imagen,2,0))+2*(momento(imagen,1,0)*pow(cx,2));
    u03=momento(imagen,0,3)-3*(cy*momento(imagen,0,2))+2*(momento(imagen,0,1)*pow(cy,2));
    u12=momento(imagen,1,2)-2*(cy*momento(imagen,1,1))-(cx*momento(imagen,0,2))+2*(momento(imagen,1,0)*pow(cy,2));
    u21=momento(imagen,2,1)-2*(cx*momento(imagen,1,1))-(cy*momento(imagen,2,0))+2*(momento(imagen,0,1)*pow(cx,2));
    //printf("\nMomentos centrales: u30=%d, u03=%d , u12=%d , u21=%d ",u30,u03,u12,u21);
    //Calculo de momentos normalizados
    n30=normalizar(imagen,u30,3,0);
    n03=normalizar(imagen,u03,0,3);
    n12=normalizar(imagen,u12,1,2);
    n21=normalizar(imagen,u21,2,1);
    //printf("\nMomentos normales: n30=%f, n03=%f, n12= %f, n21= %f ",n30,n03,n12,n21);
    
    m3= pow(n30+(3*n12),2)+pow((3*n21)-n03,2);  
    printf("\n\nθ3: %f\n",m3);
    pthread_exit(arg);
}

/*MOMENTO 4*/
void * momento4h(void *arg){
    unsigned char * imagen=imgBin;
    int cx,cy,u30,u03,u21,u12;
    double n30,n03,n21,n12,m4;
    //momentos estandar
   cx=(momento(imagen,1,0))/(momento(imagen,0,0));                 //momento estandar ẍ
   cy=(momento(imagen,0,1))/(momento(imagen,0,0));                                    //momento estandar ÿ
    //printf("\n ---- MOMENTO 4 ---- \nMomentos Estandar: ẍ=%d , ÿ=%d ",cx,cy);
    //calculo de momentos centrales
    u30=momento(imagen,3,0)-3*(cx*momento(imagen,2,0))+2*(momento(imagen,1,0)*pow(cx,2));
    u03=momento(imagen,0,3)-3*(cy*momento(imagen,0,2))+2*(momento(imagen,0,1)*pow(cy,2));
    u12=momento(imagen,1,2)-2*(cy*momento(imagen,1,1))-(cx*momento(imagen,0,2))+2*(momento(imagen,1,0)*pow(cy,2));
    u21=momento(imagen,2,1)-2*(cx*momento(imagen,1,1))-(cy*momento(imagen,2,0))+2*(momento(imagen,0,1)*pow(cx,2));
    //printf("\nMomentos centrales: u30=%d, u03=%d , u12=%d , u21=%d ",u30,u03,u12,u21);
    //Calculo de momentos normalizados
    n30=normalizar(imagen,u30,3,0);
    n03=normalizar(imagen,u03,0,3);
    n12=normalizar(imagen,u12,1,2);
    n21=normalizar(imagen,u21,2,1);
    //printf("\nMomentos normales: n30=%f, n03=%f, n12= %f, n21= %f ",n30,n03,n12,n21);
    
    m4= pow(n30+n12,2)+pow(n21+n03,2);   
    printf("\n\nθ4: %f\n",m4);
    pthread_exit(arg);
}

/*MOMENTO 5*/
void * momento5h(void *arg){
    unsigned char * imagen=imgBin;
    int cx,cy,u30,u03,u21,u12;
    double n30,n03,n21,n12,p1,p2,p3,p4,m5;
    //momentos estandar
    cx=(momento(imagen,1,0))/(momento(imagen,0,0));                 //momento estandar ẍ
    cy=(momento(imagen,0,1))/(momento(imagen,0,0));                                    //momento estandar ÿ
    //printf("\n ---- MOMENTO 5 ---- \nMomentos Estandar: ẍ=%d , ÿ=%d ",cx,cy);
    //calculo de momentos centrales
    u30=momento(imagen,3,0)-3*(cx*momento(imagen,2,0))+2*(momento(imagen,1,0)*pow(cx,2));
    u03=momento(imagen,0,3)-3*(cy*momento(imagen,0,2))+2*(momento(imagen,0,1)*pow(cy,2));
    u12=momento(imagen,1,2)-2*(cy*momento(imagen,1,1))-(cx*momento(imagen,0,2))+2*(momento(imagen,1,0)*pow(cy,2));
    u21=momento(imagen,2,1)-2*(cx*momento(imagen,1,1))-(cy*momento(imagen,2,0))+2*(momento(imagen,0,1)*pow(cx,2));
    //printf("\nMomentos centrales: u30=%d, u03=%d , u12=%d , u21=%d ",u30,u03,u12,u21);
    //Calculo de momentos normalizados
    n30=normalizar(imagen,u30,3,0);
    n03=normalizar(imagen,u03,0,3);
    n12=normalizar(imagen,u12,1,2);
    n21=normalizar(imagen,u21,2,1);

    //printf("\nMomentos normales: n30=%f, n03=%f, n12= %f, n21= %f ",n30,n03,n12,n21);
    p1=(n30-3*n12)*(n30+n12);
    p2=pow((n30+n12),2)-3*pow((n21+n03),2);
    p3=(3*n21-n03)*(n21+n03);
    p4=3*pow((n30+n12),2)-pow((n21+n03),2);
    
    m5= p1*p2+p3*p4;
    printf("\n\nθ5: %f\n",m5);  
    pthread_exit(arg);
}

/*MOMENTO 6*/
void * momento6h(void *arg){
    unsigned char * imagen=imgBin;
    int cx,cy,u30,u03,u21,u12,u20,u02,u11;
    double n30,n03,n21,n12,n02,n20,n11,p1,p2,p3,m6;
    //momentos estandar
    cx=(momento(imagen,1,0))/(momento(imagen,0,0));                                      //momento estandar ẍ
    cy=(momento(imagen,0,1))/(momento(imagen,0,0));                                       //momento estandar ÿ
    //printf("\n ---- MOMENTO 6 ---- \nMomentos Estandar: ẍ=%d , ÿ=%d ",cx,cy);
    //calculo de momentos centrales
    u20=momento(imagen,2,0)-(cx*momento(imagen,1,0));
    u02=momento(imagen,0,2)-(cy*momento(imagen,0,1));
    u11=momento(imagen,1,1)-(cx*momento(imagen,0,1));
    u30=momento(imagen,3,0)-3*(cx*momento(imagen,2,0))+2*(momento(imagen,1,0)*pow(cx,2));
    u03=momento(imagen,0,3)-3*(cy*momento(imagen,0,2))+2*(momento(imagen,0,1)*pow(cy,2));
    u12=momento(imagen,1,2)-2*(cy*momento(imagen,1,1))-(cx*momento(imagen,0,2))+2*(momento(imagen,1,0)*pow(cy,2));
    u21=momento(imagen,2,1)-2*(cx*momento(imagen,1,1))-(cy*momento(imagen,2,0))+2*(momento(imagen,0,1)*pow(cx,2));
    //printf("\nMomentos centrales: u30=%d, u03=%d , u12=%d , u21=%d ",u30,u03,u12,u21);
    //Calculo de momentos normalizados
    n30=normalizar(imagen,u30,3,0);
    n03=normalizar(imagen,u03,0,3);
    n12=normalizar(imagen,u12,1,2);
    n21=normalizar(imagen,u21,2,1);
    n20=normalizar(imagen,u20,2,0);
    n02=normalizar(imagen,u02,0,2);
    n11=normalizar(imagen,u11,1,1);

    //printf("\nMomentos normales: n30=%f, n03=%f, n12= %f, n21= %f n20=%f, n02=%f, n11= %f ",n30,n03,n12,n21,n20,n02,n11);
    
    p1=n20-n02;
    p2=pow((n30+n12),2)-pow((n21+n03),2);
    p3=4*n11*(n30+n12)*(n21+n03);
    
    m6= p1*p2+p3; 
    printf("\n\nθ6: %f\n",m6); 
    pthread_exit(arg);
}

/*MOMENTO 7*/
void * momento7h(void *arg){
    unsigned char * imagen=imgBin;
    int cx,cy,u30,u03,u21,u12;
    double n30,n03,n21,n12,p1,p2,p3,p4,m7;
    //momentos estandar
    cx=(momento(imagen,1,0))/(momento(imagen,0,0));                 //momento estandar ẍ
    cy=(momento(imagen,0,1))/(momento(imagen,0,0));                                    //momento estandar ÿ
    //printf("\n ---- MOMENTO 7 ---- \nMomentos Estandar: ẍ=%d , ÿ=%d ",cx,cy);
    //calculo de momentos centrales
    u30=momento(imagen,3,0)-3*(cx*momento(imagen,2,0))+2*(momento(imagen,1,0)*cx*cx);
    u03=momento(imagen,0,3)-3*(cy*momento(imagen,0,2))+2*(momento(imagen,0,1)*cy*cy);
    u12=momento(imagen,1,2)-2*(cy*momento(imagen,1,1))-(cx*momento(imagen,0,2))+2*(momento(imagen,1,0)*cy*cy);
    u21=momento(imagen,2,1)-2*(cx*momento(imagen,1,1))-(cy*momento(imagen,2,0))+2*(momento(imagen,0,1)*cx*cx);
    printf("\nMomentos centrales: u30=%d, u03=%d , u12=%d , u21=%d ",u30,u03,u12,u21);
    //Calculo de momentos normalizados
    n30=normalizar(imagen,u30,3,0);
    n03=normalizar(imagen,u03,0,3);
    n12=normalizar(imagen,u12,1,2);
    n21=normalizar(imagen,u21,2,1);

    //printf("\nMomentos normales: n30=%f, n03=%f, n12= %f, n21= %f n20=%f, n02=%f, n11= %f ",n30,n03,n12,n21,n20,n02,n11);
    
    p1=(3*n21-n03)*(n30+n12);
    p2=pow((n30+n12),2)-(3*pow((n21+n03),2));
    p3=(3*n12-n30)*(n21+n03);
    p4=(3*pow((n30+n12),2))-pow((n21+n03),2);
    
    m7= p1*p2+p3*p4; 
    printf("\n\nθ7: %f\n",m7);
    pthread_exit(arg);
}
