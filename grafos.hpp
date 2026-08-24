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
};