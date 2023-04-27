/*  Project_Three_HashTables
    Roa Hanoun - 1190886 */

  /*  I'm sorry for the lack of comments for the code,
    but I did not have enough time to write it,
    for a personal reason and not because there is not enough time for the project. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

typedef struct Dictionary DICT;
typedef struct Hash_Table HT;

char HashFunction[2][15] = {"Quadratic","Linear"};
int HashNum = 0;

struct Dictionary
{
    char word[100];
    char meaning[100];
    int status;
};

struct Hash_Table
{
    DICT* records;
    int numOfRecords;
    int size;
};

int Hashing(HT, char[]);
int hashFunction(HT, char[]);
void readFile(HT *table, char[]);
void insert(HT*, DICT);
void rehash(HT*);
void deleteFromTable(HT, char[]);
int findRecord(HT, char[]);
void printTable(HT);
void printHashFunction(HT);
void printTableSize(HT);
int requiredTableSize(int);
int isPrime(int);
int getNumOfRecords(HT*);
void updateNumOfRecords(HT*);

int main()
{
    printf("Choose hash function to be used:\n1- Quadratic hashing/t2- Linear proping\n");
    scanf("%d", &HashNum);
    if(HashNum < 1 || HashNum > 2)
        printf("You need to choose a valid hash function\n");
    HashNum--;
    HT table = {NULL,0,1};
    table.records = (DICT*) calloc(1, sizeof(DICT));
    char option = '0';
    do
    {
        printf("\nChoose an option\n");
        printf("1- Read file.\n");
        printf("2- Find a word.\n");
        printf("3- Update the meaning of an existing word.\n");
        printf("4- Insert a new word with its meaning.\n");
        printf("5- Print the entire hash table including the empty slots.\n");
        printf("6- Print all words in the dictionary with their associated meanings.\n");
        printf("7- Delete a word from the dictionary.\n");
        printf("8- Print the number of occupied slots, hash table size and the load factor.\n");
        printf("9- save all files back in file \"New_Dictionary.txt\".\n");
        printf("0- Exit.\n");
        char word[100];
        char meaning[100];
        option = getch();

        switch(option)
        {
        case '1':

            readFile(&table, "Dictionary.txt");

            break;
        case '2':
            fflush(stdin);
            printf("Enter the word you looking for: ");
            gets(word);
            fflush(stdin);
            int index = findRecord(table, word);
            if(index != -1)
            {
                printf("We found this record :) \n %d- %s : %s\n", index, word, table.records[index].meaning);
            }
            else
                printf("Sorry this word does not exist\n");
            break;
        case '3':

            fflush(stdin);
            printf("Enter word you want to update: ");
            gets(word);
            fflush(stdin);
            index = findRecord(table, word);
            if(index != -1)
            {
                printf("Please enter a new Meaning for the word: ");
                fflush(stdin);
                gets(meaning);
                fflush(stdin);
                strcpy(table.records[index].meaning, meaning);
                printf("Updating has been done successfully\n");
            }
            else
            {
                printf("Sorry you can't perform this operation since, Record already exists\n");
            }
            break;
        case '4':
            printf("Enter the word you need to insert it: ");
            fflush(stdin);
            gets(word);
            fflush(stdin);
            if(findRecord(table, word) != -1)
            {
                printf("Record already exists, you can't insert it again \n");
            }
            else
            {
                printf("Enter the meaning: ");
                fflush(stdin);
                gets(meaning);
                fflush(stdin);
                DICT record;
                strcpy(record.word, word);
                strcpy(record.meaning, meaning);
                insert(&table, record);
                printf("Inserted a new record successfully\n");
            }
            break;
        case '5':

            printTable(table);
            break;
        case '6':
            for(int i = 0; i < table.size; i++)
            {
                if(table.records[i].status == 1)
                    printf("%s: %s\n", table.records[i].word, table.records[i].meaning);
            }
            printf("\n");
            break;
        case '7':
            printf("Enter the word you need to deleted: ");
            fflush(stdin);
            gets(word);
            fflush(stdin);
            if(findRecord(table, word) == -1)
            {
                printf("Sorry this word does not exist\n");
            }
            else
            {
                deleteFromTable(table, word);
                printf("\n");
            }
            break;
        case '8':

            printf("Number of occupied slots is: %d\n", getNumOfRecords(&table));
            printf("Table size is: %d\n", table.size);
            printf("Load factor is: %f%% \n", (((double)getNumOfRecords(&table)/(double)table.size)));
            break;
        case '9':
            system("cls");
            FILE *out = fopen("New_Dictionary.txt", "w");
            for(int i = 0; i < table.size; i++)
            {
                if(table.records[i].status == 1)
                {
                    fprintf(out, "%s: %s\n", table.records[i].word, table.records[i].meaning);
                }
            }
            printf("Done :) you can see the table in file New_Dictionary.txt\n");
            fclose(out);
            break;
        }
    }
    while(option != '0');

    return 0;
}

void readFile(HT* table, char file[])
{

    FILE *in = fopen(file, "r");
    if(in == NULL)
    {
        printf("Error with open file\n");
    }
    char line[200];
    while(fgets(line, 200, in))
    {
        char *word = strtok(line, ":");
        char *mean = strtok(NULL, "\n");
        DICT record;
        strcpy(record.word, word);
        strcpy(record.meaning, mean);
        insert(table, record);
    }
    fclose(in);
    printf("Entry completed successfully");
}


void insert(HT *table, DICT record)
{
    (*table).numOfRecords++;
    if(((double)((*table).numOfRecords/(*table).size)) >= 0.6 )
        rehash(table);
    record.status = 1;
    int index = Hashing(*table, record.word);

    (*table).records[index] = record;
}

int hashFunction(HT table, char word[100])
{
    return strlen(word) % table.size;
}
// to return a ascii value for a word to use it to find a index for its word
int asciiValue(char word[100])
{
    int len = strlen(word);
    int ascii = 0;
    for(int i = 0; i < len ; i++)
    {
        ascii += word[i];
    }
    return ascii;
}

int Hashing(HT table, char word[100])
{
    if(strcmp(HashFunction[HashNum], "Quadratic") == 0)
    {
        int i = 1;
        int index = hashFunction(table, word);
        int quadIndex = index;
        while(table.records[quadIndex].status == 1)
        {
            quadIndex = (index + i*i) % table.size;
            i++;
        }
        return quadIndex;
    }

    if(strcmp(HashFunction[HashNum], "Linear") == 0)
    {
        int i = 0;
        int index = asciiValue(word) % table.size;
        int linearIndex = index;
        while(table.records[linearIndex].status == 1)
        {
            linearIndex = (index + i) % table.size;
            i++;
        }
        return linearIndex;

    }
    return -1;
}

void rehash(HT *table)
{
    int oldTableSize = (*table).size;
    int newTableSize = requiredTableSize(oldTableSize);
    (*table).size = newTableSize;

    DICT *temp = (*table).records;
    (*table).records = (DICT *) calloc(newTableSize, sizeof(DICT) );

    int i;

    for (i = 0 ; i < oldTableSize ; i++)
        if( temp[i].status == 1)
            insert(table, temp[i]);

    free(temp);
}


void deleteFromTable(HT table, char word[100])
{

    int index = findRecord(table, word);

    if (index != -1)
    {
        table.records[index].status = 2;
        printf("The word has been removed\n");
    }
    else
    {
        printf("Sorry this word does not exist!\n");
    }
}

int findRecord(HT table, char word[100])
{

    if(table.size == 0)
    {
        return -1;
    }
    if(strcmp(HashFunction[HashNum], "Quadratic") == 0)
    {
        int i = 1;

        int index = hashFunction(table, word);
        int quadIndex = index;

        while(table.records[quadIndex].status != 0)
        {
            if (table.records[quadIndex].status != 2 && strcmp(table.records[quadIndex].word,word) == 0)
                return quadIndex;
            else
            {
                quadIndex = (index + i*i) % table.size;
                i++;
            }
        }
    }
    if(strcmp(HashFunction[HashNum], "Linear") == 0)
    {
        int i = 0;
        int index = asciiValue(word);
        int linearIndex = index % table.size;
        while(table.records[linearIndex].status != 0)
        {
            if(table.records[linearIndex].status != 2 && strcmp(table.records[linearIndex].word, word) == 0)
                return linearIndex;
            else
            {
                linearIndex = (index + i) % table.size;
                i++;
            }
        }
    }
    return -1;
}

void printTable(HT table)
{
    int i;
    for(i = 0 ; i < table.size ; i++)
    {

        if (table.records[i].status != 0)
            printf("%d: %s: %s\n", i, table.records[i].word, table.records[i].meaning);
        else
            printf("[%d] -\n", i);
    }
}

int getNumOfRecords(HT *table)
{
    int records = 0;
    for(int i = 0; i < (*table).size; i++)
    {
        if((*table).records[i].status == 1)\
            records++;
    }
    return records;
}

void updateNumOfRecords(HT *table)
{
    (*table).numOfRecords = 0;

    for(int i = 0; i < (*table).size; i++)
    {
        if((*table).records[i].status == 1)
        {
            (*table).numOfRecords ++;
        }
    }
}

void printTableSize(HT table)
{
    printf("table size is %d\n",table.size);
}

int requiredTableSize(int n)
{
    int requiredTableSize = 2*n + 1;
    while(!isPrime(requiredTableSize))
        requiredTableSize++;

    return requiredTableSize;
}

int isPrime(int num)
{
    int i;
    for(i = 2 ; i <= num/2 ; i++)
        if (num % i == 0)
            return 0;
    return 1;
}


