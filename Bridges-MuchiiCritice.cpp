#include <stdio.h>
#include <string.h>

#pragma warning(disable:4996)

#define NMAX 100

char vecin[NMAX][NMAX]; 

char visit[NMAX];      

int trace[NMAX];       
int kth;               

int prenum[NMAX];
int low[NMAX];


void dfs(int n, short u) {
    int v;

    visit[u] = 1;        
    prenum[u] = kth;
    trace[kth++] = u;    

    for (v = 0; v < n; v++) {       
        if ((vecin[u][v] == 1) && !visit[v]) {  
            dfs(n, v);                 
        }
    }

}


int main() {
    FILE* fin, * fout;
    int n, m, u, v, i, start;

    fin = fopen("dfs.in.txt", "r");
    fout = fopen("dfs.out.txt", "w");

    fscanf(fin, "%d %d %d", &n, &m, &start);
    start--;

    for (int i = 0; i < n; i++) {
        prenum[i] = low[i] = -1;
    }

    for (i = 0; i < m; i++) {
        fscanf(fin, "%d %d", &u, &v); 

        u--;                 
        v--;                 
        vecin[u][v] = 1;     
        vecin[v][u] = 1;
    }

    dfs(n, start);        
    
    for (i = 0; i < kth; i++) {
        fprintf(fout, "%d ", trace[i] + 1);
    }

    int traceLength = kth;
    
    kth--;
    for (; kth >= 0; kth-- ) {
        int nodCurent = trace[kth];
        int maxPrenumeVecini = -1;
        int minPrenumeVecini = prenum[nodCurent];
        low[nodCurent] = prenum[nodCurent]; //#1
        int nrVecini = 0;
        for (v = 0; v < n; v++) {
            nrVecini++;
                if (vecin[nodCurent][v] == 1)
                {
                    if (prenum[v] < prenum[nodCurent]) // e muchie de intoarcere sau parinte
                    {
                        //pt a gasii tatal cautam prenum maxim pentru nodurile cu prenum mai mic decat cel curent
                        if (maxPrenumeVecini < prenum[v])
                            maxPrenumeVecini = prenum[v];
                        //pt a gasii o muchie de intoarcere cautam prenumele minim dintre cu prenum mai mic decat cel curent,
                        //cu conditia sa fie diferit de maxPrenumeVecini si de prenum[nodCurent]- conditie la linia 96
                        if (minPrenumeVecini > prenum[v])
                            minPrenumeVecini = prenum[v];
                    }
                    else                          // e copil
                        if (low[nodCurent] > low[v] )
                            low[nodCurent] = low[v];
                }               
        }
        //muchia de intoarcere o verificam dupa ce a trecut prin toti vecinii si daca  exista cel putin un vecin,  un minim diferit de maxim, min dif de prenum[nodCurent]
            if (nrVecini != 0 && minPrenumeVecini != maxPrenumeVecini && minPrenumeVecini != prenum[nodCurent] )// inseamna ca avem muchie de intoarcere;
                if (low[nodCurent] > minPrenumeVecini)
                    low[nodCurent] = minPrenumeVecini;      
    }

    printf("\nPrenum ");
    for (int i = 0; i < n; i++) {
        printf("%d ", prenum[i]);
    }

    printf("\nLow ");
    for (int i = 0; i < n; i++) {
        printf("%d ", low[i]);
    }

    printf("\nBridges:\n");
    //verificam conditia  prenum[parinte] < low[copil] ESTE Bridge : NU ESTE
    for(int i = traceLength-1; i >= 0; i--)
     {
        // nodCurent are viziune de parinte, parcurgand, cu forul de mai jos, copiii parintelui
        int nodCurent = trace[i];
        int maxPrenumeVecini = -1;
        int nodCopil = -1;
            for (v = 0; v < n; v++) {                                                              
                if (vecin[nodCurent][v] == 1)
                {
                    if (prenum[v] > prenum[nodCurent]) // e copil
                    {
                        nodCopil = v;
                        if (prenum[nodCurent] < low[nodCopil]) // e muchie critica  
                            printf("%d %d - bridge\n", nodCurent + 1, nodCopil + 1);
                    }
                }
            }
    }
    
    fclose(fin);
    fclose(fout);

    return 0;
}
