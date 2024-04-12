/*Lindsey Duong
4/11/24
Lab Assignment 9*/

#include <stdio.h>
#include <stdlib.h>

//make hash table size 23 
#define HASH_SIZE 23

//struct to define each record 
struct RecordType
{
    int     id;
    char    name;
    int     order; 
};

//struct for node for chaining 
struct Node {
    struct RecordType data;
    struct Node *next;
};

//fill function! 
struct HashType
{
    struct Node *head; 
};

//fill fucntion! x is the key 
int hash(int x)
{
    return x % HASH_SIZE; 
}

//function used to insert records into the hash table 
void insertRecord(struct HashType *hashArray, struct RecordType record)
{
    int index = hash(record.id);
    struct Node *newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = record;
    newNode->next = NULL;

    if (hashArray[index].head == NULL) {
        hashArray[index].head = newNode;
    } 
    else {
        struct Node *temp = hashArray[index].head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

//function used to display records within hash table 
void displayRecordsInHash(struct HashType *hashArray, int hashSz)
{
    int i;

    for (i = 0; i < hashSz; ++i)
    {
        printf("Index %d -> ", i);
        struct Node *temp = hashArray[i].head;
        while (temp != NULL)
        {
            printf("(%d, %c, %d) -> ", temp->data.id, temp->data.name, temp->data.order);
            temp = temp->next;
        }
        printf("NULL\n");
    }
}

//function used to parse input data
int parseData(char* inputFileName, struct RecordType** ppData)
{
    FILE* inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
        if (*ppData == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i = 0; i < dataSz; ++i)
        {
            pRecord = *ppData + i;
            fscanf(inFile, "%d ", &n);
            pRecord->id = n;
            fscanf(inFile, "%c ", &c);
            pRecord->name = c;
            fscanf(inFile, "%d ", &n);
            pRecord->order = n;
        }

        fclose(inFile);
    }

    return dataSz;
}

//function used to print the records 
void printRecords(struct RecordType pData[], int dataSz)
{
    int i;
    printf("\nRecords:\n");
    for (i = 0; i < dataSz; ++i)
    {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}

int main(void)
{
    struct RecordType *pRecords;
    int recordSz = 0;

    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);

    struct HashType *hashArray = (struct HashType*)malloc(sizeof(struct HashType) * HASH_SIZE);
    for (int i = 0; i < HASH_SIZE; i++) {
        hashArray[i].head = NULL; 
    }

    for (int i = 0; i < recordSz; i++) {
        int index = hash(pRecords[i].id);
        struct Node *newNode = (struct Node*)malloc(sizeof(struct Node));
        newNode->data = pRecords[i]; 
        newNode->next = hashArray[index].head; 
        hashArray[index].head = newNode;
    }

    displayRecordsInHash(hashArray, HASH_SIZE);

    free(pRecords);
    for (int i = 0; i < HASH_SIZE; i++) {
        struct Node *temp = hashArray[i].head;
        while (temp != NULL) {
            struct Node *prev = temp;
            temp = temp->next;
            free(prev);
        }
    }
    free(hashArray);

    return 0;
}
