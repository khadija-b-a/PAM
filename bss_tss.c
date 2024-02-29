#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bss_tss.h"

// calcule la moyenne globale de toutes les données stockées dans P
float moyenne(float** P, int row, int col){
float S=0;
for(int i=0; i<row-1; i++){
  for(int j=0; j<col-1; j++){
    S=S+P[i][j];
  }
}
S=S/((row-1)*(col-1));
return S;   
}

// calcule la somme des écarts au carré à la moyenne totale (tout le jeu de données)
float tss(float **P, int row, int col){
float tss=0;
float moy=moyenne(P, row, col);
for(int i=0; i<row-1; i++){
  for(int j=0; j<col-1; j++){
    tss=tss+(moy-P[i][j])*(moy-P[i][j]);
  }
}
return tss;
}

// calcule la moyenne d'un vecteur j pour un cluster k
float moy_vect(float **P, int *cluster, int row, int col, int k, int j){
float som=0;
int cpt=0;
for(int i=0; i<row-1;i++){
  if(cluster[i]==k){
    cpt++;
    som=som+P[i][j];}
}
return som/cpt;
}

// calcule le wss (somme des écarts au carrés à la moyenne) au sein d'un cluster k
float wss_k(float **P, int *cluster, int row, int col, int k){
float som=0;
for(int j=0;j<col-1; j++){
  for(int i=0; i<row-1;i++){
    if(cluster[i]==k){
      som=som+(moy_vect(P,cluster,row,col,k,j)-P[i][j])*(moy_vect(P,cluster,row,col,k,j)-P[i][j]);}
  }
}
return som;
}

// calcule la somme des WSS pour tous les k clusters
float wss_t(float **P, int *cluster, int row, int col, int k){
float wss=0;
for(int j=0; j<k; j++){ 
  wss=wss+wss_k(P,cluster,row,col,j+1);
}
return wss; 
}

// calcule le ratio BSS/TSS jugeant de la qualité du clustering sachant que BSS = TSS - WSS
float qualite(float **P, int*cluster, int row, int col, int k){
float bss;
bss = fabs(tss(P,row,col) - wss_t(P,cluster,row,col,k));
return bss/tss(P,row,col);
}