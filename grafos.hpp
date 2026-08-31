#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <queue>
#include <set>
#include <map>
#include <cstdlib>
#include "LDE.hpp"

using namespace std;

bool direc, ponderado;

class Grafos
{
protected:
    bool directed;
    bool pondered;
    int numVertices;

public:
    Grafos(bool directed, bool pondered, int numVertices) : directed(directed), pondered(pondered), numVertices(numVertices) {}
    bool isDirected()
    {
        return directed;
    }
    bool isPondered()
    {
        return pondered;
    }

    virtual ~Grafos() { cout << "Destrutor da classe Grafos chamado."; }
};

class GrafoMatriz : public Grafos
{
private:
    vector<vector<float>> matrizAdj;
    vector<string> labels;

public:
    GrafoMatriz(bool directed, bool pondered, int numVertices) : Grafos(directed, pondered, numVertices), matrizAdj(numVertices, vector<float>(numVertices, 0))
    {
        for(int i = 0; i < numVertices; i++){
            labels.push_back("v" + to_string(i));
        }
    }

    // Funções de vértices 

    // Inserir vertice
    bool inserirVertice(string label){
        for(int i = 0; i < numVertices; i++){
            matrizAdj[i].push_back(0);
        }
        vector<float> vetorTemp(numVertices + 1, 0);
        matrizAdj.push_back(vetorTemp);
        labels.push_back(label);
        numVertices++;
        return true;
    }

    // Remover vertice
    bool removerVertice(int id){
        if (id < 0 || id >= numVertices) return false;
        matrizAdj.erase(matrizAdj.begin() + id);
        for(int i = 0; i < numVertices - 1; i++){
            matrizAdj[i].erase(matrizAdj[i].begin() + id);
        }
        labels.erase(labels.begin() + id);
        numVertices--;
        return true;
    }

    // Retorna o nome do vértice
    string labelVertice(int id){
        if(id < 0 || id >= numVertices) return "Id de vertice nao existe";
        return labels[id];
    }

    // Funções de arestas

    bool inserirAresta(int origem, int destino, float peso = 1.0)
    {
        if (origem < 0 || origem >= numVertices || destino < 0 || destino >= numVertices)
            return false;

        float valor = pondered ? peso : 1;

        matrizAdj[origem][destino] = valor;
        if (!directed)
        {
            matrizAdj[destino][origem] = valor;
        }
        return true;
    }

    // Remove a aresta
    bool removerAresta(int origem, int destino)
    {
        if (origem < 0 || origem >= numVertices || destino < 0 || destino >= numVertices)
            return false;

        matrizAdj[origem][destino] = 0;

        if (!directed)
        {
            matrizAdj[destino][origem] = 0;
        }
        return true;
    }

    // Verifica se a aresta existe
    bool existeAresta(int origem, int destino)
    {
        if (origem < 0 || origem >= numVertices || destino < 0 || destino >= numVertices)
            return false;

        return matrizAdj[origem][destino] > 0;
    }

    // Mostra a matriz do grafo
    void imprimeGrafo()
    {
        cout << "Matriz de Adjacencia:" << endl;

        for (int i = 0; i < numVertices; i++)
        {
            for (int j = 0; j < numVertices; j++)
            {
                cout << matrizAdj[i][j] << " ";
            }

            cout << endl;
        }
    }

    // Retorna o peso da aresta
    float pesoAresta(int origem, int destino){
        if(origem < 0 || origem >= numVertices || destino < 0 || destino >= numVertices){
            return -1;
        }else if(matrizAdj[origem][destino] == 0){
            return -1;
        }
        else{
            return matrizAdj[origem][destino];
        }
    }

    // Função para retornar vizinhos
    vector<int> retornarVizinhos(int id){
        if(id < 0 || id >= numVertices) return {};
        vector<int> vizinhos;
        for(int i = 0; i < numVertices; i++){
            if(matrizAdj[id][i] > 0){
                vizinhos.push_back(i);
            }
        }
        return vizinhos;
    }

