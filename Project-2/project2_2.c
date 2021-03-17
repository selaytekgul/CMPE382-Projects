// Now I am reading 200000 lines per file.
// When I tried there was a problem in the task 5 with 200000 lines
// To see all the tasks properly please go to line 25
// just change the defined NUMBEROFLINESREAD to 100000 or less


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define FILEOUTMODE "w+"
#define FILEINMODE "r"

#define PRINTFORMAT "%s\t%d\n"

#define FILENOTOPENMSG "We couldn't open file %s to read!"
#define FILEOPENMSG "We're reading file %s with input status %d.\n"

#define OUTPUTFILENAME "Dictionary.txt"
#define INPUTFILESNAME "input.txt"

#define NUMBER_OF_DATA_FILES (10)
#define NUMBER_OF_DATA_LINES (200000)
#define TOKENS_LIMIT (NUMBER_OF_DATA_LINES * NUMBER_OF_DATA_FILES)

#define ARRAY_SIZE(a) sizeof(a) / sizeof(a[0])

// Alphabet size (# of symbols)
#define ALPHABET_SIZE (91)

// Converts key current character into index
// use only 'a' through 'z' and lower case
#define CHAR_TO_INDEX(c) ((short)c - (short)' ')

struct TrieNode_t *ThreadedTrie;
struct TrieNode_t *ThreadedTries[NUMBER_OF_DATA_FILES];
struct TrieNode_t *transfered;
char **tokensGlobal;
size_t tokens_allocGlobal;
size_t tokens_usedGlobal;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// trie node
struct TrieNode_t
{
    struct TrieNode_t *children[ALPHABET_SIZE];
    // isEndOfWord is true if the node represents
    // end of a word
    bool isEndOfWord;
    short wordNumber;
};

struct TrieNode_t *getNodePrivate(void)
{
    struct TrieNode_t *pNode = NULL;

    pNode = (struct TrieNode_t *)malloc(sizeof(struct TrieNode_t));

    if (pNode)
    {
        short i;

        pNode->isEndOfWord = false;

        for (i = 0; i < ALPHABET_SIZE; i++)
            pNode->children[i] = NULL;
        pNode->wordNumber = 0;
    }

    return pNode;
}

// If not present, inserts key into trie
// If the key is prefix of trie node, just marks leaf node
void insert(struct TrieNode_t *root, const char *key)
{
    short level;
    short length = strlen(key);
    short index;
    struct TrieNode_t *pCrawl = root;
    for (level = 0; level < length; level++)
    {
        index = CHAR_TO_INDEX(key[level]);
        if (index < 0 || index > ' ' + ALPHABET_SIZE)
        {
            continue;
        }
        if (!pCrawl->children[index])
            pCrawl->children[index] = getNodePrivate();

        pCrawl = pCrawl->children[index];
    }
    // mark last node as leaf
    pCrawl->isEndOfWord = true;
    pCrawl->wordNumber++;
}

// Returns true if key presents in trie, else false
bool search(struct TrieNode_t *root, const char *key)
{
    short level;
    short length = strlen(key);
    short index;
    struct TrieNode_t *pCrawl = root;

    for (level = 0; level < length; level++)
    {
        index = CHAR_TO_INDEX(key[level]);

        if (!pCrawl->children[index])
            return false;

        pCrawl = pCrawl->children[index];
    }

    return (pCrawl != NULL && pCrawl->isEndOfWord);
}

void trieToOutputPrivate(char *prefix, struct TrieNode_t *node)
{
    if (node->isEndOfWord)
        printf(PRINTFORMAT, prefix, node->wordNumber);
    for (char index = 0; index < ALPHABET_SIZE; ++index)
    {
        char next = ' ' + index;
        struct TrieNode_t *pChild = node->children[index];
        if (pChild)
        {
            prefix[strlen(prefix)] = next;
            prefix[strlen(prefix) + 1] = '\0';
            trieToOutputPrivate(prefix, pChild);
            prefix[strlen(prefix) - 1] = '\0';
        }
    }
}

void showDictionary(struct TrieNode_t *dictionary)
{
    char *str = (char *)malloc(1000);
    trieToOutputPrivate(str, dictionary);
}

void trieToTxtPrivate(char *prefix, struct TrieNode_t *node, FILE *file)
{
    if (node->isEndOfWord)
    {
        fprintf(file, PRINTFORMAT, prefix, node->wordNumber);
    }
    for (char index = 0; index < ALPHABET_SIZE; ++index)
    {
        char next = ' ' + index;
        struct TrieNode_t *pChild = node->children[index];
        if (pChild)
        {
            prefix[strlen(prefix)] = next;
            prefix[strlen(prefix) + 1] = '\0';
            trieToTxtPrivate(prefix, pChild, file);
            prefix[strlen(prefix) - 1] = '\0';
        }
    }
}

struct TrieNode_t *trieToTransferPrivate(char *prefix, struct TrieNode_t *node, FILE *file)
{

    if (node->isEndOfWord)
    {
        // printf("Prefix: %s\t##%d\n", prefix, node->wordNumber);
        for (size_t i = 0; i < node->wordNumber; i++)
        {
            insert(transfered, prefix);
        }
    }
    for (char index = 0; index < ALPHABET_SIZE; ++index)
    {
        char next = ' ' + index;
        struct TrieNode_t *pChild = node->children[index];
        if (pChild)
        {
            prefix[strlen(prefix)] = next;
            prefix[strlen(prefix) + 1] = '\0';
            trieToTransferPrivate(prefix, pChild, file);
            prefix[strlen(prefix) - 1] = '\0';
        }
    }
    return transfered;
}

