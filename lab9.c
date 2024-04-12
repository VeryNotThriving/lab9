#include <stdio.h>
#include <stdlib.h> // for malloc and free

// Define HASH_SIZE as a constant variable
#define HASH_SIZE 100 // Choose an appropriate hash size

// RecordType
struct RecordType
{
    int id;
    char name;
    int order;
};

// HashType: A hash table to store records
struct HashType
{
    struct RecordType* record; // Pointer to the record
    int occupied;              // Flag to indicate if the entry is occupied
};


// Compute the hash function
int hash(int x)
{   
    return x % HASH_SIZE;
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

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
    int i;

    printf("\nRecords in Hash Table:\n");
    for (i = 0; i < hashSz; ++i)
    {
        if (pHashArray[i].occupied)
        {
            printf("Index %d -> %d, %c,Order-> %d\n", i, pHashArray[i].record->id, pHashArray[i].record->name, pHashArray[i].record->order);
        }
    }
    printf("\n");
}

// Main function
int main(void)
{
    struct RecordType *pRecords;
    int recordSz = 0;

    // Parse data and get record size
    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);

    // Initialize hash table
    struct HashType hashTable[HASH_SIZE];
    for (int i = 0; i < HASH_SIZE; ++i)
    {
        hashTable[i].occupied = 0; // Initialize all entries as unoccupied
    }

    // Insert records into the hash table

    // Insert records into the hash table
    for (int i = 0; i < recordSz; ++i)
    {
        int h = hash(pRecords[i].id); // Get hash value for the record
        // Find an empty slot
        while (hashTable[h].occupied)
        {
            h = (h + 1) % HASH_SIZE; // Linear probing
        }
        // Insert the record
        hashTable[h].record = &pRecords[i];
        hashTable[h].occupied = 1;
    }


    // Display records stored in the hash table
    displayRecordsInHash(hashTable, HASH_SIZE);

    // Free dynamically allocated memory
    free(pRecords);

    return 0;
}
