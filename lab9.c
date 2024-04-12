/*Lindsey Duong
4/11/24
Lab Assignment 9*/

#include <stdio.h>
#include <stdlib.h>

#define HASH_SIZE 23

// RecordType
struct RecordType
{
    int     id;
    char    name;
    int     order; 
};

// Node for chaining
struct Node {
    struct RecordType data;
    struct Node *next;
};

// HashType structure
struct HashType
{
    struct Node *head; // Head of linked list for chaining
};

// Compute the hash function
int hash(int x)
{
    // A simple hash function to distribute keys evenly
    // Adjust as needed for better distribution
    return x % HASH_SIZE; 
}

// Function to insert a record into the hash table
void insertRecord(struct HashType *hashArray, struct RecordType record)
{
    int index = hash(record.id);
    struct Node *newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = record;
    newNode->next = NULL;

    // If no record exists at the index
    if (hashArray[index].head == NULL) {
        hashArray[index].head = newNode;
    } 
    // If there are records, append the new record to the end of the chain
    else {
        struct Node *temp = hashArray[index].head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

// Display function
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

// Function to parse input data
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
        // Implement parse data block
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

// prints the records
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

    // Create an array of HashType
    struct HashType *hashArray = (struct HashType*)malloc(sizeof(struct HashType) * HASH_SIZE);
    for (int i = 0; i < HASH_SIZE; i++) {
        hashArray[i].head = NULL; // Initialize each head pointer to NULL
    }

    // Insert records into hash table
    for (int i = 0; i < recordSz; i++) {
        int index = hash(pRecords[i].id);
        struct Node *newNode = (struct Node*)malloc(sizeof(struct Node));
        newNode->data = pRecords[i]; // Copy the record
        newNode->next = hashArray[index].head; // Insert at the beginning
        hashArray[index].head = newNode;
    }

    // Display records in hash structure
    displayRecordsInHash(hashArray, HASH_SIZE);

    // Free dynamically allocated memory
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