struct TrieNode_t *createDictionaryTransfered(struct TrieNode_t *dictionary)
{
    char *str = (char *)malloc(1000);
    FILE *file = fopen(OUTPUTFILENAME, FILEOUTMODE);
    // transfered = getNodePrivate();

    return trieToTransferPrivate(str, dictionary, file);
}

void createDictionaryFile(struct TrieNode_t *dictionary)
{
    char *str = (char *)malloc(1000);
    FILE *file = fopen(OUTPUTFILENAME, FILEOUTMODE);

    trieToTxtPrivate(str, dictionary, file);
}

void task2()
{
    FILE *inputFile;

    char *line = NULL;
    size_t len = 0;
    size_t nread;

    struct TrieNode_t *trieRoot = getNodePrivate();

    int inputStatus = 0;
    char inputFileName[] = "------------------";
    FILE *input = fopen(INPUTFILESNAME, "r");

    if (input == NULL)
    {
        printf(FILENOTOPENMSG, INPUTFILESNAME);
        exit;
    }

    short numberOfDataFilesRead = 0;
    do
    {
        inputStatus = fscanf(input, "%s", &inputFileName);
        inputFile = fopen(inputFileName, FILEINMODE);
        if (inputFile == NULL)
        {
            printf(FILENOTOPENMSG, inputFileName);
            exit;
        }
        printf(FILEOPENMSG, inputFileName, inputStatus);

        long linesRead = 0;
        while ((nread = getline(&line, &len, inputFile) != -1) && linesRead < NUMBER_OF_DATA_LINES)
        {
            line = strsep(&line, "\n");
            insert(trieRoot, line);
            linesRead++;
        }

        numberOfDataFilesRead++;

    } while (inputStatus == 1 && numberOfDataFilesRead < NUMBER_OF_DATA_FILES);

    createDictionaryFile(trieRoot);
}

void task3()
{
    FILE *inputFile;

    char *line = NULL;
    size_t len = 0;
    size_t nread;

    struct TrieNode_t *trieRoot = getNodePrivate();

    int inputStatus = 0;
    char inputFileName[] = "------------------";
    FILE *input = fopen(INPUTFILESNAME, "r");

    if (input == NULL)
    {
        printf(FILENOTOPENMSG, INPUTFILESNAME);
        exit;
    }

    // char *token;
    size_t tokens_limit = 100;
    short numberOfDataFilesRead = 0;
    do
    {
        inputStatus = fscanf(input, "%s", &inputFileName);
        inputFile = fopen(inputFileName, FILEINMODE);
        if (inputFile == NULL)
        {
            printf(FILENOTOPENMSG, inputFileName);
            exit;
        }
        printf(FILEOPENMSG, inputFileName, inputStatus);

        long linesRead = 0;
        while (linesRead < NUMBER_OF_DATA_LINES)
        {
            size_t tokens_alloc = 1;
            size_t tokens_used = 0;
            char **tokens = calloc(tokens_alloc, sizeof(char *));
            while (tokens_used < tokens_limit)
            {

                if (nread = getline(&line, &len, inputFile) != -1)
                {
                    line = strsep(&line, "\n");

                    if (tokens_used == tokens_alloc)
                    {
                        tokens_alloc *= 2;
                        tokens = realloc(tokens, tokens_alloc * sizeof(char *));
                    }

                    tokens[tokens_used++] = strdup(line);
                    linesRead++;
                }
            }

            if (tokens_used == 0)
            {
                free(tokens);
                tokens = NULL;
            }
            else
            {
                tokens = realloc(tokens, tokens_used * sizeof(char *));
            }
            size_t tokens_inserted = 0;
            while (tokens_inserted < tokens_used)
            {
                insert(trieRoot, tokens[tokens_inserted]);
                tokens_inserted++;
            }
        }

        numberOfDataFilesRead++;

    } while (inputStatus == 1 && numberOfDataFilesRead < NUMBER_OF_DATA_FILES);

    createDictionaryFile(trieRoot);
}

void *task4threaded(void *readFileParam)
{

    char *line = NULL;
    size_t len = 0;
    size_t nread;

    int inputStatus = 0;

    short numberOfDataFilesRead = 0;

    long linesRead = 0;
    while ((nread = getline(&line, &len, readFileParam) != -1) && linesRead < NUMBER_OF_DATA_LINES)
    {
        line = strsep(&line, "\n");
        pthread_mutex_lock(&mutex);
        insert(ThreadedTrie, line);
        pthread_mutex_unlock(&mutex);

        linesRead++;
    }

    numberOfDataFilesRead++;
}

void *task5threaded(void *readFileParam)
{
    struct TrieNode_t *localTrie = getNodePrivate();
    char *line = NULL;
    size_t len = 0;
    size_t nread;

    int inputStatus = 0;

    long linesRead = 0;
    while ((nread = getline(&line, &len, readFileParam) != -1) && linesRead < NUMBER_OF_DATA_LINES)
    {
        line = strsep(&line, "\n");
        // pthread_mutex_lock(&mutex);
        insert(localTrie, line);
        // pthread_mutex_unlock(&mutex);

        linesRead++;
    }

    return localTrie;
}

