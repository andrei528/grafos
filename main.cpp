#include <iostream>
#include "LDE.hpp"
#include "grafos.hpp"

using namespace std;

int main(){

    LDE <int> list;
    initializeLDE(list);

    insertNode(&list, 5, 20);
    insertNode(&list, 3, 20);
    showLDE(&list);
    
    // Testes de grafo matriz
    GrafoMatriz grafom1(false, false, 5);

    grafom1.insertAresta(1, 1);
    grafom1.insertAresta(2, 1);
    grafom1.insertAresta(3, 1);

    grafom1.removeAresta(3, 1);

    cout << endl;
    cout << "Grafo matriz funcoes :" << endl;
    cout << "1 , 1" << endl;
    cout << grafom1.existeAresta(1, 1) << endl;
    cout << "3, 1" << endl;
    cout << grafom1.existeAresta(3, 1) << endl;
    cout << "1, 2" << endl;
    cout << grafom1.existeAresta(1, 2) << endl;

    // Testes de grafo lista
    GrafoLista grafol1(false, false, 5);

    grafol1.insertAresta(1, 1);
    grafol1.insertAresta(2, 1);
    grafol1.insertAresta(3, 1);

    grafol1.removeAresta(3, 1);

    cout << endl;
    cout << "Grafo lista funcoes :" << endl;
    cout << "1 , 1" << endl;
    cout << grafol1.existeAresta(1, 1) << endl;
    cout << "3, 1" << endl;
    cout << grafol1.existeAresta(3, 1) << endl;
    cout << "1, 2" << endl;
    cout << grafol1.existeAresta(1, 2) << endl;


    /*insertNode(&list, 3, 10);
    insertNode(&list, 5, 20);
    insertNode(&list, 4, 30);
    insertNode(&list, 6, 40);
    insertNode(&list, 1, 23);
    insertNode(&list, 10, 2);

    showLDE(list);

    removeNode(&list, 4);
    removeNode(&list, 6);

    cout << endl;

    showLDE(list);*/

    deleteLDE(&list);

    return 0;
}