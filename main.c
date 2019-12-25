#include <stdio.h>
#include <stdlib.h>
#define SIZE 9999999

/// Structs
typedef struct Node
{
    int value;
    struct Node *next;
    struct Node *previous;
} Node;

typedef struct LinkedList
{
    Node *root;
    int capacity;
    int size;
    int empty;
    int full;
} LinkedList;

/// Global Variables
int FRAMES_COUNT;
char *POLICY; //O P T I M A L \0
LinkedList LIST;

/// Functions Prototypes
void mainmenu();
void readInput();

/// Helping Functions
void strRead(char *buf, int length);
void strPrint(char *buf);

/// Double LinkedList Implementation
void initializeLinkedList(LinkedList *linkedList, int capacity);
Node* addNode(LinkedList *linkedList, int value);
int deleteNode(LinkedList *linkedList, int value);
Node* findEnd(LinkedList *linkedList);
Node* findNode(LinkedList *linkedList, int value);
Node* getNewNode(int value);
void printList(LinkedList *linkedList);

int main()
{
    mainmenu();
    return 0;
}

/// Functions
void mainmenu()
{
    initializeLinkedList(&LIST, SIZE);
    readInput();
    //printList(LIST);
    return;
}

void readInput()
{
    scanf("%d", &FRAMES_COUNT);
    POLICY = (char*) malloc(8 * sizeof(char));
    strRead(POLICY, 8);
    strPrint(POLICY);
    int buffer;
    scanf("%d", &buffer);
    while(buffer != -1)
    {
        addNode(&LIST, buffer);
    //    printList(&LIST);
        scanf("%d", &buffer);
    }
    //printList(&LIST);
    return;
}

/// Helping Functions
void strRead(char *buf, int length)
{
    // Flush the stdin manually
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    // read the string
    fgets(buf, length, stdin);
    // Remove extra trailing spaces and newlines
    int i = 0;
    while(buf[i] != '\0') i++;
    while(buf[i-1] == '\n' || buf[i-1] == ' ')
    {
        buf[i-1] = '\0';
        i--;
    }
}

void strPrint(char *buf)
{
    int i=0;
    while(buf[i])
    {
        printf("%c", buf[i]);
        i++;
    }
    printf("\n");
    return;
}

/// Double LinkedList Implementation
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
    {
        return NULL;
    }
    Node *node = getNewNode(value);
    if((*linkedList).empty == 1)
    {
        (*linkedList).root = node;
        (*linkedList).empty = 0;
    }
    else
    {
        Node *last = findEnd(linkedList);
        if(last == NULL)
        {
            (*linkedList).root = node;
        }
        else
        {
            (*node).previous = last;
            (*last).next = node;
        }
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
        return NULL;
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

void printList(LinkedList *linkedList)
{
    Node *temp = (*linkedList).root;
    while(temp != NULL)
    {
        printf("%d\t", (*temp).value);
        temp = (*temp).next;
    }
    printf("\n");
    return;
}