void task4()
{

    pthread_t t[10];

    FILE *inputFile;
    short numberOfDataFilesRead = 0;
    ThreadedTrie = getNodePrivate();

    int inputStatus = 0;
    char inputFileName[] = "------------------";

    FILE *input = fopen(INPUTFILESNAME, "r");
    if (input == NULL)
    {
        printf(FILENOTOPENMSG, INPUTFILESNAME);
        exit;
    }
    do
    {

        inputStatus = fscanf(input, "%s", &inputFileName);
        inputFile = fopen(inputFileName, FILEINMODE);

        if (inputFile == NULL)
        {
            printf(FILENOTOPENMSG, inputFileName);
            exit;
        }
        printf(FILEOPENMSG, inputFileName, inputStatus);

        pthread_create(&t[numberOfDataFilesRead], NULL, task4threaded, inputFile);
        // task4threaded(inputFile);
        numberOfDataFilesRead++;

    } while (inputStatus == 1 && numberOfDataFilesRead < NUMBER_OF_DATA_FILES);
    for (int i = 0; i < 10; i++)
    {
        pthread_join(t[i], NULL);
    }

    createDictionaryFile(ThreadedTrie);
}

void task6()
{

    FILE *inputFile;

    char *line = NULL;
    size_t len = 0;
    size_t nread;

    struct TrieNode_t *trieRoot = getNodePrivate();

    int inputStatus = 0;
    char inputFileName[] = "------------------";
    FILE *input = fopen(INPUTFILESNAME, "r");

    if (input == NULL)
    {
        printf(FILENOTOPENMSG, INPUTFILESNAME);
        exit;
    }

    short numberOfDataFilesRead = 0;

    size_t tokens_alloc = 1;
    size_t tokens_used = 0;
    char **tokens = calloc(tokens_alloc, sizeof(char *));
    do
    {
        inputStatus = fscanf(input, "%s", &inputFileName);
        inputFile = fopen(inputFileName, FILEINMODE);
        if (inputFile == NULL)
        {
            printf(FILENOTOPENMSG, inputFileName);
            exit;
        }
        printf(FILEOPENMSG, inputFileName, inputStatus);

        long linesRead = 0;

        while (linesRead < NUMBER_OF_DATA_LINES)
        {
            if (nread = getline(&line, &len, inputFile) != -1)
            {
                line = strsep(&line, "\n");

                if (tokens_used == tokens_alloc)
                {
                    tokens_alloc *= 2;
                    tokens = realloc(tokens, tokens_alloc * sizeof(char *));
                }

                tokens[tokens_used++] = strdup(line);
                linesRead++;
            }
        }

        numberOfDataFilesRead++;

    } while (inputStatus == 1 && numberOfDataFilesRead < NUMBER_OF_DATA_FILES && tokens_used < TOKENS_LIMIT);

    size_t tokens_inserted = 0;
    while (tokens_inserted < tokens_used)
    {
        insert(trieRoot, tokens[tokens_inserted]);
        tokens_inserted++;
    }

    createDictionaryFile(trieRoot);
}

