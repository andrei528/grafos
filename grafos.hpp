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

    bool insertAresta(int origem, int destino, int peso = 1){
        if(origem < 0 || origem >= numVertices || destino < 0 || destino >= numVertices) return false;

        int valor = pondered ? peso : 1;

        matrizAdj[origem][destino] = valor;
        if(!directed){
            matrizAdj[destino][origem] = valor;
        }
        return true;
    }

    bool removeAresta(int origem, int destino){
        if(origem < 0 || origem >= numVertices || destino < 0 || destino >= numVertices) return false;

        matrizAdj[origem][destino] = 0;
        
        if(!directed){
            matrizAdj[destino][origem] = 0;
        }
        return true;
    }

    bool existeAresta(int origem, int destino){
        if(origem < 0 || origem >= numVertices || destino < 0 || destino >= numVertices) return false;

        return matrizAdj[origem][destino] > 0;
    }
};

class GrafoLista : public Grafos
{
private:
    LDEVertices listaVertices;

public:
    GrafoLista(bool directed, bool pondered, int numVertices) : Grafos(directed, pondered, numVertices), listaVertices()
    {
        for (int i = 0; i < numVertices; i++)
        {
            insertVertice(&listaVertices, i);
        }
    }

    ~GrafoLista()
    {
        deleteVertices(&listaVertices);
    }

    bool insertAresta(int origem, int destino, int peso = 1){

        NodeVertice *vOrigem = findVertice(&listaVertices, origem);
        NodeVertice *vDestino = findVertice(&listaVertices, destino);

        if (vOrigem == NULL || vDestino == NULL) return false;

        int valor = pondered ? peso : 1;

        insertNode(&vOrigem->listaAdjacencia, destino, valor);

        if(!directed){
            insertNode(&vDestino->listaAdjacencia, origem, valor);
        }
        return true;
    }

    bool removeAresta(int origem, int destino){

        NodeVertice *vOrigem = findVertice(&listaVertices, origem);
        NodeVertice *vDestino = findVertice(&listaVertices, destino);

        if (vOrigem == NULL || vDestino == NULL) return false;

        removeNode(&vOrigem->listaAdjacencia, destino);

        if(!directed){
            removeNode(&vDestino->listaAdjacencia, origem);
        }

        return true;
    }

    bool existeAresta(int origem, int destino){
        
        NodeVertice *vOrigem = findVertice(&listaVertices, origem);
        NodeVertice *vDestino = findVertice(&listaVertices, destino);

        if (vOrigem == NULL || vDestino == NULL) return false;

        return searchNode(&vOrigem->listaAdjacencia, destino);
    }
};