    vector<int> listarVertices(){
        vector<int> vertices;
        for(int i = 0; i < numVertices; i++){
            vertices.push_back(i);
        }
        return vertices;
    }
};

struct VerticeInfo{
    string label;
    LDE<float> arestas;
};

class GrafoLista : public Grafos
{
private:
    LDE<VerticeInfo> listaVertices;

public:
    GrafoLista(bool directed, bool pondered, int numVertices) : Grafos(directed, pondered, numVertices), listaVertices()
    {
        for (int i = 0; i < numVertices; i++)
        {
            insertNode(&listaVertices, i, VerticeInfo{"v" + to_string(i), LDE<float>()});
        }
    }

    ~GrafoLista()
    {
        Node <VerticeInfo> *aux = listaVertices.head;
        while(aux != NULL){
            Node <VerticeInfo> *temp = aux;
            aux = aux->prox;
            deleteLDE(&temp->data.arestas);
            delete temp;
        }
        listaVertices.head = listaVertices.tail = NULL;
    }

    GrafoLista(GrafoLista &&outro) noexcept : Grafos(outro.directed, outro.pondered, outro.numVertices), listaVertices(outro.listaVertices){
        outro.listaVertices.head = NULL;
        outro.listaVertices.tail = NULL;
    }

    // funções básicas para vértice

    // Insere um novo vertice
    bool inserirVertice(string label){
        int id = (listaVertices.tail == NULL) ? 0 : listaVertices.tail->id + 1;
        insertNode(&listaVertices, id, VerticeInfo{label, LDE<float>()});
        numVertices++;

        return true;
    }

    // Remove um vertice
    bool removerVertice(int id){
        Node <VerticeInfo> *vertice = findNode(&listaVertices, id);
        if (vertice == NULL) return false;

        Node <VerticeInfo> *aux = listaVertices.head;
        while(aux != NULL){
            removeNode(&aux->data.arestas, id);
            aux = aux->prox;
        }
        deleteLDE(&vertice->data.arestas);
        removeNode(&listaVertices, id);
        numVertices--;
        return true;
    }

    // Retorna nome do vertice
    string labelVertice(int id){
        Node <VerticeInfo> *vertice = findNode(&listaVertices, id);

        if(vertice == NULL) return "Id de vertice nao existe";

        return vertice->data.label;
    }

    // Funções de arestas

    //Insere a aresta
    bool inserirAresta(int origem, int destino, float peso = 1.0)
    {

        Node <VerticeInfo> *vOrigem = findNode(&listaVertices, origem);
        Node <VerticeInfo> *vDestino = findNode(&listaVertices, destino);

        if (vOrigem == NULL || vDestino == NULL)
            return false;

        float valor = pondered ? peso : 1;

        insertNode(&vOrigem->data.arestas, destino, valor);

        if (!directed)
        {
            insertNode(&vDestino->data.arestas, origem, valor);
        }
        return true;
    }


    // Remove aresta
    bool removerAresta(int origem, int destino)
    {

        Node <VerticeInfo> *vOrigem = findNode(&listaVertices, origem);
        Node <VerticeInfo> *vDestino = findNode(&listaVertices, destino);

        if (vOrigem == NULL || vDestino == NULL)
            return false;

        removeNode(&vOrigem->data.arestas, destino);

        if (!directed)
        {
            removeNode(&vDestino->data.arestas, origem);
        }

        return true;
    }

    // Verifica se aresta existe
    bool existeAresta(int origem, int destino)
    {

        Node <VerticeInfo> *vOrigem = findNode(&listaVertices, origem);
        Node <VerticeInfo> *vDestino = findNode(&listaVertices, destino);

        if (vOrigem == NULL || vDestino == NULL)
            return false;

        return searchNode(&vOrigem->data.arestas, destino);
    }

    // Mostra a lista do grafo
    void imprimeGrafo()
    {
        cout << "Lista de Adjacencia:" << endl;

        Node <VerticeInfo> *vertice = listaVertices.head;

        while (vertice != NULL)
        {
            cout << vertice->id << ": ";

            Node<float> *aresta = vertice->data.arestas.head;

            while (aresta != NULL)
            {
                cout << aresta->id;

                if (pondered)
                {
                    cout << "(" << aresta->data << ")";
                }

                if (aresta->prox != NULL)
                {
                    cout << " -> ";
                }

                aresta = aresta->prox;
            }

            cout << endl;

            vertice = vertice->prox;
        }
    }