void task5transfer()
{
    char words1[][333] = {"Lorem ipsum dolor sit amet, consectetur adipiscing elit.",
                          "Curabitur at lacus hendrerit, ut, consectetur felis.	",
                          "Nunc in felis interdum, posuere nunc elementum, vestibulum leo.	",
                          "Ut bibendum orci at luctus convallis.	",
                          "Donec dictum augue vel luctus feugiat.	",
                          "Pellentesque lacinia augue ac tortor finibus efficitur.	",
                          "Vivamus tempus urna et erat condimentum efficitur.	",
                          "Suspendisse suscipit mauris sed mauris viverra molestie.	",
                          "Phasellus at massa et sem consequat faucibus in quis arcu.	",
                          "Aliquam elementum magna nec orci efficitur mollis.	",
                          "Aliquam at metus non dolor malesuada imperdiet.	",
                          "Donec porta mauris non nulla euismod, sit amet consectetur arcu tempor.	",
                          "Nunc in nisi ac eros sagittis pretium.	",
                          "Praesent a purus dictum dui congue faucibus et ut diam.	",
                          "Aliquam placerat metus et felis pellentesque congue.	",
                          "Sed non turpis lacinia, consectetur elit at, laoreet velit.	",
                          "Suspendisse quis turpis ut metus blandit volutpat.	",
                          "Sed ac nibh imperdiet, suscipit mi non, laoreet ligula.	",
                          "Aliquam a ante non enim vulputate blandit.	",
                          "Nunc ac eros in ex ullamcorper suscipit.	",
                          "Integer aliquam tortor ut tortor lacinia aliquam.	",
                          "Curabitur at lacus hendrerit, ut, consectetur felis.	",
                          "Nunc in felis interdum, posuere nunc elementum, vestibulum leo.	",
                          "Ut bibendum orci at luctus convallis.	",
                          "Donec dictum augue vel luctus feugiat.	",
                          "Pellentesque lacinia augue ac tortor finibus efficitur.	",
                          "Vivamus tempus urna et erat condimentum efficitur.	",
                          "Suspendisse suscipit mauris sed mauris viverra molestie.	",
                          "Phasellus at massa et sem consequat faucibus in quis arcu.	",
                          "Aliquam elementum magna nec orci efficitur mollis.	",
                          "Aliquam at metus non dolor malesuada imperdiet.	",
                          "Donec porta mauris non nulla euismod, sit amet consectetur arcu tempor.	",
                          "Nunc in nisi ac eros sagittis pretium.	",
                          "Praesent a purus dictum dui congue faucibus et ut diam.	",
                          "Aliquam placerat metus et felis pellentesque congue.	",
                          "Sed non turpis lacinia, consectetur elit at, laoreet velit.	",
                          "Suspendisse quis turpis ut metus blandit volutpat.	",
                          "Sed ac nibh imperdiet, suscipit mi non, laoreet ligula.	",
                          "Aenean in dui eu orci porttitor gravida.	",
                          "Aliquam sollicitudin sem quis porta commodo.	",
                          "Ut scelerisque nunc a enim eleifend scelerisque.	",
                          "Nam ac ligula vitae nunc congue pellentesque malesuada ac magna.	",
                          "Nulla fringilla odio eget venenatis hendrerit.	",
                          "Donec nec neque malesuada, sodales odio vel, viverra turpis.	",
                          "Praesent maximus metus nec viverra tristique.	",
                          "Nunc feugiat tortor a libero lobortis, a egestas lorem viverra.	",
                          "Etiam luctus nibh tincidunt, pharetra dolor vel, porta massa.	",
                          "Donec quis mauris cursus, fringilla ex et, commodo risus.	",
                          "Sed commodo tortor quis urna congue fringilla nec sit amet eros.	",
                          "Praesent luctus ligula vel vehicula consectetur.	",
                          "Pellentesque placerat metus sed arcu consequat, at aliquet ex ullamcorper.	",
                          "Praesent id lectus vel lectus interdum vestibulum at id odio.	",
                          "Proin sit amet mi ut sem blandit imperdiet.	",
                          "Fusce ac dolor eu urna posuere semper.	",
                          "Aenean varius mauris sed sapien iaculis, non ultricies lacus egestas.	",
                          "Duis sed eros ac turpis varius ultrices.	",
                          "Vestibulum consectetur enim et nibh commodo, lobortis venenatis ipsum mollis.	",
                          "Donec commodo eros nec nibh ultricies, tristique sagittis ante tempor.	",
                          "Praesent maximus metus nec viverra tristique.	",
                          "Nunc feugiat tortor a libero lobortis, a egestas lorem viverra.	",
                          "Etiam luctus nibh tincidunt, pharetra dolor vel, porta massa.	",
                          "Donec quis mauris cursus, fringilla ex et, commodo risus.	",
                          "Sed commodo tortor quis urna congue fringilla nec sit amet eros.	",
                          "Praesent luctus ligula vel vehicula consectetur.	",
                          "Pellentesque placerat metus sed arcu consequat, at aliquet ex ullamcorper.	",
                          "Praesent id lectus vel lectus interdum vestibulum at id odio.	",
                          "Proin sit amet mi ut sem blandit imperdiet.	",
                          "Fusce ac dolor eu urna posuere semper.	",
                          "Aenean varius mauris sed sapien iaculis, non ultricies lacus egestas.	",
                          "Duis sed eros ac turpis varius ultrices.	",
                          "Vestibulum consectetur enim et nibh commodo, lobortis venenatis ipsum mollis.	",
                          "Donec commodo eros nec nibh ultricies, tristique sagittis ante tempor.	",
                          "Nullam nec justo sagittis, rhoncus diam eget, mattis libero.	",
                          "Vestibulum nec odio lacinia, sodales elit in, consequat diam.	",
                          "Donec iaculis nisi at feugiat auctor.	",
                          "Aliquam id ligula tempor, fringilla turpis sit amet, dictum nisl.	",
                          "Ut pellentesque quam vitae tellus consectetur, a elementum enim pellentesque.	",
                          "Etiam porta ipsum id felis fermentum, condimentum sollicitudin lacus scelerisque.	",
                          "Nam ac quam in nunc eleifend eleifend.	",
                          "Duis ultrices eros sed maximus porttitor.	",
                          "Proin aliquet ipsum ut massa mattis lobortis.	",
                          "Integer faucibus libero eu turpis mollis, eget pretium nibh pellentesque.	",
                          "Duis porta arcu et risus convallis interdum id ut velit.	",
                          "Quisque imperdiet lorem sed est molestie, aliquet euismod mi rhoncus.	",
                          "Nullam nec justo sagittis, rhoncus diam eget, mattis libero.	",
                          "Vestibulum nec odio lacinia, sodales elit in, consequat diam.	",
                          "Donec iaculis nisi at feugiat auctor.	",
                          "Aliquam id ligula tempor, fringilla turpis sit amet, dictum nisl.	",
                          "Ut pellentesque quam vitae tellus consectetur, a elementum enim pellentesque.	",
                          "Etiam porta ipsum id felis fermentum, condimentum sollicitudin lacus scelerisque.	",
                          "Nam ac quam in nunc eleifend eleifend.	",
                          "Duis ultrices eros sed maximus porttitor.	",
                          "Proin aliquet ipsum ut massa mattis lobortis.	",
                          "Integer faucibus libero eu turpis mollis, eget pretium nibh pellentesque.	",
                          "Duis porta arcu et risus convallis interdum id ut velit.	",
                          "Quisque imperdiet lorem sed est molestie, aliquet euismod mi rhoncus.	",
                          "Nulla faucibus est a erat consequat, eu viverra risus ultrices.	",
                          "Fusce molestie ipsum vitae dictum tempor.	",
                          "Vivamus ac purus pellentesque, pulvinar felis vitae, lobortis sapien.	",
                          "Integer sollicitudin eros id suscipit lacinia.	",
                          "Proin quis dolor ut nulla hendrerit mollis.	",
                          "Etiam vitae augue at libero volutpat eleifend quis non purus.	",
                          "Ut et erat feugiat, bibendum nibh ut, fermentum nibh.	",
                          "Curabitur tincidunt massa in sapien pellentesque venenatis.	",
                          "Ut vulputate magna vitae tortor venenatis iaculis.	",
                          "Etiam posuere diam et sapien iaculis, eu dapibus turpis sagittis.	",
                          "Phasellus vel lectus ut urna efficitur ultrices.	",
                          "Nam ultricies justo a quam varius vehicula eget sed mauris.	",
                          "Mauris efficitur ante eget arcu consequat mattis.	",
                          "Ut dignissim massa pulvinar hendrerit varius.	",
                          "Curabitur hendrerit erat rutrum nisl dignissim, sed pharetra est tincidunt.	",
                          "Etiam non risus at justo facilisis lobortis.	",
                          "Pellentesque sagittis elit efficitur eleifend venenatis.	",
                          "Nulla sit amet orci rutrum, viverra felis sit amet, condimentum neque.	",
                          "Sed elementum tellus in erat aliquet imperdiet.	",
                          "Pellentesque suscipit justo et ipsum accumsan, eu condimentum nisi lacinia.	",
                          "Morbi sagittis leo in purus elementum, vitae pretium urna tincidunt.	",
                          "Vivamus accumsan purus at nulla vulputate, et varius odio accumsan.	",
                          "Ut mollis urna a augue accumsan dictum.	",
                          "Maecenas ac diam vitae justo mattis luctus.	",
                          "Curabitur sit amet purus eget justo consectetur vestibulum.	",
                          "Duis porta arcu et risus convallis interdum id ut velit.	",
                          "Quisque imperdiet lorem sed est molestie, aliquet euismod mi rhoncus.	",
                          "Nulla faucibus est a erat consequat, eu viverra risus ultrices.	",
                          "Fusce molestie ipsum vitae dictum tempor.	",
                          "Vivamus ac purus pellentesque, pulvinar felis vitae, lobortis sapien.	",
                          "Integer sollicitudin eros id suscipit lacinia.	",
                          "Proin quis dolor ut nulla hendrerit mollis.	",
                          "Etiam vitae augue at libero volutpat eleifend quis non purus.	",
                          "Ut et erat feugiat, bibendum nibh ut, fermentum nibh.	",
                          "Curabitur tincidunt massa in sapien pellentesque venenatis.	",
                          "Ut vulputate magna vitae tortor venenatis iaculis.	",
                          "Etiam posuere diam et sapien iaculis, eu dapibus turpis sagittis.	",
                          "Phasellus vel lectus ut urna efficitur ultrices.	",
                          "Nam ultricies justo a quam varius vehicula eget sed mauris.	",
                          "Mauris efficitur ante eget arcu consequat mattis.	",
                          "Ut dignissim massa pulvinar hendrerit varius.	",
                          "Curabitur hendrerit erat rutrum nisl dignissim, sed pharetra est tincidunt.	",
                          "Etiam sit amet urna dictum, pretium mauris non, bibendum magna.	",
                          "Phasellus ac odio viverra, ultrices magna in, tempus lacus.	",
                          "Curabitur at neque ut felis rhoncus aliquam euismod eu erat.	",
                          "Nulla blandit mi aliquet, cursus felis at, euismod lorem.	",
                          "Proin et dolor in erat varius sollicitudin.	",
                          "Phasellus rutrum tortor eget mi lacinia, eget malesuada felis tincidunt.	",
                          "Suspendisse vel neque mollis, semper est eu, lobortis magna.	",
                          "Sed a magna vestibulum, finibus sem et, mattis justo.	",
                          "Etiam nec felis in elit iaculis ullamcorper eget et sem.	",
                          "Quisque ac nisi in risus pretium iaculis.	",
                          "Vestibulum ac libero bibendum, congue nibh ut, vehicula odio.	",
                          "Morbi luctus nunc in enim commodo ultricies.	",
                          "Aenean vehicula ipsum ac dui suscipit, ultrices molestie purus eleifend.	",
                          "Vivamus eget diam faucibus, convallis nisi et, accumsan ipsum.	",
                          "Praesent feugiat est dignissim consectetur eleifend.	",
                          "Duis porta arcu et risus convallis interdum id ut velit.	",
                          "Quisque imperdiet lorem sed est molestie, aliquet euismod mi rhoncus.	",
                          "Nulla faucibus est a erat consequat, eu viverra risus ultrices.	",
                          "Fusce molestie ipsum vitae dictum tempor.	",
                          "Vivamus ac purus pellentesque, pulvinar felis vitae, lobortis sapien.	",
                          "Integer sollicitudin eros id suscipit lacinia.	",
                          "Proin quis dolor ut nulla hendrerit mollis.	",
                          "Etiam vitae augue at libero volutpat eleifend quis non purus.	",
                          "Ut et erat feugiat, bibendum nibh ut, fermentum nibh.	",
                          "Curabitur tincidunt massa in sapien pellentesque venenatis.	",
                          "Ut vulputate magna vitae tortor venenatis iaculis.	",
                          "Etiam posuere diam et sapien iaculis, eu dapibus turpis sagittis.	",
                          "Phasellus vel lectus ut urna efficitur ultrices.	",
                          "Nam ultricies justo a quam varius vehicula eget sed mauris.	",
                          "Mauris efficitur ante eget arcu consequat mattis.	",
                          "Ut dignissim massa pulvinar hendrerit varius.	",
                          "Curabitur hendrerit erat rutrum nisl dignissim, sed pharetra est tincidunt.	",
                          "Phasellus euismod turpis ac ante rhoncus, id placerat diam ultricies.	",
                          "Vivamus at metus ultricies, dictum leo et, cursus augue.	",
                          "Nunc sed purus a sem ullamcorper facilisis nec sed augue.	",
                          "Vivamus elementum lectus ut nulla dapibus auctor.	",
                          "Mauris bibendum odio vitae sollicitudin ornare.	",
                          "Cras et odio commodo nisi auctor porttitor nec sed massa."
                          "Phasellus ac odio viverra, ultrices magna in, tempus lacus.	",
                          "Curabitur at neque ut felis rhoncus aliquam euismod eu erat.	",
                          "Nulla blandit mi aliquet, cursus felis at, euismod lorem.	",
                          "Proin et dolor in erat varius sollicitudin.	",
                          "Phasellus rutrum tortor eget mi lacinia, eget malesuada felis tincidunt.	",
                          "Suspendisse vel neque mollis, semper est eu, lobortis magna.	",
                          "Sed a magna vestibulum, finibus sem et, mattis justo.	",
                          "Etiam nec felis in elit iaculis ullamcorper eget et sem.	",
                          "Quisque ac nisi in risus pretium iaculis.	",
                          "Vestibulum ac libero bibendum, congue nibh ut, vehicula odio.	",
                          "Morbi luctus nunc in enim commodo ultricies.	",
                          "Aenean vehicula ipsum ac dui suscipit, ultrices molestie purus eleifend.	",
                          "Vivamus eget diam faucibus, convallis nisi et, accumsan ipsum.	",
                          "Praesent feugiat est dignissim consectetur eleifend.	",
                          "Phasellus euismod turpis ac ante rhoncus, id placerat diam ultricies.	",
                          "Vivamus at metus ultricies, dictum leo et, cursus augue.	",
                          "Nunc sed purus a sem ullamcorper facilisis nec sed augue.	",
                          "Vivamus elementum lectus ut nulla dapibus auctor.	",
                          "Mauris bibendum odio vitae sollicitudin ornare.	",
                          "Cras et odio commodo nisi auctor porttitor nec sed massa."

    };

    char words2[][333] = {"Lorem ipsum dolor sit amet, consectetur adipiscing elit.",
                          "Curabitur at lacus hendrerit, ut, consectetur felis.	",
                          "Nunc in felis interdum, posuere nunc elementum, vestibulum leo.	",
                          "Ut bibendum orci at luctus convallis.	",
                          "Donec dictum augue vel luctus feugiat.	",
                          "Pellentesque lacinia augue ac tortor finibus efficitur.	",
                          "Vivamus tempus urna et erat condimentum efficitur.	",
                          "Suspendisse suscipit mauris sed mauris viverra molestie.	",
                          "Phasellus at massa et sem consequat faucibus in quis arcu.	",
                          "Aliquam elementum magna nec orci efficitur mollis.	",
                          "Aliquam at metus non dolor malesuada imperdiet.	",
                          "Donec porta mauris non nulla euismod, sit amet consectetur arcu tempor.	",
                          "Nunc in nisi ac eros sagittis pretium.	",
                          "Praesent a purus dictum dui congue faucibus et ut diam.	",
                          "Aliquam placerat metus et felis pellentesque congue.	",
                          "Sed non turpis lacinia, consectetur elit at, laoreet velit.	",
                          "Suspendisse quis turpis ut metus blandit volutpat.	",
                          "Sed ac nibh imperdiet, suscipit mi non, laoreet ligula.	",
                          "Aliquam a ante non enim vulputate blandit.	",
                          "Nunc ac eros in ex ullamcorper suscipit.	",
                          "Integer aliquam tortor ut tortor lacinia aliquam.	",
                          "Curabitur at lacus hendrerit, ut, consectetur felis.	",
                          "Nunc in felis interdum, posuere nunc elementum, vestibulum leo.	",
                          "Ut bibendum orci at luctus convallis.	",
                          "Donec dictum augue vel luctus feugiat.	",
                          "Pellentesque lacinia augue ac tortor finibus efficitur.	",
                          "Vivamus tempus urna et erat condimentum efficitur.	",
                          "Suspendisse suscipit mauris sed mauris viverra molestie.	",
                          "Phasellus at massa et sem consequat faucibus in quis arcu.	",
                          "Aliquam elementum magna nec orci efficitur mollis.	",
                          "Aliquam at metus non dolor malesuada imperdiet.	",
                          "Donec porta mauris non nulla euismod, sit amet consectetur arcu tempor.	",
                          "Nunc in nisi ac eros sagittis pretium.	",
                          "Praesent a purus dictum dui congue faucibus et ut diam.	",
                          "Aliquam placerat metus et felis pellentesque congue.	",
                          "Sed non turpis lacinia, consectetur elit at, laoreet velit.	",
                          "Suspendisse quis turpis ut metus blandit volutpat.	",
                          "Sed ac nibh imperdiet, suscipit mi non, laoreet ligula.	",
                          "Aenean in dui eu orci porttitor gravida.	",
                          "Aliquam sollicitudin sem quis porta commodo.	",
                          "Ut scelerisque nunc a enim eleifend scelerisque.	",
                          "Nam ac ligula vitae nunc congue pellentesque malesuada ac magna.	",
                          "Nulla fringilla odio eget venenatis hendrerit.	",
                          "Donec nec neque malesuada, sodales odio vel, viverra turpis.	",
                          "Praesent maximus metus nec viverra tristique.	",
                          "Nunc feugiat tortor a libero lobortis, a egestas lorem viverra.	",
                          "Etiam luctus nibh tincidunt, pharetra dolor vel, porta massa.	",
                          "Donec quis mauris cursus, fringilla ex et, commodo risus.	",
                          "Sed commodo tortor quis urna congue fringilla nec sit amet eros.	",
                          "Praesent luctus ligula vel vehicula consectetur.	",
                          "Pellentesque placerat metus sed arcu consequat, at aliquet ex ullamcorper.	",
                          "Praesent id lectus vel lectus interdum vestibulum at id odio.	",
                          "Proin sit amet mi ut sem blandit imperdiet.	",
                          "Fusce ac dolor eu urna posuere semper.	",
                          "Aenean varius mauris sed sapien iaculis, non ultricies lacus egestas.	",
                          "Duis sed eros ac turpis varius ultrices.	",
                          "Vestibulum consectetur enim et nibh commodo, lobortis venenatis ipsum mollis.	",
                          "Donec commodo eros nec nibh ultricies, tristique sagittis ante tempor.	",
                          "Praesent maximus metus nec viverra tristique.	",
                          "Nunc feugiat tortor a libero lobortis, a egestas lorem viverra.	",
                          "Etiam luctus nibh tincidunt, pharetra dolor vel, porta massa.	",
                          "Donec quis mauris cursus, fringilla ex et, commodo risus.	",
                          "Sed commodo tortor quis urna congue fringilla nec sit amet eros.	",
                          "Praesent luctus ligula vel vehicula consectetur.	",
                          "Pellentesque placerat metus sed arcu consequat, at aliquet ex ullamcorper.	",
                          "Praesent id lectus vel lectus interdum vestibulum at id odio.	",
                          "Proin sit amet mi ut sem blandit imperdiet.	",
                          "Fusce ac dolor eu urna posuere semper.	",
                          "Aenean varius mauris sed sapien iaculis, non ultricies lacus egestas.	",
                          "Duis sed eros ac turpis varius ultrices.	",
                          "Vestibulum consectetur enim et nibh commodo, lobortis venenatis ipsum mollis.	",
                          "Donec commodo eros nec nibh ultricies, tristique sagittis ante tempor.	",
                          "Nullam nec justo sagittis, rhoncus diam eget, mattis libero.	",
                          "Vestibulum nec odio lacinia, sodales elit in, consequat diam.	",
                          "Donec iaculis nisi at feugiat auctor.	",
                          "Aliquam id ligula tempor, fringilla turpis sit amet, dictum nisl.	",
                          "Ut pellentesque quam vitae tellus consectetur, a elementum enim pellentesque.	",
                          "Etiam porta ipsum id felis fermentum, condimentum sollicitudin lacus scelerisque.	",
                          "Nam ac quam in nunc eleifend eleifend.	",
                          "Duis ultrices eros sed maximus porttitor.	",
                          "Proin aliquet ipsum ut massa mattis lobortis.	",
                          "Integer faucibus libero eu turpis mollis, eget pretium nibh pellentesque.	",
                          "Duis porta arcu et risus convallis interdum id ut velit.	",
                          "Quisque imperdiet lorem sed est molestie, aliquet euismod mi rhoncus.	",
                          "Nullam nec justo sagittis, rhoncus diam eget, mattis libero.	",
                          "Vestibulum nec odio lacinia, sodales elit in, consequat diam.	",
                          "Donec iaculis nisi at feugiat auctor.	",
                          "Aliquam id ligula tempor, fringilla turpis sit amet, dictum nisl.	",
                          "Ut pellentesque quam vitae tellus consectetur, a elementum enim pellentesque.	",
                          "Etiam porta ipsum id felis fermentum, condimentum sollicitudin lacus scelerisque.	",
                          "Nam ac quam in nunc eleifend eleifend.	",
                          "Duis ultrices eros sed maximus porttitor.	",
                          "Proin aliquet ipsum ut massa mattis lobortis.	",
                          "Integer faucibus libero eu turpis mollis, eget pretium nibh pellentesque.	",
                          "Duis porta arcu et risus convallis interdum id ut velit.	",
                          "Quisque imperdiet lorem sed est molestie, aliquet euismod mi rhoncus.	",
                          "Nulla faucibus est a erat consequat, eu viverra risus ultrices.	",
                          "Fusce molestie ipsum vitae dictum tempor.	",
                          "Vivamus ac purus pellentesque, pulvinar felis vitae, lobortis sapien.	",
                          "Integer sollicitudin eros id suscipit lacinia.	",
                          "Proin quis dolor ut nulla hendrerit mollis.	",
                          "Etiam vitae augue at libero volutpat eleifend quis non purus.	",
                          "Ut et erat feugiat, bibendum nibh ut, fermentum nibh.	",
                          "Curabitur tincidunt massa in sapien pellentesque venenatis.	",
                          "Ut vulputate magna vitae tortor venenatis iaculis.	",
                          "Etiam posuere diam et sapien iaculis, eu dapibus turpis sagittis.	",
                          "Phasellus vel lectus ut urna efficitur ultrices.	",
                          "Nam ultricies justo a quam varius vehicula eget sed mauris.	",
                          "Mauris efficitur ante eget arcu consequat mattis.	",
                          "Ut dignissim massa pulvinar hendrerit varius.	",
                          "Curabitur hendrerit erat rutrum nisl dignissim, sed pharetra est tincidunt.	",
                          "Etiam non risus at justo facilisis lobortis.	",
                          "Pellentesque sagittis elit efficitur eleifend venenatis.	",
                          "Nulla sit amet orci rutrum, viverra felis sit amet, condimentum neque.	",
                          "Sed elementum tellus in erat aliquet imperdiet.	",
                          "Pellentesque suscipit justo et ipsum accumsan, eu condimentum nisi lacinia.	",
                          "Morbi sagittis leo in purus elementum, vitae pretium urna tincidunt.	",
                          "Vivamus accumsan purus at nulla vulputate, et varius odio accumsan.	",
                          "Ut mollis urna a augue accumsan dictum.	",
                          "Maecenas ac diam vitae justo mattis luctus.	",
                          "Curabitur sit amet purus eget justo consectetur vestibulum.	",
                          "Duis porta arcu et risus convallis interdum id ut velit.	",
                          "Quisque imperdiet lorem sed est molestie, aliquet euismod mi rhoncus.	",
                          "Nulla faucibus est a erat consequat, eu viverra risus ultrices.	",
                          "Fusce molestie ipsum vitae dictum tempor.	",
                          "Vivamus ac purus pellentesque, pulvinar felis vitae, lobortis sapien.	",
                          "Integer sollicitudin eros id suscipit lacinia.	",
                          "Proin quis dolor ut nulla hendrerit mollis.	",
                          "Etiam vitae augue at libero volutpat eleifend quis non purus.	",
                          "Ut et erat feugiat, bibendum nibh ut, fermentum nibh.	",
                          "Curabitur tincidunt massa in sapien pellentesque venenatis.	"

    };

    struct TrieNode_t *dictionary1 = getNodePrivate();
    struct TrieNode_t *dictionary2 = getNodePrivate();

    // Construct trie
    for (short i = 0; i < ARRAY_SIZE(words1); i++)
        insert(dictionary1, words1[i]);

    for (short i = 0; i < ARRAY_SIZE(words1); i++)
        insert(dictionary2, words2[i]);

    struct TrieNode_t *trans = getNodePrivate();
    // trans = createDictionaryTransfered(dictionary1);
    transfered = getNodePrivate();

    createDictionaryTransfered(dictionary1);
    createDictionaryTransfered(dictionary2);

    showDictionary(transfered);
    createDictionaryFile(transfered);
}

