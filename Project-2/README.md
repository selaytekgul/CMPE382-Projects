# CMPE 382 – OPERATING SYSTEMS
# PROJECT-2
>The aim of the project is to implement multi-threaded programming in C programming language.
>To use threads the tasks that should be completed are about reading lines
>i.e. queries in the data files  that are large enough to see the effects on thread usage in the program.

##### Note1:
[Program opearates file open for two modes:][df1]
* #define FILEOUTMODE "w+"
* #define FILEINMODE "r"
	
##### Note2:
[There are proper messages created incase of file opening failures:][df1]
* #define FILENOTOPENMSG "We couldn't open file %s to read!\n"
* #define FILEOPENMSG "We're reading file %s with input status %d.\n"

### TASK 1:

Firstly, there should be a trie to be used in the query insertion process.
Also, in the trie (*struct TrieNode_t*) implementation there should be an integer value kept in the nodes of the trie regarding to the frequencies of the queries in the data file.

I have used the implementation that I found on website:
[geeks4geeks]

I have implemented the trie with the *wordNumber* value in the node of the struct in the *project2_1.c*.
In the trie implementation there are functions to be used in the other tasks.

    struct TrieNode_t *getNodePrivate(void)
    void insert(struct TrieNode_t *root, const char *key)
    bool search(struct TrieNode_t *root, const char *key)

There are also other output related functions that I have implemented:

    void trieToOutputPrivate(char *prefix, struct TrieNode_t *node)
    void showDictionary(struct TrieNode_t *dictionary)
    void trieToTxtPrivate(char *prefix, struct TrieNode_t *node, FILE *file)
    void createDictionaryFile(struct TrieNode_t *dictionary)

Additionally, I have implemented another function to be used in the merge operation needed in the task 5:

    struct TrieNode_t *trieToTransferPrivate(char *prefix, struct TrieNode_t *node, FILE *file)
    struct TrieNode_t *createDictionaryTransfered(struct TrieNode_t *dictionary)

To execute *project2_1.c* there are 2 options:
- to test a trie creation without access to a file:
```sh
$ make 1
```
- to test a trie merging without access to a file:

```sh
$ make 11
```
----------------------------------------------------------------------------------
Since the other tasks in the project are related with the file operations I have implemented them in another C file: *project2_2.c*
To begin with, according to have a flexible design for changing both the names and the number of the data files to be read, I am using another input file named #define INPUTFILESNAME "input.txt" consisting of the list of the names of the data files.
----------------------------------------------------------------------------------
### TASK 2:

Secondly, I have used my implemented trie to read *#define NUMBER_OF_DATA_FILES (10)* data files with *#define NUMBER_OF_DATA_LINES (40000)* lines from each of the files.
Also, I have inserted the queries that I read from the data files as soon as possible i.e. when I read *nread = getline(&line, &len, inputFile)* the line from the file I called the insert *insert(trieRoot, line);* function in the [void task2()] and after finishing creating the trie by reading all the data files there, the output text file which is *#define OUTPUTFILENAME "Dictionary.txt"* is *createDictionaryFile(trieRoot);* created.

To execute *project2_2.c* for `task2`:
```sh
$ make 2
```
### TASK 3:

Thirdly, I have used my implemented trie to read *#define NUMBER_OF_DATA_FILES (10)* data files with *#define NUMBER_OF_DATA_LINES (40000)* lines from each of the files.
Also, I have inserted the queries that I read from the data files after filling the limited *#define TOKENS_LIMIT (NUMBER_OF_DATA_LINES * NUMBER_OF_DATA_FILES)* the string(char *) array(char**) that I used in *void task3()* instead of inserting them when I read *nread = getline(&line, &len, inputFile)* from the file. After finishing creating the trie by reading all the data files there, the output text file which is *#define OUTPUTFILENAME "Dictionary.txt"* is *createDictionaryFile(trieRoot);* created.

To execute *project2_2.c* for `task3`:
```sh
$ make 3
```
### TASK 4:
>THREADED EXECUTION

Fourtly, I have used my implemented trie to read *#define NUMBER_OF_DATA_FILES (10)* data files with *#define NUMBER_OF_DATA_LINES (40000)* lines from each of the files.
Moreover, in this task, I have used another threaded function to be called in *void task4()*:

    void *task4threaded(void *readFileParam)
Therefore, I have started with declaring a *Global* *struct TrieNode_t *ThreadedTrie;*  and I have called the threaded function in the *void task4()* by creating *#define NUMBER_OF_DATA_FILES (10)* different threads for each file and after inserting all these queries which are read in a threaded fashion, I have construct a *ThreadedTrie* that I have initialize at the beginnig of the *void task4()* function.
After finishing creating the global variable trie by reading all the data files there and waiting them all to be finished by using *pthread_join(t[i], NULL);*, the output text file which is *#define OUTPUTFILENAME "Dictionary.txt"* is *createDictionaryFile(ThreadedTrie);* created.

To execute *project2_2.c* for `task4`:
```sh
$ make 4
```
### TASK 5:
>THREADED EXECUTION

