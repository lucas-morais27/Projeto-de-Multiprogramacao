#include <iostream>
#include <string>
#include <fstream>

using namespace std;

int main(int argc, char* argv[4]) {
    fstream arquivo_matriz1, arquivo_matriz2;
    arquivo_matriz1.open("../data/matriz1.txt", ios::app);
    arquivo_matriz2.open("../data/matriz2.txt", ios::app);

    srand((unsigned) time(0));

    int n1 = atoi(argv[1]), m1 = atoi(argv[2]), n2 = atoi(argv[3]), m2 = atoi(argv[4]); 
    
    //Gerando matriz 1
    arquivo_matriz1 << n1 << " " << m1 << endl;
    for (int i = 0; i < n1; i++) {
        for (int j = 0; j < m1; j++) {
            arquivo_matriz1 << (rand()%10) << " ";
        }
        arquivo_matriz1 << endl;
    }
    arquivo_matriz1.close();

    //Gerando matriz 2
    arquivo_matriz2 << n2 << " " << m2 << endl;
    for (int k = 0; k < n2; k++) {
        for (int l = 0; l < m2; l++) {
            arquivo_matriz2 << (rand()%10) << " ";
        }
        arquivo_matriz2 << endl;
    }
    arquivo_matriz2.close();
    
    return 0;
}