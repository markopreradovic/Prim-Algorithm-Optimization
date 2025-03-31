#include <iostream>
#include <fstream>
#include <omp.h>
#include <stdlib.h>
#include <time.h>

#define MAX 10

using namespace std;

int genMat[MAX][MAX];
int dim;
int parent[MAX];
int key[MAX];
int visited[MAX];

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

void printMST(char *fileName) {
	cout << "Algoritam: Primov algoritam OpenMP" << endl;
	cout << "Grana   -    Tezina" << endl;
	ofstream izlazniFajl(fileName);
	izlazniFajl << "OpenMP:" << endl;
	izlazniFajl << "Grana   -    Tezina" << endl;
	int s = 0; //ukupna tezina
	for (int i = 1; i < dim; i++) {
		int x = parent[i];
		izlazniFajl << "(" << parent[i] << "-" << i << ")     \t " << genMat[i][x] << endl;
		cout << "(" << parent[i] << "  -  " << i << ")" << "\t" << genMat[i][x] << endl;
		s += genMat[i][x];
	}
	izlazniFajl << "Ukupna tezina - > " << s << endl;
	cout << "Ukupna tezina - > " << s << endl;
}

int minEdge() {
	int min = INT_MAX;
	int index = 0;
#pragma omp parallel
	{
	     int index_local = index;
		 int min_local = min;
#pragma omp for nowait
		 for (int i = 0; i < MAX; i++) {
			 if (visited[i] == 0 && key[i] < min_local) {
				 min_local = key[i];
				 index_local = i;
			 }
		 }
#pragma omp critical
		 {
			 if (min_local < min) {
				 min = min_local;
				 index = index_local;
			 }
		 }
	}
	return index;
}

void primOpenMP(char *fileName) {

	int count;
	for (int i = 0; i < MAX; i++) {
		key[i] = INT_MAX;
		visited[i] = 0;
	}
	key[0] = 0;
	parent[0] = -1;

	for (count = 0; count < MAX - 1; count++) {
		int u = minEdge();
		visited[u] = true;
		int v;
#pragma omp parallel for schedule(static)
		for (v = 0; v < MAX; v++) {
			if (genMat[u][v] && visited[v] == 0 && genMat[u][v] < key[v]) {
				parent[v] = u;
				key[v] = genMat[u][v];
			}
		}
	}
	printMST(fileName);
}

int main(int argc, char* argv[])
{
	if (argc < 0) {
		cout << "Greska: nedovoljno argumenata!";
		return 1;
	}

	ofstream ulazniFajl(argv[1]);
	char* fileName = argv[2];
	if (!ulazniFajl.is_open()) {
		cout << "Greska: ulazni fajl nije moguce otvoriti!";
		return 1;
	}

	//Generisanje matrice za ulaz.txt
	//srand(time(NULL));
	char* file = argv[2];
	dim = atoi(argv[3]);
	generateMatrix(dim);
	for (int i = 0; i < dim; i++) {
		for (int j = 0; j < dim; j++) {
			ulazniFajl << genMat[i][j] << " ";
		}
		ulazniFajl << endl;
	}

	//inputMatrix();
	
	double start_time = omp_get_wtime();
	primOpenMP(fileName);
	double end_time = omp_get_wtime();
	cout << "Vrijeme izvrsavanja - > " << end_time - start_time << " s" << endl;

	//writeMatrix();
	return 0;
}