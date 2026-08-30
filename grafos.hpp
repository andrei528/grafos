#pragma once
#include <iostream>
#include <vector>
#include "LDE.hpp"

using namespace std;

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
    vector<vector<int>> matrizAdj;

public:
    GrafoMatriz(bool directed, bool pondered, int numVertices) : Grafos(directed, pondered, numVertices), matrizAdj(numVertices, vector<int>(numVertices, 0))
    {
    }

    // Funções de vértices 

    // Inserir vertice
    bool insertVertice(){
        for(int i = 0; i < numVertices; i++){
            matrizAdj[i].push_back(0);
        }
        vector<int> vetorTemp(numVertices + 1, 0);
        matrizAdj.push_back(vetorTemp);
        numVertices++;
        return true;
    }

    bool insertAresta(int origem, int destino, int peso = 1)
    {
        if (origem < 0 || origem >= numVertices || destino < 0 || destino >= numVertices)
            return false;

        int valor = pondered ? peso : 1;

        matrizAdj[origem][destino] = valor;
        if (!directed)
        {
            matrizAdj[destino][origem] = valor;
        }
        return true;
    }

    bool removeAresta(int origem, int destino)
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

    bool existsAresta(int origem, int destino)
    {
        if (origem < 0 || origem >= numVertices || destino < 0 || destino >= numVertices)
            return false;

        return matrizAdj[origem][destino] > 0;
    }

    void show()
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
};

class GrafoLista : public Grafos
{
private:
    LDE<LDE<int>> listaVertices;

public:
    GrafoLista(bool directed, bool pondered, int numVertices) : Grafos(directed, pondered, numVertices), listaVertices()
    {
        for (int i = 0; i < numVertices; i++)
        {
            insertNode(&listaVertices, i, LDE<int>());
        }
    }

    ~GrafoLista()
    {
        Node <LDE<int>> *aux = listaVertices.head;
        while(aux != NULL){
            Node <LDE<int>> *temp = aux;
            deleteLDE(&temp->data);
            delete temp;
            aux = aux->prox;
        }
        listaVertices.head = listaVertices.tail = NULL;
    }

    // funções básicas para vértice

    // Insere um novo vertice
    bool insertVertice(){
        insertNode(&listaVertices, numVertices, LDE<int>());
        numVertices++;

        return true;
    }

    bool insertAresta(int origem, int destino, int peso = 1)
    {

        Node <LDE<int>> *vOrigem = findNode(&listaVertices, origem);
        Node <LDE<int>> *vDestino = findNode(&listaVertices, destino);

        if (vOrigem == NULL || vDestino == NULL)
            return false;

        int valor = pondered ? peso : 1;

        insertNode(&vOrigem->data, destino, valor);

        if (!directed)
        {
            insertNode(&vDestino->data, origem, valor);
        }
        return true;
    }

    bool removeAresta(int origem, int destino)
    {

        Node <LDE<int>> *vOrigem = findNode(&listaVertices, origem);
        Node <LDE<int>> *vDestino = findNode(&listaVertices, destino);

        if (vOrigem == NULL || vDestino == NULL)
            return false;

        removeNode(&vOrigem->data, destino);

        if (!directed)
        {
            removeNode(&vDestino->data, origem);
        }

        return true;
    }

    bool existsAresta(int origem, int destino)
    {

        Node <LDE<int>> *vOrigem = findNode(&listaVertices, origem);
        Node <LDE<int>> *vDestino = findNode(&listaVertices, destino);

        if (vOrigem == NULL || vDestino == NULL)
            return false;

        return searchNode(&vOrigem->data, destino);
    }

    void show()
    {
        cout << "Lista de Adjacencia:" << endl;

        Node <LDE<int>> *vertice = listaVertices.head;

        while (vertice != NULL)
        {
            cout << vertice->id << ": ";

            Node<int> *aresta = vertice->data.head;

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
};