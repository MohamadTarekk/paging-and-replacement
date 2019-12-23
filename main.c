#include <stdio.h>
#include <stdlib.h>


/// Global Variables
int FRAMES_COUNT;
char POLICY[8]; //O P T I M A L \0
LinkedList* LIST:

/// Functions Prototypes
readFile();

int main()
{
    char s[8];

    printf("Hello world!\n");
    return 0;
}

/// IndexedList Implementation
typedef struct Node
{
    int value;
    Node *next;
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
    Node *node = (*linkedList).root;
    if((*node).value == value)
    {
        Node *temp = node;
        node = (*node).next;
        free(temp);
        return 1;
    }
    Node *prev = node;
    node = (*node).next;
    while(1)
    {
        if(node == NULL)
            return 0;
        if((*node).value == value)
        {
            Node *temp = node;
            node = (*node).next;
            (*prev).next = node;
            free(temp);
            return 1;
        }
        else
        {
            prev = node;
            node = (*node).next;
        }
    }
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
        if(*temp == NULL)
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
    return node;
}
