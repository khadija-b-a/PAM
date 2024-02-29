#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <math.h>
#include<time.h> 
#include "pam.h"
#include "silhouette.h"
#include "bss_tss.h"

int main (int argc, char *argv[]) {
	int Col=0;
	int Row=0;
	int i;
  int k; 
	int nbaff;
	char **C;
  char **L;
	float **P; //matrice pour stocker les valeurs (row-1)*(col-1)
  float **distance; //matrice N*N concerve les distances de chaque sommet vers tous les autres
	int E;
	int o;
 
  
	if(argc == 4){
		Row = atoi(argv[1]);
		Col = atoi(argv[2]);
	}
	else exit(124);

	P = malloc ((Row-1)*sizeof(float*));

  if(P==NULL){
    printf("problème d'allocation de mémoire\n");
    exit(2); 
	}

	for(i = 0; i<Row-1; i++){
		P[i] = malloc((Col-1)*sizeof(float));
	}
	
	C = malloc (Col*sizeof(char*));
  if(C==NULL){
    printf("problème d'allocation de mémoire\n");
    exit(2); 
	}

	L = malloc ((Row-1)*sizeof(char*));
  if(L==NULL){
    printf("problème d'allocation de mémoire\n");
    exit(2); 
	}
			
	recupere_fichier(Col,P,argv[3],L,C);
	//afficher_donnees(Row, Col, P);
  utilisateur_donnees(P, Row-1, Col-1);
  //afficher_donnees(Row-1, Col-1, P);

  //création de la matrice de distance
  distance= malloc ((Row-1)*sizeof(float*));
  if(distance==NULL){
    printf("problème d'allocation de mémoire\n");
    exit(2);
	}

  for(i = 0; i<Row-1; i++){
		distance[i] = malloc((Row-1)*sizeof(float));
	}
  //implémentation de la matrice des distance
  matrice_dist(distance, P, Col-1, Row-1);
  //affiche_distance(distance, Row-1);

  //entrer le nombre de cluster à générer
  do{
    printf("Veuillez saisir le nombre de cluster à former :\n");
    scanf("%d",&k);
    if(k>Row-1){printf("Le nombre de cluster ne peut excéder le nombre de points/objets du jeu de données.\n");}
	} while(k>Row-1);

  //choix aléatoire des k centre
  int centre[k]; int nouv_centre[k]; int cluster[Row-1]; 
 
  centre_alea(k, centre, Row-1);
  printf("LES CENTRES AU DEBUT*****************\n");
  affiche_centre(centre,k,L);
	//affiche_chif_centre(centre, k);
  printf("*************************************\n");
  init_cluster(cluster, Row-1);

	for(i = 0; i<k; i++){
		nouv_centre[i] = centre[i];
	}
	do{
  	def_cluster(cluster, centre, k, Row-1, distance);

  	for(i = 0; i<k; i++){
			centre[i] = nouv_centre[i];
	  }

	  for(i = 0; i<k; i++){ 
			E = cout(centre[i], distance, Row-1, cluster);
			o = meilleur_changement(E, centre[i], distance, Row-1, cluster); 
			if(centre[i]!=o){
				nouv_centre[i]=o; 
			}
	  }
	}while(diff_centre(centre, nouv_centre,k));

  printf("LES CENTRES A LA FIN*****************\n");
  affiche_centre(centre,k,L);
  printf("*************************************\n");
	printf("\nVeuillez saisir le nombre d'objets affichés par ligne de la console :\n");
  scanf("%d",&nbaff);
  affiche_cluster(cluster,Row-1,k,L,nbaff);
  printf("\nSILHOUETTE*************************\n");  
  printf("le coef silhouette dans notre cas est %.3f\n",s_total(distance, cluster,Row-1,k));
  printf("************************************\n");

  printf("\nBSS/TSS= %.3f\n", qualite(P, cluster, Row, Col, k));
  printf("************************************\n");

  fichier_distance(distance, Row-1);
  fichier_cluster(centre,cluster,Row-1,k);
  

  free_tab(P, Row-1);
	free_char(L,Row-1);
	free_char(C,Col);
  free_tab(distance,Row-1);
}