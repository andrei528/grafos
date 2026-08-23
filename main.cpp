#include <iostream>
#include "LDE.hpp"

using namespace std;

int main(){

    LDE <int> list;
    initializeLDE(list);

    insertNode(&list, 5, 20);
    insertNode(&list, 3, 20);
    showLDE(list);

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