void task4copyfor5()
{

    pthread_t t[10];

    FILE *inputFile;
    short numberOfDataFilesRead = 0;

    // for (size_t i = 0; i < NUMBER_OF_DATA_FILES; i++)
    // {
    //     ThreadedTries[i] = getNodePrivate();
    // }

    int inputStatus = 0;
    char inputFileName[] = "------------------";

    FILE *input = fopen(INPUTFILESNAME, "r");
    if (input == NULL)
    {
        printf(FILENOTOPENMSG, INPUTFILESNAME);
        exit;
    }
    do
    {
        ThreadedTries[numberOfDataFilesRead];

        inputStatus = fscanf(input, "%s", &inputFileName);
        inputFile = fopen(inputFileName, FILEINMODE);

        if (inputFile == NULL)
        {
            printf(FILENOTOPENMSG, inputFileName);
            exit;
        }
        printf(FILEOPENMSG, inputFileName, inputStatus);

        pthread_create(&t[numberOfDataFilesRead], NULL, task5threaded, inputFile);
        // task4threaded(inputFile);
        numberOfDataFilesRead++;

    } while (inputStatus == 1 && numberOfDataFilesRead < NUMBER_OF_DATA_FILES);

    for (int i = 0; i < 10; i++)
    {
        pthread_join(t[i], (void *)&ThreadedTries[i]);
    }
    transfered = getNodePrivate();

    for (size_t i = 0; i < NUMBER_OF_DATA_FILES; i++)
    {
        createDictionaryTransfered(ThreadedTries[i]);
    }

    createDictionaryFile(transfered);
}

