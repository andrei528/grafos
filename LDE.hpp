#include <iostream>

using namespace std;

template <typename T>
struct Node
{
    int id;
    int weight;
    Node<T> *prox = NULL;
    Node<T> *prev = NULL;
};

template <typename T>
struct LDE
{
    Node<T> *head = NULL;
    Node<T> *tail = NULL;
};

template <typename T>
void initializeLDE(LDE<T> &list)
{
    list.head = NULL;
    list.tail = NULL;
}

// Inseri sempre no final
template <typename T>
bool insertFinal(LDE<T> *list, int id, int weight)
{
    Node<T> *new_node = new Node<T>;
    new_node->id = id;
    new_node->weight = weight;
    new_node->prox = NULL;
    new_node->prev = NULL;

    if (list->tail == NULL)
    {
        list->head = new_node;
        list->tail = new_node;
    }
    else
    {
        list->tail->prox = new_node;
        new_node->prev = list->tail;
        list->tail = new_node;
    }

    return true;
}

// Sempre de forma ordenada pelo id
template <typename T>
bool insertNode(LDE<T> *list, int id, int weight)
{
    Node<T> *new_node = new Node<T>;
    new_node->id = id;
    new_node->weight = weight;
    new_node->prox = NULL;
    new_node->prev = NULL;

    Node<T> *aux;
    Node<T> *aux2;

    if (list->tail == NULL)
    {
        list->head = new_node;
        list->tail = new_node;
    }
    // Inserir no inicio
    else if (id < list->head->id)
    {
        aux = list->head;
        new_node->prox = list->head;
        list->head = new_node;
        aux->prev = list->head;
    }
    // Inserir no final
    else if (id > list->tail->id)
    {
        aux = list->tail;
        new_node->prev = list->tail;
        list->tail = new_node;
        aux->prox = list->tail;
    }
    // Inserir no meio
    else
    {
        aux = list->head;
        while (aux != NULL)
        {
            if (aux->prox == NULL)
            {
                break;
            }

            if (id > aux->id && id < aux->prox->id)
            {
                aux2 = aux->prox;

                new_node->prox = aux2;
                new_node->prev = aux;
                aux->prox = new_node;
                aux2->prev = new_node;
            }
            aux = aux->prox;
        }
    }

    return true;
}

// Remover node pelo id
template <typename T>
bool removeNode(LDE<T> *list, int id)
{
    Node<T> *aux, *aux2, *aux3;

    if (list->head == NULL)
    {
        return false;
    }

    // Caso seja o primeiro valor
    if (id == list->head->id)
    {
        aux = list->head;
        // Caso só exista esse valor na lista
        if (aux->prox == NULL)
        {
            list->head = NULL;
            list->tail = NULL;
            delete aux;
        }
        // Caso tenha mais valores na lista
        else
        {
            aux2 = aux->prox;
            aux->prox = NULL;
            aux2->prev = NULL;
            list->head = aux2;
            delete aux;
        }
    }

    // Caso seja o ultimo valor
    else if (id == list->tail->id)
    {
        aux = list->tail;
        aux2 = aux->prev;

        aux->prev = NULL;
        aux2->prox = NULL;

        list->tail = aux2;
        delete aux;
    }
    else
    {
        aux = list->head;
        // Remover do meio
        while (aux != NULL)
        {
            if (aux->id == id)
            {
                aux2 = aux->prev;
                aux3 = aux->prox;
                aux2->prox = aux3;
                aux3->prev = aux2;

                aux->prox = NULL;
                aux->prev = NULL;

                delete aux;
                break;
            }

            aux = aux->prox;
        }
    }

    return true;
}

// Mostrar no terminal
template <typename T>
void showLDE(LDE<T> lista)
{
    Node<T> *aux = lista.head;
    while (aux != NULL)
    {
        cout << aux->id << " ";
        aux = aux->prox;
    }
}

// Liberar LDE
template <typename T>
void deleteLDE(LDE<T> *list)
{

    Node<T> *temp, *aux = list->head;

    while (aux != NULL)
    {
        temp = aux;
        aux = aux->prox;
        delete temp;
    }
    list->head = list->tail = NULL;
}

// ---- Lista de vertices: cada vertice guarda sua propria lista de arestas ----

struct NodeVertice
{
    int id;
    LDE<int> listaAdjacencia;
    NodeVertice *prox = NULL;
    NodeVertice *prev = NULL;
};

struct LDEVertices
{
    NodeVertice *head = NULL;
    NodeVertice *tail = NULL;
};

void initializeLDEVertices(LDEVertices &list)
{
    list.head = NULL;
    list.tail = NULL;
}

// Inserir vertice
void insertVertice(LDEVertices *list, int id)
{
    NodeVertice *new_node = new NodeVertice;
    new_node->id = id;
    initializeLDE(new_node->listaAdjacencia);
    new_node->prox = NULL;
    new_node->prev = NULL;

    if (list->tail == NULL)
    {
        list->head = new_node;
        list->tail = new_node;
    }
    else
    {
        list->tail->prox = new_node;
        new_node->prev = list->tail;
        list->tail = new_node;
    }
}

// Encontrar vertice
NodeVertice *findVertice(LDEVertices *list, int id)
{
    NodeVertice *aux = list->head;
    while (aux != NULL)
    {
        if (aux->id == id)
        {
            return aux;
        }
        aux = aux->prox;
    }
    return NULL;
}

// Remover vertice
bool removerVertice(LDEVertices *list, int id)
{
    NodeVertice *aux = list->head;
    while (aux != NULL)
    {
        if (aux->id == id)
        {
            if (aux->prev != NULL)
            {
                aux->prev->prox = aux->prox;
            }
            else
            {
                list->head = aux->prox;
            }
            if (aux->prox != NULL)
            {
                aux->prox->prev = aux->prev;
            }
            else
            {
                list->tail = aux->prev;
            }
            deleteLDE(&aux->listaAdjacencia);
            delete aux;
            return true;
        }
        aux = aux->prox;
    }
    return false;
}

// Deletar lista de vertices
void deleteVertices(LDEVertices *list)
{
    NodeVertice *aux = list->head;
    while (aux != NULL)
    {
        NodeVertice *temp = aux;
        aux = aux->prox;
        deleteLDE(&temp->listaAdjacencia);
        delete temp;
    }
    list->head = NULL;
    list->tail = NULL;
}