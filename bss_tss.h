float moyenne(float** P, int row, int col);
float tss(float **P, int row, int col);
float wss_k(float **P, int *cluster, int row, int col, int k);
float wss_t(float **P, int *cluster, int row, int col, int k);
float qualite(float **P, int*cluster, int row, int col, int k);
float moy_vect(float **P, int *cluster, int row, int col, int k, int j);