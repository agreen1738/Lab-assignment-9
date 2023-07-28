#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
    int     id;
    char    name;
    int     order; 
};

// Node for linked list
struct Node
{
    struct RecordType record;
    struct Node* next;
};

// HashType (Hash Table)
struct HashType
{
    struct Node* head;
};

// Compute the hash function
int hash(int x, int hashSz)
{
    return x % hashSz; // Simple modulo hash function
}

// parses input file to an integer array
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

// Inserts a record into the hash table
void insertRecord(struct HashType* pHashArray, int hashSz, struct RecordType record)
{
    int index = hash(record.id, hashSz);

    // Create a new node for the record
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode == NULL)
    {
        printf("Memory allocation error\n");
        exit(-1);
    }
    newNode->record = record;
    newNode->next = NULL;

    // If the index is unoccupied, add the record as the first node
    if (pHashArray[index].head == NULL)
    {
        pHashArray[index].head = newNode;
    }
    else
    {
        // If the index is already occupied, add the record to the end of the linked list
        struct Node* current = pHashArray[index].head;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = newNode;
    }
}

// Display records in the hash structure
// Skip the indices which are free
// The output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
    int i;

    for (i = 0; i < hashSz; ++i)
    {
        if (pHashArray[i].head != NULL)
        {
            printf("index %d:\n", i);
            struct Node* current = pHashArray[i].head;
            while (current != NULL)
            {
                printf("\tID: %d\tName: %c \tOrder: %d\n", current->record.id, current->record.name, current->record.order);
                current = current->next;
            }
        }
    }
}

// Main function
int main(void)
{
    struct RecordType *pRecords;
    int recordSz = 0;
    int hashSz = 10; // The size of the hash table (you can change this according to your requirements)

    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);

    // Create the hash table (array of linked lists)
    struct HashType* pHashArray = (struct HashType*)malloc(sizeof(struct HashType) * hashSz);
    if (pHashArray == NULL)
    {
        printf("Memory allocation error\n");
        exit(-1);
    }

    // Initialize the hash table
    for (int i = 0; i < hashSz; ++i)
    {
        pHashArray[i].head = NULL;
    }

    // Insert records into the hash table
    for (int i = 0; i < recordSz; ++i)
    {
        insertRecord(pHashArray, hashSz, pRecords[i]);
    }

    // Display the records in the hash table
    displayRecordsInHash(pHashArray, hashSz);

    // Free allocated memory
    for (int i = 0; i < hashSz; ++i)
    {
        struct Node* current = pHashArray[i].head;
        while (current != NULL)
        {
            struct Node* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(pHashArray);
    free(pRecords);

    return 0;
}