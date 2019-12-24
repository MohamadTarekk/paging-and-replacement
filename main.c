#include <stdio.h>
#include <stdlib.h>
#define SIZE = 999999999

/// Global Variables
int FRAMES_COUNT;
char POLICY[8]; //O P T I M A L \0
LinkedList* LIST:

/// Functions Prototypes
void mainmenu();
void readFile();

int main()
{
    mainmenu();
    return 0;
}

/// Double LinkedList Implementation
typedef struct Node
{
    int value;
    Node *next;
    Node *previous;
} Node;

typedef struct LinkedList
{
    Node *root;
    int capacity;
    int size;
    int empty;
    int full;
} LinkedList;

void initializeLinkedList(LinkedList *linkedList, int capacity)
{
    linkedList = (LinkedList*) malloc(sizeof(LinkedList));
    (*linkedList).root = NULL;
    (*linkedList).capacity = capacity;
    (*linkedList).size = 0;
    (*linkedList).empty = 1;
    (*linkedList).full = 0;
    return;
}

Node* addNode(LinkedList *linkedList, int value)
{
    if((*linkedList).full == 1)
        return NULL;
    Node *node = getNewNode(value);
    if((*linkedList).empty)
    {
        (*linkedList).root = node;
        (*linkedList).empty = 0;
    }
    else
    {
        Node *last = findEnd(linkedList);
        (*node).previous = last;
        (*last).next = node;
    }
    (*linkedList).size++;
    if((*linkedList).size == (*linkedList).capacity)
        (*linkedList).full = 1;
    return node;
}

int deleteNode(LinkedList *linkedList, int value)
{
    if((*linkedList).empty)
        return 0;
    Node *node = findNode(linkedList, value);
    if(node == NULL)
        return 0;
    Node *prev = (*node).previous;
    Node *next = (*node).next;
    if(prev != NULL)
        (*prev).next = next;
    if(next != NULL)
        (*next).previous = prev;
    free(node);
    (*linkedList).size--;
    if((*linkedList).size == 0)
        (*linkedList).empty = 1;
    return 1;
}

Node* findEnd(LinkedList *linkedList)
{
    Node *temp = ((*linkedList).root);
    if(temp == NULL)
        return NULL;
    while((*temp).next != NULL)
        temp = (*temp).next;
    return temp;
}

Node* findNode(LinkedList *linkedList, int value)
{
    if((*linkedList).empty == 1)
        return NULL:
    Node *temp = (*linkedList).root;
    while(1)
    {
        if(temp == NULL)
            return NULL;
        if((*temp).value == value)
            return temp;
        temp = (*temp).next;
    }
}

Node* getNewNode(int value)
{
    Node *node = (Node*) malloc(sizeof(Node));
    (*node).value = value;
    (*node).next = NULL;
    (*node).previous = NULL;
    return node;
}
