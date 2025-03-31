#include <iostream>
#include <fstream>
#include <ctime>

#define MAX 5000

using namespace std;

int genMat[MAX][MAX];
int dim;


void generateMatrix(int s) {  //Generise tezinsku matricu
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

void writeMatrix() {
	for (int i = 0; i < dim; i++) {
		for (int j = 0; j < dim; j++) {
			cout << genMat[i][j] << " ";
		}
		cout << endl;
	}
}

void PrimSeq(char *fileName) {
	cout <<  endl;
	cout << "Algoritam: Primov algoritam - sekvencijalna obrada" << endl;

	ofstream izlazniFajl(fileName);
	int visited[MAX];
	memset(visited, false, sizeof(visited));
	visited[0] = true; 
	cout << "Grana   -    Tezina" << endl;
	izlazniFajl << "Sequential:" << endl;
	izlazniFajl << "Grana   -   Tezina" << endl;
	int count = 0;
	int s = 0; //ukupna tezina
	while (count < dim - 1) {
		int x = 0;
		int y = 0;
		int min = 10000;
		for (int i = 0; i < dim; i++) {
			if (visited[i]) {
				for (int j = 0; j < dim; j++) {
					if (min > genMat[i][j]) {
						if (!visited[j] && genMat[i][j]) {
                           min = genMat[i][j];
						   x = i;
						   y = j;
						}
						
					}
				}
			}
		}
		s += genMat[x][y];
		cout  << "(" << x << "  -  " << y << ")" << "\t" << genMat[x][y] << endl;
		izlazniFajl << "(" << x << "  -  " << y << ")" << "\t" << genMat[x][y] << endl;
		visited[y] = true;
		count++;
	}
	izlazniFajl << "Ukupna tezina - > " << s;
	cout << "Ukupna tezina - > " << s << endl;
	}
	

int main(int argc, char* argv[]) {

	if (argc < 0) {
		cout << "Greska: nedovoljno argumenata!";
		return 1;
	}

	ofstream ulazniFajl(argv[1]);
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
    
	//inputMatrix(); OPCIONO ako zelimo sami da unesemo matricu

	clock_t start = clock();
	PrimSeq(file);
	clock_t stop = clock();
	double time = (stop - start) / (double)CLOCKS_PER_SEC;
	cout << "Vrijeme izvrsavanja: " << time << " s" << endl;

	//writeMatrix();

	return 0;
}