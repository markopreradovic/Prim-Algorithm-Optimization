#include <iostream>
#include <vector>
#include <limits>
#include <mpi.h>
#include <fstream>

#define MAX 10000
using namespace std;

int genMat[MAX][MAX];
int dim;
char* fileName;
int key[MAX];
int parent[MAX];
bool visited[MAX];

int minKey(int key[], bool visited[]) {
    int min = INT_MAX;
    int min_index=0;
    for (int i = 0; i < MAX; i++) 
        if (visited[i] == false && key[i] < min) {
            min = key[i];
            min_index = i;
        }
    return min_index;
}

void generateMatrix(int s) {               //Generise tezinsku matricu
    int dim = s;

    for (int i = 0; i < dim; i++)
        for (int j = 0; j < dim; j++)
            genMat[i][j] = rand() % 100;

    for (int i = 0; i < dim; i++)          //elementi na glavnoj dijagonali moraju biti 0
        genMat[i][i] = 0;

    for (int i = 0; i < dim; i++)          //matrica mora biti simetricna
        for (int j = 0; j < dim; j++)
            genMat[j][i] = genMat[i][j];
}

void writeMatrix() {
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            cout << genMat[i][j] << " ";
        }
        cout << endl;
    }
}

void inputMatrix() {
    int k, n;
    cout << "Unos matrice (n x k)" << endl;
    cout << "Unesi n - > " << endl;
    cin >> n;
    cout << "Unesi k - > " << endl;
    cin >> k;
    cout << "Unesi matrixu n x k:" << endl;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < k; j++)
            cin >> genMat[i][j];
}


void printMST(char* fileName) {
    cout << "Algoritam: Primov algoritam OpenMPI" << endl;
    cout << "Grana   -    Tezina" << endl;
    ofstream izlazniFajl(fileName);
    izlazniFajl << "OpenMPI: " << endl;
    izlazniFajl << "Grana   -    Tezina" << endl;
    int s = 0;
    for (int i = 1; i < dim; i++) {
        izlazniFajl << "(" << parent[i] << "-" << i << ")     \t " << genMat[i][parent[i]] << endl;
        cout << "(" << parent[i] << "  -  " << i << ")" << "\t" << genMat[i][parent[i]] << endl;
        s += genMat[i][parent[i]];
    }
    izlazniFajl << "Ukupna tezina - > " <<  s << endl;
    cout << "Ukupna tezina - > " << s << endl;
}

void primMST(int rank, int size, char* file) {

    int localMinKey, localMinIndex;

    int globalMinKey, globalMinIndex;

    int local_key[MAX];
    bool local_visited[MAX];

    MPI_Bcast(genMat, MAX * MAX, MPI_INT, 0, MPI_COMM_WORLD);

    int localSegment = MAX / size;
    int startPartition = rank * localSegment;
    int endPartition = startPartition + localSegment;

    for (int i = startPartition; i < endPartition; i++) {
        key[i] = INT_MAX;
        visited[i] = false;
    }
    key[0] = 0;

    for (int count = 0; count < MAX - 1; count++) {
        localMinKey = INT_MAX;
        for (int i = startPartition; i < endPartition; i++) {
            if (visited[i] == false && key[i] < localMinKey) {
                localMinKey = key[i];
                localMinIndex = i;
            }
        }

        MPI_Reduce(&localMinKey, &globalMinKey, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
        MPI_Bcast(&globalMinKey, 1, MPI_INT, 0, MPI_COMM_WORLD);

        if (rank == 0) {
            globalMinIndex = minKey(key, visited);
        }

        MPI_Bcast(&globalMinIndex, 1, MPI_INT, 0, MPI_COMM_WORLD);

        if (globalMinIndex >= startPartition && globalMinIndex < endPartition) {
            visited[globalMinIndex] = true;
        }

        for (int i = startPartition; i < endPartition; i++) {
            if (genMat[globalMinIndex][i]) {
                if (visited[i] == false && genMat[globalMinIndex][i] < key[i]) {
                    key[i] = genMat[globalMinIndex][i];
                    parent[i] = globalMinIndex;
                }
            }
        }
        MPI_Allgather(&key[startPartition], localSegment, MPI_INT, local_key, localSegment, MPI_INT, MPI_COMM_WORLD);
        MPI_Allgather(&visited[startPartition], localSegment, MPI_C_BOOL, local_visited, localSegment, MPI_C_BOOL, MPI_COMM_WORLD);

        for (int i = 0; i < MAX; i++) {
            if (visited[i] == false && local_visited[i / localSegment] == false && local_key[i / localSegment] < key[i]) {
                key[i] = local_key[i / localSegment];
                parent[i] = (i / localSegment) * localSegment + rank * localSegment;
            }
        }
    }
    printMST(file);
        
}

int main(int argc, char* argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);


    ofstream ulazniFajl(argv[1]);
    fileName = argv[2];
    if (!ulazniFajl.is_open()) {
        cout << "Greska: ulazni fajl nije moguce otvoriti!";
        return 1;
    }

    dim = atoi(argv[3]);
    generateMatrix(dim);
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            ulazniFajl << genMat[i][j] << " ";
        }
        ulazniFajl << endl;
    }

    double start_time = MPI_Wtime();
    primMST(rank, size, fileName);
    double end_time = MPI_Wtime();
    double elapsed_time = end_time - start_time;
    cout << "Elapsed time: " << elapsed_time << " seconds." << endl;
   
    MPI_Finalize();
    return 0;
}