    // Retorna peso da aresta
    float pesoAresta(int origem, int destino){
        Node <VerticeInfo> *vOrigem = findNode(&listaVertices, origem);
        Node <VerticeInfo> *vDestino = findNode(&listaVertices, destino);

        if(vOrigem == NULL || vDestino == NULL) return -1;

        Node <float> *aresta = findNode(&vOrigem->data.arestas, destino);

        if(aresta == NULL) return -1;
        return aresta->data;
    }

    // Retorna vizinhos de vértice
    vector<int> retornarVizinhos(int id){
        Node <VerticeInfo> *vertice = findNode(&listaVertices, id);

        if(vertice == NULL) return {};

        Node <float> *aux = vertice->data.arestas.head;

        vector<int> vizinhos;

        while(aux != NULL){
            vizinhos.push_back(aux->id);
            aux = aux->prox;
        }
        return vizinhos;
    }

    vector<int> listarVertices(){
        vector<int> vertices;
        Node <VerticeInfo> *aux = listaVertices.head;
        while(aux != NULL){
            vertices.push_back(aux->id);
            aux = aux->prox;
        }
        return vertices;
    }

};


template <typename T>
T lerGrafo(string caminho){
    ifstream arquivo(caminho);
    if(!arquivo.is_open()){
        T g(false, false, 0);
        return g;
    }
    int V, A, D, P;
    arquivo >> V >> A >> D >> P;

    direc = (D == 1);
    ponderado = (P == 1);

    T g(direc, ponderado, V);

    int origem, destino = 0;

    while(arquivo >> origem >> destino){
        float peso = 1;
        if(ponderado){
            arquivo >> peso;
        }
        g.inserirAresta(origem, destino, peso);
    }

    return g;
}

template <typename T>
void bfs(T &grafo, int origem){
    queue<int> fila;
    set<int> visitados;

    fila.push(origem);
    visitados.insert(origem);

    while(!fila.empty()){
        int atual = fila.front();
        fila.pop();

        cout << atual << " ";

        vector<int> vizinhos = grafo.retornarVizinhos(atual);
        for(int viz : vizinhos){
            if(!visitados.count(viz)){
                visitados.insert(viz);
                fila.push(viz);
            }
        }
    }
}

template <typename T>
void dfsAux(T &grafo, int atual, set<int> &visitados){
    visitados.insert(atual);
    cout << atual << " ";
    
    vector<int> vizinhos = grafo.retornarVizinhos(atual);
    for(int viz : vizinhos){
        if(!visitados.count(viz)){
            dfsAux(grafo, viz, visitados);
        }
    }
}

template <typename T>
void dfs(T &grafo, int origem){
    set<int> visitados;
    dfsAux(grafo, origem, visitados);
}

void imprimirCaminho(int v, int origem, map<int, int> &anteriores){
    if(v == origem){
        cout << v;
        return;
    }
    if(anteriores.count(v) == 0){
        cout << "sem caminho";
        return;
    }
    imprimirCaminho(anteriores[v], origem, anteriores);
    cout << " -> " << v;
}

template <typename T>
void dijkstra(T &grafo, int origem){
    const float INFINITO = 1e9;
    map<int, float> distancias;
    map<int, int> anteriores;
    set<int> fechados;

    vector<int> vertices = grafo.listarVertices();
    for(int v  : vertices){
        distancias[v] = INFINITO;
    }
    distancias[origem] = 0;

    while(true){
        int atual = -1;
        float menorDist = INFINITO;
        for(int v : vertices){
            if(fechados.count(v) == 0 && distancias[v] < menorDist){
                menorDist = distancias[v];
                atual = v;
            }
        }
        if(atual == -1) break;

        vector<int> vizinhos = grafo.retornarVizinhos(atual);
        for(int viz : vizinhos){
            float novaDistancia = distancias[atual] + grafo.pesoAresta(atual, viz);
            if(distancias[viz] > novaDistancia){
                distancias[viz] = novaDistancia;
                anteriores[viz] = atual;
            }
        }
        fechados.insert(atual);
    }
    for(int v : vertices){
        cout << "Vertice " << v << " - distancia: ";
        if(distancias[v] >= INFINITO){
            cout << "nao alcancavel" << endl;
        }else{
            cout << distancias[v] << " - caminho: ";
            imprimirCaminho(v, origem, anteriores);
            cout << endl;
        }
    }
}