void prankCaller(char *number)
{
    int no = atoi(number);
    switch (no)
    {
    case 2:
        task2();
        break;
    case 3:
        task3();
        break;
    case 4:
        task4();
        break;
    case 5:
        task4copyfor5();
        break;
    case 6:
        task6();
        break;
    case 7:
        printf("In the improved version %d\n", no);
        task6improved();
        break;
    default:
        printf("No such command like %d\n", no);
        break;
    }
}

void *task4threadedforimprovement(void *readFileParam)
{

    char *line = NULL;
    size_t len = 0;
    size_t nread;

    int inputStatus = 0;

    short numberOfDataFilesRead = 0;

    long linesRead = 0;
    while ((nread = getline(&line, &len, readFileParam) != -1) && linesRead < NUMBER_OF_DATA_LINES)
    {
        line = strsep(&line, "\n");
        pthread_mutex_lock(&mutex);

        if (tokens_usedGlobal == tokens_allocGlobal)
        {
            tokens_allocGlobal *= 2;
            tokensGlobal = realloc(tokensGlobal, tokens_allocGlobal * sizeof(char *));
        }

        tokensGlobal[tokens_usedGlobal++] = strdup(line);
        pthread_mutex_unlock(&mutex);

        linesRead++;
    }

    numberOfDataFilesRead++;
}

