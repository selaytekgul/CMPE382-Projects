#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define FILEOUTMODE "w+"

#define PRINTFORMAT "%s\t%d\n"

#define OUTPUTFILENAME "Dictionary.txt"

#define ARRAY_SIZE(a) sizeof(a) / sizeof(a[0])

// Alphabet size (# of symbols)
#define ALPHABET_SIZE (91)

// Converts key current character into index
// use only 'a' through 'z' and lower case
#define CHAR_TO_INDEX(c) ((short)c - (short)' ')

struct TrieNode_t *transfered;

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
        printf("Prefix: %s\t##%d\n", prefix, node->wordNumber);
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

void task1()
{

    char words[][100] = {"Lorem ipsum dolor sit amet, consectetur adipiscing elit.",
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
    struct TrieNode_t *dictionary = getNodePrivate();

    // Construct trie
    for (short i = 0; i < ARRAY_SIZE(words); i++)
        insert(dictionary, words[i]);

    createDictionaryFile(dictionary);
    showDictionary(dictionary);

    char output[][32] = {"Not present in trie", "Present in trie"};
    printf("\n%s --- %s\n", "Vivamus elementum lectus ut nulla dapibus auctor.", output[search(dictionary, "Vivamus elementum lectus ut nulla dapibus auctor.")]);
    printf("%s --- %s\n", "SELAY", output[search(dictionary, "SELAY")]);
}

void task1forMergeTrie()
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

void prankCaller(char *number)
{
    int no = atoi(number);
    switch (no)
    {
    case 1:
        task1();
        break;
    case 11:
        task1forMergeTrie();
        break;
    default:
        printf("No such command like %d\n", no);
        break;
    }
}
int main(int argc, char const *argv[])
{
    prankCaller(argv[1]);

    return EXIT_SUCCESS;
}
