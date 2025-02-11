#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 9999999

/// Structs
typedef struct Node
{
    int value;
    int use;
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
LinkedList *LIST;

/// Functions Prototypes
void mainmenu();
void readInput();
void startPaging();
void FIFO();
void LRU();
void CLOCK();
void OPTIMAL();

/// Helping Functions
void strRead(char *buf, int length);
void strPrint(char *buf);

/// Double LinkedList Implementation
LinkedList* initializeLinkedList(int capacity);
Node* addNode(LinkedList *linkedList, int value);
int deleteNode(LinkedList *linkedList, int value);
int removeNode(LinkedList *linkedList, Node *node);
Node* findEnd(LinkedList *linkedList);
Node* findNode(LinkedList *linkedList, int value);
void replaceNode(LinkedList *linkedList, Node *oldNode, Node *newNode);
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
    LIST = initializeLinkedList(SIZE);
    readInput();
    startPaging();
    return;
}

void readInput()
{
    scanf("%d", &FRAMES_COUNT);
    POLICY = (char*) malloc(8 * sizeof(char));
    strRead(POLICY, 8);
    int buffer;
    scanf("%d", &buffer);
    while(buffer != -1)
    {
        addNode(LIST, buffer);
        scanf("%d", &buffer);
    }
    return;
}

void startPaging()
{
    if(!strcmp(POLICY, "FIFO"))
        FIFO();
    else if(!strcmp(POLICY, "LRU"))
        LRU();
    else if(!strcmp(POLICY, "CLOCK"))
        CLOCK();
    else if(!strcmp(POLICY, "OPTIMAL"))
        OPTIMAL();
    else
        printf("Unsupported Algorithm!");

    return;
}

void FIFO()
{
    // initialize frames list
    printf("Replacement Policy = FIFO\n-------------------------------------\nPage   Content of Frames\n----   -----------------\n");
    LinkedList *frames = initializeLinkedList(FRAMES_COUNT);
    // add first page request
    Node *request = LIST->root;
    addNode(frames, request->value);
    // print page
    char page[10];
    sprintf(page, "%02d", request->value);
    printf("%s     ", page);
    printList(frames);
    // initialize oldest
    Node *oldest = frames->root;
    request = request->next;
    // receive page requests
    int faults = 0;
    while(request != NULL)
    {
        if(frames->full == 1)
        {
            Node *node = findNode(frames, request->value);
            if(node == NULL)
            {
                oldest->value = request->value;
                if(oldest->next == NULL)
                {
                    oldest = frames->root;
                }
                else
                {
                    oldest = oldest->next;
                }
                sprintf(page, "%02d F   ", request->value);
                faults++;
            }
            else
            {
                sprintf(page, "%02d     ", request->value);
            }
        }
        else
        {
            Node *node = findNode(frames, request->value);
            if(node == NULL)
            {
                addNode(frames, request->value);
            }
            sprintf(page, "%02d     ", request->value);
        }
        printf("%s", page);
        printList(frames);
        request = request->next;
    }
    printf("-------------------------------------\n");
    printf("Number of page faults = %d\n", faults);
    return;
}

void LRU()
{
    // initialize frames and lru
    printf("Replacement Policy = LRU\n-------------------------------------\nPage   Content of Frames\n----   -----------------\n");
    LinkedList *frames = initializeLinkedList(FRAMES_COUNT);
    // add first page request
    Node *request = LIST->root;
    addNode(frames, request->value);
    // print page
    char page[10];
    sprintf(page, "%02d", request->value);
    printf("%s     ", page);
    printList(frames);
    request = request->next;
    // receive page requests
    int faults = 0;
    while(request != NULL)
    {
        if(frames->full == 1)
        {
            Node *node = findNode(frames, request->value);
            if(node == NULL)
            {
                LinkedList *lru = initializeLinkedList(FRAMES_COUNT);
                Node *node = request->previous;
                addNode(lru, node->value);
                node = node->previous;
                while(lru->full == 0 && (node != NULL))
                {
                    if(findNode(lru, node->value) == NULL)
                    {
                        addNode(lru, node->value);
                    }
                    node = node->previous;
                }
                int target = findEnd(lru)->value;
                findNode(frames, target)->value = request->value;
                free(lru);
                sprintf(page, "%02d F   ", request->value);
                faults++;
            }
            else
            {
                sprintf(page, "%02d     ", request->value);
            }
        }
        else
        {
            Node *node = findNode(frames, request->value);
            if(node == NULL)
            {
                addNode(frames, request->value);
            }
            sprintf(page, "%02d     ", request->value);
        }
        printf("%s", page);
        printList(frames);
        request = request->next;
    }
    printf("-------------------------------------\n");
    printf("Number of page faults = %d\n", faults);
    return;
}

