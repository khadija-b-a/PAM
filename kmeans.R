# Chargement des librairies
library(stats)
library(cluster)
# Lecture du fichier
data <‐ read.csv2("choixpeauMagique.csv", header=T, row.names=1)
# Normalisation des données (centrer réduire), vecteur Maison exclu
scale(data[,1:4])
# Calcul de la matrice des distances selon la distance de manhattan
d <‐ dist(data[,1:4], method="manhattan")
# Initialisation de la racine
set.seed(123)
# Algo k‐means tourne 1 fois
kmeans1 <‐ kmeans(d,centers = 4, nstart = 1)
# Affichage du résultat pour obtenir BSS/TSS
kmeans1
# On calcule le coefficient silhouette sur k‐means 1
sil_kmeans1 <‐ silhouette(kmeans1$cluster,d)
# On stocke la table des clusters sur k‐means 1
kmeans1 <‐ table(kmeans1$cluster)
# On refait tourner l'algo k‐means 5 fois
kmeans5 <‐ kmeans(d,centers = 4, nstart = 5)
kmeans5
# On calcule le coefficient silhouette sur k‐means 5
sil_kmeans5 <‐ silhouette(kmeans5$cluster,d)
# On stocke la table des clusters sur k‐means 5
kmeans5 <‐ table(kmeans5$cluster)
# On affiche le nombre d'éléments par clusters
print(kmeans1)
print(kmeans5)
# On affiche les informations silhouettes pour kmeans1
summary(sil_kmeans1)
summary(sil_kmeans5)
