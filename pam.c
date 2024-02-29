#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <math.h>
#include<time.h>
#include "pam.h"
#define square(a)  (a)*(a)

void recupere_fichier(int Col, float **P, char *nom, char **L, char **C){
	FILE *f;
	int i=0;
	char ligne[1024];
	char *ptr_chaine;
	f = fopen(nom,"r"); //ouvre le fichier
	fgets(ligne,1024,f); 
	const char* separateur = " ,;/-!";//séparateur possible pour les données
  //il faut s'assurer qu'entre les lignes, il y a bien des retours a la ligne sinon le prog ne marche pas
	ptr_chaine = strtok(ligne,separateur);
	for(int j=0;j<Col;j++){
		C[j] = malloc((strlen(ptr_chaine)+1)*sizeof(char));
		sscanf(ptr_chaine, "%s", C[j]);
		ptr_chaine = strtok(NULL,separateur);			
	} //recupère le nom des colonnes

	while(fgets(ligne,1024,f) != NULL){
		ptr_chaine = strtok(ligne,separateur); // récupère la chaine avec avant le point virgule
		L[i] = malloc((strlen(ptr_chaine)+1)*sizeof(char));
		sscanf(ptr_chaine, "%s", L[i]);//stocke le nom des lignes
		for(int j=0;j<Col-1;j++){// récupère la chaine avec avant le point virgule pour chaque valeur
			ptr_chaine = strtok(NULL,separateur);
			sscanf(ptr_chaine, "%f", &P[i][j]);
		}
		i++;
	}
	fclose(f);
}

//génération d'une matrice de distance nb_sommet*nb_sommet 
void matrice_dist(float** distance, float **P, int col, int row){
	int i,j; 
  for(i=0;i<row;i++){
    for(j=i;j<row;j++){ float d=0;
    if(i==j) {distance[i][j]=distance[j][i]=0;}
    else { int k=0; 
      for(k=0; k<col; k++){
      d=d+fabs(P[i][k]-P[j][k]);
      }
    }
    distance[i][j]=distance[j][i]=d; 
    }
  }
}

//Choisir aléatoirement k sommet et les définir comme centroïde
void centre_alea(int k, int * centre, int row){
	srand(time(NULL));
	int i, a; 
	for(i=0; i<k; i++){
		do{ a=rand()%row;} while(verif_centre(centre,i,a));
     centre[i]=a;
      }
}

int verif_centre(int *centre, int k, int a){
		int i; 
    for(i=0; i<k;i++) if(centre[i]==a)return 1;	
		return 0;
}

//initialiser au début tous les sommets tq'ils n'appartiennent à aucun cluster
void init_cluster(int * cluster, int row){
	int i; 
	for (i=0; i<row; i++){
		cluster[i]=0;
	}
}

//définir pour sommet le cluster auquel il appartient donc trouver le centre des k centre ayant la distance minimale avec le sommet i
void def_cluster(int *cluster, int *centre, int k, int row, float ** distance){
	int i, j; 
  	for(i=0; i<row; i++){
    	float min=distance[i][centre[0]];
    	int clust=1;
      for(j=1;j<k;j++){ 
        if(min>distance[i][centre[j]]) 
        {min=distance[i][centre[j]]; clust=j+1; }
      }
  	cluster[i]=clust; 
  }
}

//retourne 1 si les 2 tabs ne contiennent pas les mêmes centres sinon retourne 0
int diff_centre(int* centre, int* nouv_centre, int k){
	int i;
	for(i=0;i<k;i++){
  	if(centre[i]!=nouv_centre[i]) return 1;
 	}
	return 0;   
}

//calculer retourne la somme des distance d'un i appartenant à un cluster x à tous les points (j) du cluster x
float cout(int i, float** distance,int row, int* cluster){
	int j;
  float somme=0;
	for(j=0; j<row;j++){
		if(cluster[i]==cluster[j]) {somme=somme+distance[i][j];}
	}
 
	return somme;  
}

// calcul le sommet qui a le cout le plus petit comparé au cout S du centre original i et renvoie cette somme ainsi que 
int meilleur_changement(int S, int i, float** distance,int row, int* cluster){
	int nobjets = i;
	float Stemp = 0;
	for(int j=0; j<row;j++){
		if(cluster[j]==cluster[i] && j!=i ){ 
			Stemp = cout(j, distance, row, cluster);
		if(S>Stemp){
			S = Stemp;
			nobjets = j;
		}}
	}
	return nobjets;
}

