#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <vector>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

chrono::steady_clock::time_point inicio;
chrono::steady_clock::time_point fim;

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

void i_processos(int P, int posicao, vector<vector<int>> matriz1, vector<vector<int>> matriz2){
    int processo_atual = posicao/P;
    int linha_i = posicao / matriz2[0].size(); //Linha da posição atual.
    int linha_f = (posicao + P) / matriz2[0].size(); //Linha da última posição.
    int coluna_i = posicao % matriz2[0].size(); //Coluna da posição atual
    int coluna_f = (posicao + P) % matriz2[0].size(); //Coluna da última posição.

    //Verifica se ainda existe um nº de elementos menor ou igual que P e efetua a troca do índice.
    if (posicao + P >= matriz1.size() * matriz2[0].size()){ 
        linha_f = (matriz1.size() * matriz2[0].size()) / matriz2[0].size();
        coluna_f = (matriz1.size() * matriz2[0].size()) % matriz2[0].size();
    }
    
    vector<int> resultado; //Vetor auxiliar para guardar as multiplicações do processo atual.
    //Multiplica as matrizes.
    while(linha_i != linha_f || coluna_i != coluna_f){
        resultado.push_back(0);
        for (int i = 0; i < matriz1[linha_i].size(); i++) {
            resultado[resultado.size()-1] += matriz1[linha_i][i] * matriz2[i][coluna_i];
        }
        coluna_i++;
        if (coluna_i >= matriz2[0].size()){
            coluna_i = 0;
            linha_i++;
        }
    }
    //Fim de cálculo do tempo.
    fim = chrono::steady_clock::now();

    linha_i = posicao / matriz2[0].size();
    coluna_i = posicao % matriz2[0].size();

    //Escreve o resultado de cada multiplicação.
    ofstream arquivo_processo("../data/processo" + to_string(processo_atual) + ".txt");
    for (int i = 0; i < resultado.size(); i++, coluna_i++){
        arquivo_processo << "c" << linha_i + 1 << coluna_i + 1 << " " << resultado[i]<<endl;
        if (coluna_i >= matriz2[0].size() - 1){
            coluna_i = -1;
            linha_i++;
        }
    }

    //Calculo do tempo.
    int tempo = chrono::duration_cast<chrono::milliseconds>(fim - inicio).count();  
    arquivo_processo << tempo;
    arquivo_processo.close();
}

int main(int argc, char *argv[3]){
    vector<vector<int>> matriz1 = cria_matriz(argv[1]);  
    vector<vector<int>> matriz2 = cria_matriz(argv[2]);
    int P = atoi(argv[3]); //Quantidade de elementos por processo.
    int n_processos = (matriz1.size() * matriz2[0].size()) / P; //Quantidade de processos.
    if ((matriz1.size() * matriz2[0].size()) % P != 0) { n_processos++; }
    pid_t processos[n_processos]; //Vetor com os processos necessários.

    //Inicia cálculo do tempo.
    inicio = chrono::steady_clock::now();
    for (int i = 0; i < n_processos; i++){
        processos[i] = fork();
        if (processos[i] == 0){
            i_processos(P, P*i, matriz1, matriz2);
            exit(0);
        }
    }
    wait(NULL);

    return 0;
}