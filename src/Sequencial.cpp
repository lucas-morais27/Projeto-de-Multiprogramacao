#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <chrono>

using namespace std;

vector<vector<int>> cria_matriz(string arquivo_entrada) {
    vector<vector<int>> matriz;
    int n_linha, n_coluna;
    string linha;
    ifstream arquivo_matriz(arquivo_entrada);

    //Pega as dimensões da matriz.
    getline(arquivo_matriz, linha);
    stringstream linha_coluna(linha);
    linha_coluna >> n_linha;
    linha_coluna >> n_coluna;

    //Cria a matriz.
    for (int i = 0; i < n_linha; i++) {
        getline(arquivo_matriz, linha);
        stringstream numeros(linha);
        vector<int> linha_inteiros; //Recebe todos os números de cada linha.
        string item;
        for (int j = 0; j < n_coluna; j++) {
            while (getline(numeros, item, ' ')) {
                linha_inteiros.push_back(stoi(item));
            }
        }
        matriz.push_back(linha_inteiros);
    }
    
    arquivo_matriz.close();
    
    return matriz;
}

int main(int argc, char *argv[2]) {  
    vector<vector<int>> matriz_resultado;
    vector<vector<int>> matriz1 = cria_matriz(argv[1]);
    vector<vector<int>> matriz2 = cria_matriz(argv[2]);
    vector<int> linha;

    //Inicia cálculo do tempo.
    chrono::steady_clock::time_point inicio = chrono::steady_clock::now();

    //Multiplica as matrizes.
    for(int i = 0; i < matriz1.size(); i++){
        linha.clear();
        for(int j = 0; j < matriz2[0].size(); j++){
            linha.push_back(0);
            for(int k = 0, l = 0; k < matriz1[i].size(), l < matriz2.size(); k++, l++){
                linha[linha.size()-1] += (matriz1[i][k] * matriz2[l][j]);
            }
        }
        matriz_resultado.push_back(linha);
    }

    //Fim de cálculo do tempo.
    chrono::steady_clock::time_point fim = chrono::steady_clock::now();

    int tempo = chrono::duration_cast<chrono::milliseconds>(fim - inicio).count(); 

    //Escreve o resultado de cada multiplicação.
    fstream arquivo_resultado;
    arquivo_resultado.open("../data/sequencial.txt", ios::app);
    arquivo_resultado << matriz_resultado.size() << " " << matriz_resultado[0].size() << endl;
    
    for (auto m = 0; m < matriz_resultado.size(); m++) {
        for (auto n = 0; n < matriz_resultado[0].size(); n++) {
            arquivo_resultado << "c" << m + 1 << n + 1 << " " << matriz_resultado[m][n] << endl;
        }
    }

    arquivo_resultado << tempo;
    arquivo_resultado.close();
    
    return 0;
}