void task6improved()
{
    pthread_t t[10];

    FILE *inputFile;

    char *line = NULL;
    size_t len = 0;
    size_t nread;

    struct TrieNode_t *trieRoot = getNodePrivate();

    int inputStatus = 0;
    char inputFileName[] = "------------------";
    FILE *input = fopen(INPUTFILESNAME, "r");

    if (input == NULL)
    {
        printf(FILENOTOPENMSG, INPUTFILESNAME);
        exit;
    }

    short numberOfDataFilesRead = 0;

    tokens_allocGlobal = 1;
    tokens_usedGlobal = 0;
    tokensGlobal = calloc(tokens_allocGlobal, sizeof(char *));
    do
    {

        inputStatus = fscanf(input, "%s", &inputFileName);
        inputFile = fopen(inputFileName, FILEINMODE);

        if (inputFile == NULL)
        {
            printf(FILENOTOPENMSG, inputFileName);
            exit;
        }
        printf(FILEOPENMSG, inputFileName, inputStatus);

        pthread_create(&t[numberOfDataFilesRead], NULL, task4threadedforimprovement, inputFile);
        // task4threaded(inputFile);
        numberOfDataFilesRead++;

    } while (inputStatus == 1 && numberOfDataFilesRead < NUMBER_OF_DATA_FILES && tokens_usedGlobal < TOKENS_LIMIT);

    for (int i = 0; i < 10; i++)
    {
        pthread_join(t[i], NULL);
    }
    size_t tokens_inserted = 0;
    while (tokens_inserted < tokens_usedGlobal)
    {
        insert(trieRoot, tokensGlobal[tokens_inserted]);
        tokens_inserted++;
    }
    createDictionaryFile(trieRoot);
}

int main(int argc, char const *argv[])
{
    prankCaller(argv[1]);

    return EXIT_SUCCESS;
}