template <typename T>
void menu(T &grafo){
    int opcao;
    do {
        cout << "\n===== MENU =====" << endl;
        cout << "1 - Inserir vertice" << endl;
        cout << "2 - Remover vertice" << endl;
        cout << "3 - Inserir aresta" << endl;
        cout << "4 - Remover aresta" << endl;
        cout << "5 - Verifica se aresta existe" << endl;
        cout << "6 - Peso de uma aresta" << endl;
        cout << "7 - Nome( label) de um vertice" << endl;
        cout << "8 - Vizinhos de um vertice" << endl;
        cout << "9 - Imprimir grafo" << endl;
        cout << "10 - BFS" << endl;
        cout << "11 - DFS" << endl;
        cout << "12 - Dijkstra" << endl;
        cout << "0 - Sair" << endl;
        cout << "Escolha: ";
        cin >> opcao;

        switch(opcao){
            case 1: {
                string label;
                cout << "Label do vertice: ";
                cin >> label;
                grafo.inserirVertice(label);
                break;
            }
            case 2: {
                int id;
                cout << "Id do vertice a remover: ";
                cin >> id;
                grafo.removerVertice(id);
                break;
            }
            case 3:{
                int origem, destino;
                float peso = 1;
                cout << "Origem: "; cin >> origem;
                cout << "Destino: "; cin >> destino;
                if(grafo.isPondered()){
                    cout << "Peso: ";
                    cin >> peso;
                }
                grafo.inserirAresta(origem, destino, peso);
                break;
            }
            case 4:{
                int origem, destino;
                cout << "Origem: "; cin >> origem;
                cout << "Destino: "; cin >> destino;
                grafo.removerAresta(origem, destino);
                break;
            }
            case 5:{
                int origem, destino;
                cout << "Origem: "; cin >> origem;
                cout << "Destino: "; cin >> destino;
                cout << "Existe aresta? " << (grafo.existeAresta(origem, destino) ? "sim" : "nao") << endl;
                break;
            }
            case 6:{
                int origem, destino;
                cout << "Origem: "; cin >> origem;
                cout << "Destino: "; cin >> destino;
                cout << "Peso: " << grafo.pesoAresta(origem, destino) << endl;
                break;
            }
            case 7:{
                int id;
                cout << "Id do vertice: "; cin >> id;
                cout << "Label: " << grafo.labelVertice(id) << endl;
                break;
            }
            case 8:{
                int id;
                cout << "Id do vertice: "; cin >> id;
                vector<int> vizinhos = grafo.retornarVizinhos(id);
                cout << "Vizinhos: ";
                for(int v : vizinhos){
                    cout << v << " ";
                }
                cout << endl;
                break;
            }
            case 9:{
                system("cls");
                grafo.imprimeGrafo();
                cout << endl;
                break;
            }
            case 10:{
                int origem;
                cout << "Vertice de origem: "; cin >> origem;
                cout << "BFS: ";
                bfs(grafo, origem);
                cout << endl;
                break;
            }
            case 11:{
                int origem;
                cout << "Vertice de origem: "; cin >> origem;
                cout << "DFS: ";
                dfs(grafo, origem);
                cout << endl;
                break;
            }
            case 12:{
                int origem;
                cout << "Vertice de origem: "; cin >> origem;
                dijkstra(grafo, origem);
                break;
            }
            case 0:{
                cout << "Saindo..." << endl;
                break;
            }
            default:
            cout << "Opcao invalida" << endl;
        }

    }while(opcao != 0);
}