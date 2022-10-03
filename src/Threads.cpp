#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>

using namespace std;

vector< vector <int>> matriz1;  //!<Matriz 1 
vector< vector <int>> matriz2;  //!<Matriz 2  
vector<vector<int>> matriz_resultado; 

int P;  //!<Número de elementos por thread
chrono::steady_clock::time_point comeco;  //!<Tempo inicial das threads 

/*! Constroi matriz via dados de arquivo de texto.
    \param string Nome do endereço do arquivo de texto com dados da matriz. 
    \return Matriz de inteiros. 
*/  
vector< vector <int>> buildMatriz(string arq){
    vector< vector <int>> matriz;
    int n,m;
    int aux;
    string linha_str;

    ifstream arquivo("../data/"+arq+".txt");
    getline(arquivo, linha_str);
    stringstream dimensao(linha_str);  
    dimensao >> n;    
    dimensao >> m;

    for(int i=0; i<n; i++){
        getline(arquivo, linha_str);
        stringstream elemento_ss(linha_str);
        vector<int> linha_int;
        for(int j=0; j<m; j++){
            elemento_ss << linha_str;
            elemento_ss >> aux;
            linha_int.push_back(aux);
        }
        matriz.push_back(linha_int);
    }
    arquivo.close();
    
    return matriz;
}

void definir_matriz_vazia(vector<vector<int>> matriz, int linhas, int colunas){
    matriz.reserve(linhas);

    for(int l = 0; l < linhas; l++){
        matriz.push_back(vector<int>(colunas));
    }
}

void* implementacao_threads(void *i){
    vector<int> produto;  //!<Vetor dos produtos dos elementos da thread atual
    
    const int linhas_matriz1 = matriz1.size();
    const int colunas_matriz2 = matriz2[0].size();

    int pos_inicial = ((int)(size_t)i) * P;  //!<Posição atual na matriz produto
    int pos_final = (((int)(size_t) i) + 1) * P - 1;

    int linha = pos_inicial / colunas_matriz2;  //!<Linha da posição atual
    int coluna  = pos_inicial % colunas_matriz2;  //!<Coluna da posição atual

    int linha_final = (pos_final + P) / colunas_matriz2;  //!<Linha da ultima posição da thread atual
    int coluna_final = (pos_final + P) % colunas_matriz2;  //!<Coluna da ultima posição da thread atual

    if(pos_final > linhas_matriz1 * colunas_matriz2){
        linha_final = linhas_matriz1 - 1;
        coluna_final = colunas_matriz2 - 1;
    }

    //Multiplicação das matrizes

    comeco = chrono::steady_clock::now();
    while(linha != linha_final || coluna != coluna_final){
        produto.push_back(0);
        for(int i=0; i<matriz1[0].size(); i++)
            produto[produto.size()-1] += matriz1[linha][i] * matriz2[i][coluna];
        
        linha++;
        if(coluna >= colunas_matriz2){
            coluna =0;
            linha++;
        }
    }
    chrono::steady_clock::time_point fim = chrono::steady_clock::now();  //!<Tempo final da thread atual
    int tempo = chrono::duration_cast<chrono::milliseconds>(fim - comeco).count();  //!<Tempo total da execução da thread atual
    
    //Escrita
    linha = pos_inicial / matriz2[0].size();
    coluna = pos_inicial % matriz2[0].size();

    fstream thread_arquivo;
    thread_arquivo.open("../data/thread_"+to_string(pos_inicial/P)+".txt", ios::app);
    
    for(int i=0; i<produto.size(); i++, coluna++){
        thread_arquivo<<"c"<<linha+1<<"x"<<coluna+1<<" "<<produto[i]<<endl;
        if(coluna >= matriz2[0].size()){
            coluna=0;
            linha++;
        }
    }
    thread_arquivo<<tempo;
    thread_arquivo.close();

    pthread_exit(NULL);
}

int main(int argc, char *argv[]){
    matriz1 = buildMatriz(argv[1]);
    matriz2 = buildMatriz(argv[2]);
    definir_matriz_vazia(matriz_resultado, matriz1.size(), matriz2[0].size());

    P = atoi(argv[3]);
    
    int NTHREADS = (matriz1.size() * matriz2[0].size() )/P;  //!<Quantidade de threads a ser usadas em esse programa
    if((matriz1.size() * matriz2[0].size())%P != 0)//se P não divide a quantidade de elementos da matriz produto
        NTHREADS++;

    pthread_t threads[NTHREADS];  //!<Vetor dos IDs das threads
    
    //comeco = chrono::steady_clock::now();
    for(int i=0; i<NTHREADS; i++)
        pthread_create(&threads[i], NULL, implementacao_threads, (void*)(size_t)(i));

    for(int i=0; i<NTHREADS; i++)
        pthread_join(threads[i], NULL);

    return 0;
}