Fiftly, I have used my implemented trie to read *#define NUMBER_OF_DATA_FILES (10)* data files with *#define NUMBER_OF_DATA_LINES (40000)* lines from each of the files.
Moreover again, in this task, I have used a different threaded function to be called in *void task5()*:

    void *task5threaded(void *readFileParam)
What is more in this task, as I mentioned in the implementation of the trie i.e. Task1, I have used another function of trie implementation which transferes a trie to another.
Since, it is already implemented in the trie that I use to search queries in the trie as well as I have added the wordCount specification for every word in the trie, while transfering a trie to another, I have added the queries found in the given trie to the one that I am trying to construct wordCount times instead of just once in the test function for merging operation *void task5transfer()* which combines 2 example tries to a new trie by using these two functions:

    struct TrieNode_t *trieToTransferPrivate(char *prefix, struct TrieNode_t *node, FILE *file)
    struct TrieNode_t *createDictionaryTransfered(struct TrieNode_t *dictionary)
    
So, since in this task I have to use the threaded function to construct local tries, I used the *Global* *struct TrieNode_t *ThreadedTrie;* as a combination of these local tries that I already used in the task4 and I have called the threaded function in the *void task5()* by creating *#define NUMBER_OF_DATA_FILES (10)* different threads for each file again as in the task4 and after constructing all the local tries which are constructed in a threaded fashion, I have transfered all those to another trie which is *transfered* that I have initialize just before  combining all ten tries at the *void task5()* function.
After finishing creating the global variable *transfered* trie, the output text file which is *#define OUTPUTFILENAME "Dictionary.txt"* is *createDictionaryFile(ThreadedTrie);* created.

To execute *project2_2.c* for `task5`:
```sh
$ make 5
```
### TASK 6:

Sixthly, I have used my implemented trie to read *#define NUMBER_OF_DATA_FILES (10)* data files with *#define NUMBER_OF_DATA_LINES (40000)* lines from each of the files.
Also, I have inserted the queries that I read from the data files after filling the string(char *) array(char**) that I used in *void task6()* instead of inserting them when I read *nread = getline(&line, &len, inputFile)* from the file.
Different than the implementation that I applied in the task3, this time my [string][df1] (char * ) [array][df1](char ** ) is not limited and it covers each file when there is a read operation into the memory. After finishing creating the string arrays for each of the files by reading them all here, before creating the output text file which is *#define OUTPUTFILENAME "Dictionary.txt"* by *createDictionaryFile(trieRoot);* of course all the queries are inserted to the trie.

To execute *project2_2.c* for `task6`:
```sh
$ make 6
```
### TASK 7:

Seventhly, I have used my implemented trie to read *#define NUMBER_OF_DATA_FILES (10)* data files with *#define NUMBER_OF_DATA_LINES (40000)* lines from each of the files.

In this task in order to improve both of the task4 and task6 I have created a *Global* [string][df1] (char * ) [array][df1](char ** ) which is not limited.

I the task6 I used to have a nonlimited [string array][df1] to not to access to the files each time of insertion to the tries. Now, in the improved version of task6 which is *void task6improved()* I am again reading all the files at first before creation of the trie but in a threaded way by using my new threaded function:

    void *task4threadedforimprovement(void *readFileParam)

I the new created threaded function called in *void task6improved()* I have applied the similar operations of the task4. However this time, in the threaded function I am adding all the queries that come from each file
- since each files are sent to their own threads
to the *Global* [string array][df1].

Importantly:
Since the [string array][df1] that all threads are accesing to write is **Global** I have to use
            pthread_mutex_lock(&mutex);
            pthread_mutex_unlock(&mutex);
while adding the queries to the Global string array.

After finishing creating the **one** string array for each of the files by reading them all here in a threaded fashion, before creating the output text file which is *#define OUTPUTFILENAME "Dictionary.txt"* by *createDictionaryFile(trieRoot);* of course all the queries are inserted to the trie.

To execute *project2_2.c* for `task7`:
```sh
$ make 7
```
-------------------------------------------------------------------------------------------------------------
##### Note3:
[In order to delete the Dictionary files created in the execution of the tasks each time you can remove the already created Dictionary.txt by using:][df1]
```sh
$ make rd
```
-------------------------------------------------------------------------------------------------------------
## EXECUTION TIMES
| TASKS | EXPLANATION |ELAPSED TIMES | CPU USAGE (%)
| ------ | ------ | ------ | ------ |
| TASK 1 | Building a Memory-Based Data Structure for Building the Dictionary|0:00.01 |56|
| TASK 1.1 |Building a Memory-Based Data Structure for Building the Dictionary Merge| 0:00.06 |47|
| TASK 2 |Sequential Execution - One Query at a Time| 0:02.94 |95|
| TASK 3 |Sequential Execution - Multiple Queries |0:02.81 |94|
| TASK 4 |Threaded Execution| 0:03.34 |87|
| TASK 5 | Threaded Execution - Multiple Tries |0:05.47 |112|
| TASK 6 | Completely Memory-Based Dictionary Creation |0:03.12 |95|
| TASK 7 | Improvement | 0:02.92| 96|

   [geeks4geeks]: <hhttps://www.geeksforgeeks.org/trie-insert-and-search/>
   [df1]:--
