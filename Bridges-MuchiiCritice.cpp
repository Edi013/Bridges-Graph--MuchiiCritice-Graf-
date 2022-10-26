// Algoritmul de parcurgere in adancime (DFS). Reprezentarea grafului se face cu matrice de adiacenta.

#include <stdio.h>
#include <string.h>

#pragma warning(disable:4996)

#define NMAX 100

char vecin[NMAX][NMAX]; // Matricea de adiacenta a grafului

char visit[NMAX];      // visit[k] = 1 daca nodul k a fost vizitat si 0 in caz contrar

int trace[NMAX];       // tablou ce pastreaza nodurile in ordinea in care sunt intalnite la parcurgerea in latime
int kth;               // numarul de elemente vizitate

int prenum[NMAX];
int low[NMAX];


void dfs(int n, short u) {
    int v;

    visit[u] = 1;        // marcheaza nodul u ca fiind vizitat
    prenum[u] = kth;
    trace[kth++] = u;    // adauga nodul u in tabloul ce contine lista nodurilor deja vizitate //trace ==  prenum

    for (v = 0; v < n; v++) {       // pentru toate nodurile grafului
        if ((vecin[u][v] == 1) && !visit[v]) {  // verifica daca u si v sunt vecine si daca v nu a fost vizitat
            dfs(n, v);                  // apeleaza recursiv dfs pentru nodul v
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
        fscanf(fin, "%d %d", &u, &v);  // citeste o pereche de noduri adiacente

        u--;                  // scade 1 din valorile nodurilor
        v--;                  // transforma nodurile din intervalul 1...n in 0...(n-1)
        vecin[u][v] = 1;      // marcheaza faptul ca u si v sunt adiacente
        vecin[v][u] = 1;
    }

    dfs(n, start);          // porneste din start sa viziteze nodurile folosind strategia DFS

    // afiseaza in ordine nodurile vizitate
    for (i = 0; i < kth; i++) {
        fprintf(fout, "%d ", trace[i] + 1);
    }

    int traceLength = kth;
    
    //calculam low pt fiecare, luate pe inversul parcurgerii,  din trace le luam.

    kth--;
    for (; kth >= 0; kth-- ) {
        int nodCurent = trace[kth];
        int maxPrenumeVecini = -1;
        int minPrenumeVecini = prenum[nodCurent];
        low[nodCurent] = prenum[nodCurent]; //#1
        int nrVecini = 0;
        for (v = 0; v < n; v++) {// v = nodIncercat
            nrVecini++;
           // prenum [copil] > prenum[parinte] && max( de prenume dintre vecini.)
                if (vecin[nodCurent][v] == 1)
                {
                    if (prenum[v] < prenum[nodCurent]) // e muchie de intoarcere sau parinte
                    {
                        //pt a gasii tatal cautam prenumele maxim dintre cei mai mici decat el
                        if (maxPrenumeVecini < prenum[v])
                            maxPrenumeVecini = prenum[v];
                        //pt a gasii o muchie de intoarcere cautam prenumele minim dintre cei mai mici decat el cu cond sa fie diferit de prenumele maxim si de prenum[nodCurent]
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
        int nodCurent = trace[i];
        int maxPrenumeVecini = -1;
        int nodCopil = -1;
            for (v = 0; v < n; v++) {                                                              
                if (vecin[nodCurent][v] == 1)
                {
                    if (prenum[v] > prenum[nodCurent]) // e copil
                    {
                        nodCopil = v;
                        //pt a gasii tatal cautam prenumele maxim dintre cei mai mici decat el
                        if (prenum[nodCurent] < low[nodCopil]) // e muchie critica  // nodCurent e viziune de parinte. low de copil trb
                            printf("%d %d - bridge\n", nodCurent + 1, nodCopil + 1);

                    }
                }
            }

            

    }
    

    fclose(fin);
    fclose(fout);

    return 0;
}