void CLOCK()
{
    // initialize frames and lru
    printf("Replacement Policy = CLOCK\n-------------------------------------\nPage   Content of Frames\n----   -----------------\n");
    LinkedList *frames = initializeLinkedList(FRAMES_COUNT);
    Node *nil;
    while(frames->full == 0)
    {
        nil = addNode(frames, 0);
        nil->use = -1;  // mark as empty
    }
    // make the list circular
    nil->next = frames->root;
    frames->root->previous = nil;
    // initialize clock pointer
    Node *clockPtr = frames->root;
    // add first page request
    Node *request = LIST->root;
    clockPtr->value = request->value;
    clockPtr->use = 1;
    clockPtr = clockPtr->next;
    // print page
    char page[10];
    sprintf(page, "%02d", request->value);
    printf("%s     ", page);
    printList(frames);
    request = request->next;
    // receive page requests
    int faults = 0;
    while(request != NULL)
    {
        Node *target = findNode(frames, request->value);
        if(target == NULL)
        {
            if(clockPtr->use == -1)
            {
                clockPtr->value = request->value;
                clockPtr->use = 1;
                clockPtr = clockPtr->next;
                sprintf(page, "%02d     ", request->value);
            }
            else    // frames array is full
            {
                while((clockPtr->use != 0) && (clockPtr->value != request->value))
                {
                    clockPtr->use = 0;
                    clockPtr = clockPtr->next;
                }
                clockPtr->value = request->value;
                clockPtr->use = 1;
                clockPtr = clockPtr->next;
                sprintf(page, "%02d F   ", request->value);
                faults++;
            }
        }
        else
        {
            target->use = 1;
            sprintf(page, "%02d     ", request->value);
        }
        printf("%s", page);
        printList(frames);
        request = request->next;
    }
    printf("-------------------------------------\n");
    printf("Number of page faults = %d\n", faults);
    return;
}