//retourne la moyenne des valeurs pour une ligne donnée
float moy_tab(float** p, int row,int i){
	int j; 
	float somme=0;
	for(j=0; j<row;j++){somme=somme+p[j][i];}
	somme=somme/row;
	return somme;   
}

//calcul de l'écart-type pour une ligne donnée à partir de la moyenne et des valeurs de la lignes 
float ecart_type(float **p, int row, int i, float moy){
	int j; float somme=0;
	for(j=0; j<row; j++){somme=somme+square(p[j][i]-moy);}
	somme=sqrtf(somme/row);
	return somme;   
}

//centre et réduit les données d'un tableau de float de taille row*col 
void centrer_reduire_donnee(float **p, int row, int col){
	int i,j; 
	for(j=0;j<col;j++){ 
  	float moy=moy_tab(p,row,j),ecartype=ecart_type(p,row,j,moy);
  	for(i=0; i<row; i++){
  		p[i][j]=(p[i][j]-moy)/ecartype;  
  	}
	}
}

//demande à l'utilisateur s'il veut centrer et réduire les données puis  le faire si la répnse est oui
void utilisateur_donnees(float **p, int row, int col) {
 	char centre_reduit; 
  printf("Souhaitez-vous que les données soient centrées et réduites?\n");
  printf("Y pour Yes, N pour No\n");
  scanf("%c",&centre_reduit);
  if(centre_reduit=='Y' || centre_reduit=='y') centrer_reduire_donnee(p, row, col);
}


void afficher_donnees(int Row, int Col, float **P){
	for(int i = 0; i<Row-1; i++){
		for(int j = 0; j<Col-1; j++){
			printf("%.2f |", P[i][j]);
		}
		printf("\n");
	}
} 

void affiche_distance(float** distance, int row){
  int i; 
  for(i=0;i<row;i++){
    printf(" distance de %d à %d = %.2f\n ",i, 1, distance[i][1]);
  } 
}

void affiche_cluster(int * cluster, int row, int k, char **L, int nbaff){ 
	int aff;
	for( int i =1; i<=k;i++){
		aff = 0;
		printf("\nCluster %d :\n",i);
		for(int j=0; j<row;j++){
			if(aff == nbaff){
				printf("\n");
				aff=0;
			}
			if(cluster[j]==i) {
				printf("%s, ", L[j]);
				aff++;
			}
		}
		printf("\n");
	}
}

void affiche_chif_clust(int*cluster, int row){
	int i; 
	for(i=0; i<row;i++){ printf("sommet %d cluster %d\n",i+1,cluster[i]);}
}

void affiche_chif_centre(int* centre, int k){
	int i; 
	for(i=0;i<k;i++){
  	printf("cluster %d: %d \n", i+1, centre[i]);
	}
}

void affiche_centre(int * centre, int k, char **L){
	for(int i = 0; i<k; i++){
		printf("L'objet choisi est %s\n", L[centre[i]]);
	}
}

//on peut utiliser cette fonction pour P et distance uniquement qui sont des tab de float à double entrées
void free_tab(float**p, int row){
	int i; 
	for(i=0; i<row; i++){
 		free(p[i]);
  }
  free(p);
}

void free_char(char**p, int row){
	int i; 
	for(i=0; i<row; i++){
 		free(p[i]);
  }
  free(p);
}

void fichier_distance( float** distance, int row) {
	FILE *f;
	f = fopen("sortie_distance.txt","w"); //ouvre le fichier en mode écriture
	if (f==NULL) {
  	printf("problème d'ouverture du fichier\n");
  	exit(3);
	}
	int i,j; 
	for(i=0;i<row; i++){
  	for(j=0;j<row;j++){
    	fprintf(f,"%.2f ; ", distance[i][j]);
  	}
  	fprintf(f,"\n");
	}
  fclose(f);
}

void fichier_cluster(int *centre, int *cluster, int row, int k){
	FILE *f1;
	f1 = fopen("sortie_cluster.txt","w");
	if (f1==NULL) {
  	printf("problème d'ouverture du fichier\n");
  	exit(3);
	}
	int i,j;
	for(j=0; j<k;j++){ 
		fprintf(f1,"Centre Cluster %d : %d \n",j+1, centre[j]+1); 
	}
	fprintf(f1,"\n");
	for(j=0; j<k;j++){ 
  	fprintf(f1,"Cluster %d :  ",j+1);
		for(i=0;i<row; i++){ 
  		if(cluster[i]==(j+1)) fprintf(f1," %d ;  ",i+1);
		} 
		fprintf(f1,"\n");
	}
	fclose(f1);
}