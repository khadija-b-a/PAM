#include <stdio.h>
#include <stdlib.h>
#include "silhouette.h"


//calcul a pour un sommet donné i
float a_centre(float** distance, int* cluster, int i, int row){
int j, nb=0;
float somme=0; 
for(j=0; j<row; j++){
if(cluster[i]==cluster[j] && i!=j){ 
  nb++; somme=somme+distance[i][j];
}}
if(somme!=0)somme=somme/nb;
return somme;   
}

//calcul la distance d'un point i a un cluster donné numero nb_clust
float sommet_cluster(float **distance, int * cluster, int row, int i, int nb_clust){
int k, nb=0; float somme_dist=0; 
for(k=0; k<row; k++){
  if(cluster[k]==nb_clust){somme_dist=somme_dist+distance[i][k];nb++;}
}
somme_dist=somme_dist/nb;
return somme_dist;   
}

//calcul b pour un sommet donné i
float b_calcul(float** distance, int * cluster, int row, int i, int k){
float min=0; int j=1;
while(cluster[i]==j) j++;
min=sommet_cluster(distance,cluster,row,i,j);
for(j=1; j<=k;j++){
  if(cluster[i]!=j) {float d= sommet_cluster(distance,cluster,row,i,j); 
  if(min>d) {min=d;}}
}
 return min; 
}

//calcul s pour un sommet donné i
float s_sommet(float** distance, int * cluster, int row, int k, int i){
  float a=0, b=0;
  a= a_centre( distance, cluster,i,row);
  b=b_calcul(distance,cluster,row,i,k);
float s=0; 
if(a>b)s=(b-a)/a;
else s=(b-a)/b;
return s;  
}

float s_total(float** distance, int* cluster, int row, int k){
int i; float s_t=0;
for(i=0;i<row;i++){
  s_t=s_t+s_sommet(distance, cluster, row,k,i);
}
s_t=s_t/row;
return s_t;  
}