void OPTIMAL()
{
    // initialize frames and lru
    printf("Replacement Policy = OPTIMAL\n-------------------------------------\nPage   Content of Frames\n----   -----------------\n");
    LinkedList *frames = initializeLinkedList(FRAMES_COUNT);
    // add first page request
    Node *request = LIST->root;
    addNode(frames, request->value);
    // print page
    char page[10];
    sprintf(page, "%02d", request->value);
    printf("%s     ", page);
    printList(frames);
    request = request->next;
    // receive page requests
    int faults = 0;
    while(request != NULL)
    {
        if(frames->full == 1)
        {
            Node *node = findNode(frames, request->value);
            if(node == NULL)
            {
                LinkedList *upcoming = initializeLinkedList(FRAMES_COUNT);
                Node *next = request->next;
                while(upcoming->full == 0 && (next != NULL))
                {
                    if((findNode(frames, next->value) != NULL) && (findNode(upcoming, next->value) == NULL))
                    {
                        addNode(upcoming, next->value);
                    }
                    next = next->next;
                }
                if(frames->size != upcoming->size)
                {
                    Node *a = frames->root;
                    while(a != NULL)
                    {
                        int found = 0;
                        Node *b = upcoming->root;
                        while(b != NULL)
                        {
                            if(a->value == b->value)
                            {
                                found = 1;
                                break;
                            }
                            b = b->next;
                        }
                        if(!found)
                            break;
                        a = a->next;
                    }
                    a->value = request->value;
                }
                else
                {
                    int target = findEnd(upcoming)->value;
                    findNode(frames, target)->value = request->value;
                }
                free(upcoming);
                sprintf(page, "%02d F   ", request->value);
                faults++;
            }
            else
            {
                sprintf(page, "%02d     ", request->value);
            }
        }
        else
        {
            Node *node = findNode(frames, request->value);
            if(node == NULL)
            {
                addNode(frames, request->value);
            }
            sprintf(page, "%02d     ", request->value);
        }
        printf("%s", page);
        printList(frames);
        request = request->next;
    }
    printf("-------------------------------------\n");
    printf("Number of page faults = %d\n", faults);
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
LinkedList* initializeLinkedList(int capacity)
{
    LinkedList *linkedList = (LinkedList*) malloc(sizeof(LinkedList));
    linkedList->root = NULL;
    linkedList->capacity = capacity;
    linkedList->size = 0;
    linkedList->empty = 1;
    linkedList->full = 0;
    return linkedList;
}

Node* addNode(LinkedList *linkedList, int value)
{
    if(linkedList->full == 1)
    {
        return NULL;
    }
    Node *node = getNewNode(value);
    if(linkedList->empty == 1)
    {
        linkedList->root = node;
        linkedList->empty = 0;
    }
    else
    {
        Node *last = findEnd(linkedList);
        if(last == NULL)
        {
            linkedList->root = node;
        }
        else
        {
            node->previous = last;
            last->next = node;
        }
    }
    linkedList->size++;
    if(linkedList->size == linkedList->capacity)
        linkedList->full = 1;
    return node;
}

int deleteNode(LinkedList *linkedList, int value)
{
    if(linkedList->empty)
        return 0;
    Node *node = findNode(linkedList, value);
    return removeNode(linkedList, node);
}

int removeNode(LinkedList *linkedList, Node *node)
{
    if(node == NULL)
        return 0;
    Node *prev = node->previous;
    Node *next = node->next;
    if(prev != NULL)
        prev->next = next;
    if(next != NULL)
        next->previous = prev;
    free(node);
    linkedList->size--;
    if(linkedList->size == 0)
        linkedList->empty = 1;
    return 1;
}

Node* findEnd(LinkedList *linkedList)
{
    Node *temp = linkedList->root;
    int index = 0;
    if(temp == NULL)
        return NULL;
    while((temp->next != NULL) && (index < linkedList->size))
    {
        temp = temp->next;
        index++;
    }
    return temp;
}

Node* findNode(LinkedList *linkedList, int value)
{
    if(linkedList->empty == 1)
        return NULL;
    Node *temp = linkedList->root;
    int index = 0;
    while(index < linkedList->size)
    {
        if(temp == NULL)
            return NULL;
        if(temp->value == value)
            return temp;
        temp = temp->next;
        index++;
    }
    return NULL;
}

void replaceNode(LinkedList *linkedList, Node *oldNode, Node *newNode)
{
    // link node with previous
    Node *prev = oldNode->previous;
    prev->next = newNode;
    newNode->previous = prev;
    // link node with next
    Node *next = oldNode->next;
    next->previous = newNode;
    newNode->next = next;
    // delete old node
    free(oldNode);
    return;
}

Node* getNewNode(int value)
{
    Node *node = (Node*) malloc(sizeof(Node));
    node->value = value;
    node->use = 1;
    node->next = NULL;
    node->previous = NULL;
    return node;
}

void printList(LinkedList *linkedList)
{
    Node *temp = linkedList->root;
    int index = 0;
    while((temp != NULL) && (index < linkedList->size))
    {
        if(temp->use != -1)
            printf("%02d ", temp->value);
        temp = temp->next;
        index++;
    }
    printf("\n");
    return;
}
