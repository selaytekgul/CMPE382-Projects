#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <sys/wait.h>

int commandCounter(char *line, char **commands);
int commandErrorFinderChooser(char *line);
int commandNameComparer(char *command);
int ifC(char *miniCommand);
int ifAorB(char *miniCommand);
int forAllTokens(char **tokens, size_t numtokens);
int threadedCaller(char **tokens, size_t numtokens);
char **wannaOut(const char *str, const char *delim, size_t *numtokens);

char **strsplit(const char *str, const char *delim, size_t *numtokens);
void strip_extra_spaces(char *str);
int substring(char *str, char *sub);

void searchC(char *keyword, char *fileName);
int search(char *keyword, char *fileName);
void replaceC(char *target, char *source, char *fileName);
int replace(char *target, char *source, char *fileName);
void insertCa(char *inserted, char *target, char *fileName);
void insertCb(char *inserted, char *target, char *fileName);
int inserta(char *inserted, char *target, char *fileName);
int insertb(char *inserted, char *target, char *fileName);
int lineCount(char *fileName);
void split(char *number, char *fileName);
void head(char *number, char *fileName);
void tail(char *number, char *fileName);
void mid(char *startLine, char *endLine, char *fileName);
void quit();

int commandCounter(char *line, char **commands)
//-----------------------------------------------------
// Summary: finds the number of words entered
// Precondition: line is the string that we scan from the user
// Postcondition: returns the number of words in the command
// if the number of the words in the command is more than 6
// returns -1, commands which is array of string will be filled
// with the words scanned
// Functions called: -
// Used in: commandErrorFinderChooser
//-----------------------------------------------------
{
    char *found;
    line = strsep(&line, "\n");

    strip_extra_spaces(line);
    int i = 0;

    while ((found = strsep(&line, " ")) != NULL)
    {
        if (i < 7)
        {
            // if (strlen(found)!=0)
            if (strcmp(found, "\0") != 0 && strcmp(found, "\n") != 0 && strcmp(found, " ") != 0 && strcmp(found, " \n") != 0)
                commands[i++] = found;
        }
        else
        {
            return -1;
        }
    } //while
    if (i != 0)
    {
        commands[i - 1] = strsep(&commands[i - 1], " \n");
    }
    return i;
} //commandCounter

int commandErrorFinderChooser(char *line)
//-----------------------------------------------------
// Summary: sends the line scanned to the functions
// Precondition: line is the string that we scan from the user
// Postcondition: returns -1 if there is an error in the line
// scanned and prints error messages accordingly
// Functions called: commandCounter,commandNameComparer
// ifC, ifAorB, all the command functions in hw
// Used in: main
//-----------------------------------------------------
{
    char **commands = (char **)(malloc(6 * sizeof(char *)));

    switch (commandCounter(line, commands))
    {
    case -1:
        printf("Too long command");
        return -1;
        break; //-1
    case 0:
        printf("You entered nothing!!");
        return -1;
        break; //0
    case 1:
        if (commandNameComparer(commands[0]) == 9) //QUIT
        {
            quit();
        }
        else
        {
            printf("Only quit command is 1 word!!");
            printf("There is no such a command like %s without args", commands[0]);
        }
        break; //1
    case 2:
        if (commandNameComparer(commands[0]) == 4) //LINECOUNT
        {
            lineCount(commands[1]);
        }
        else
        {
            printf("Only lineCount command contains 2 word!!");
            printf("There is no such a command like %s with 1 arg: %s", commands[0], commands[1]);
        }
        break; //2
    case 3:
        if (commandNameComparer(commands[0]) == 1) //SEARCH
        {
            search(commands[1], commands[2]);
        }
        else if (commandNameComparer(commands[0]) == 5) //SPLIT
        {
            split(commands[1], commands[2]);
        }
        else if (commandNameComparer(commands[0]) == 6) //HEAD
        {
            head(commands[1], commands[2]);
        }
        else if (commandNameComparer(commands[0]) == 7) //TAIL
        {
            tail(commands[1], commands[2]);
        }
        else
        {
            printf("Only search, split, head, tail commands contains 3 word!!");
            printf("There is no such a command like %s with args:  %s %s", commands[0], commands[1], commands[2]);
        }
        break; //3
    case 4:
        if (commandNameComparer(commands[0]) == 1) //SEARCH with [-c]
        {
            if (ifC(commands[2]) == 1)
            {
                searchC(commands[1], commands[3]);
            }
            else
            {
                printf("Did you mean search %s -c %s?", commands[1], commands[3]);
                printf("Try search with -c again!!");
            }
        }
        else if (commandNameComparer(commands[0]) == 2) //REPLACE
        {
            replace(commands[1], commands[2], commands[3]);
        }
        else if (commandNameComparer(commands[0]) == 8) //MID
        {
            mid(commands[1], commands[2], commands[3]);
        }
        else
        {
            printf("Only search with count, mid commands contains 4 word!!");
            printf("There is no such a command like %s with args: %s %s %s", commands[0], commands[1], commands[2], commands[3]);
        }
        break; //4
    case 5:
        if (commandNameComparer(commands[0]) == 2) //REPLACE with [-c]
        {
            if (ifC(commands[3]) == 1)
            {
                replaceC(commands[1], commands[2], commands[4]);
            }
            else
            {
                printf("Did you mean replace %s %s -c %s?", commands[1], commands[2], commands[4]);
                printf("Try replace with -c again!!");
            }
        }
        else if (commandNameComparer(commands[0]) == 3) //INSERT
        {
            if (ifAorB(commands[2]) == 1) //INSERT with "a"
            {
                inserta(commands[1], commands[3], commands[4]);
            }
            else if (ifAorB(commands[2]) == 2) //INSERT with "b"
            {
                insertb(commands[1], commands[3], commands[4]);
            }
            else
            {
                printf("There should be -a or -b to insert a keyword.");
                printf("Did you mean insert %s <-a or -b> %s %s?", commands[1], commands[3], commands[4]);
            }
        }
        else
        {
            printf("Only replace with count, insert commands contains 5 word!!");
            printf("There is no such a command like %s with args: %s %s %s %s", commands[0], commands[1], commands[2], commands[3], commands[4]);
        }
        break; //5
    case 6:
        if (commandNameComparer(commands[0]) == 3) //INSERT with [-c]
            if (ifC(commands[2]) == 1)
            {
                if (ifAorB(commands[3]) == 1) //INSERT with "a"
                {
                    insertCa(commands[1], commands[4], commands[5]);
                }
                else if (ifAorB(commands[3]) == 2) //INSERT with "b"
                {
                    insertCb(commands[1], commands[4], commands[5]);
                }
                else
                {
                    printf("There should be -a or -b to insert a keyword.");
                    printf("Did you mean insert %s %s <-a or -b> %s %s?", commands[1], commands[2], commands[4], commands[5]);
                }
            }
            else
            {
                printf("Did you mean insert %s -c %s %s %s?", commands[1], commands[3], commands[4], commands[5]);
                printf("Try replace with -c again!!");
            }
        else
        {
            printf("Only insert with count command contains 6 word!!");
            printf("There is no such a command like %s with args: %s %s %s %s %s", commands[0], commands[1], commands[2], commands[3], commands[4], commands[5]);
        }
        break; //6
    }          //switch
} //commandErrorFinderChooser

int ifAorB(char *miniCommand)
//-----------------------------------------------------
// Summary: compares the given string with "-a" or "-b"
// Precondition: only one string should be given
// Postcondition: returns 1 if "-a", 2 if "-b" else -1
// Functions called: -
// Used in: commandErrorFinderChooser
//-----------------------------------------------------
{
    if (strncmp(miniCommand, "-a", 2) == 0)
    {
        return 1;
    }
    else if (strncmp(miniCommand, "-b", 2) == 0)
    {
        return 2;
    }
    else
        return -1;
} //ifAorB

int ifC(char *miniCommand)
//-----------------------------------------------------
// Summary: compares the given string with "-c"
// Precondition: only one string should be given
// Postcondition: returns 1 if equal, else -1
// Functions called: -
// Used in: commandErrorFinderChooser
//-----------------------------------------------------
{
    if (strncmp(miniCommand, "-c", 2) == 0)
    {
        return 1;
    }
    else
        return -1;
} //ifC

int commandNameComparer(char *command)
//-----------------------------------------------------
// Summary: compares the string with the valid commands
// Precondition: only one string should be given
// Postcondition: returns an int acc. to the command
// returns -1 if there is no defined command
// Functions called: -
// Used in: commandErrorFinderChooser
//-----------------------------------------------------
{
    if (strncmp(command, "search", 6) == 0)
    {
        return 1;
    }
    else if (strncmp(command, "replace", 7) == 0)
    {
        return 2;
    }
    else if (strncmp(command, "insert", 6) == 0)
    {
        return 3;
    }
    else if (strncmp(command, "lineCount", 9) == 0)
    {
        return 4;
    }
    else if (strncmp(command, "split", 5) == 0)
    {
        return 5;
    }
    else if (strncmp(command, "head", 4) == 0)
    {
        return 6;
    }
    else if (strncmp(command, "tail", 4) == 0)
    {
        return 7;
    }
    else if (strncmp(command, "mid", 3) == 0)
    {
        return 8;
    }
    else if (strncmp(command, "quit", 4) == 0)
    {
        return 9;
    }
    else
    {
        return -1;
    }
} //commandNameComparer

void searchC(char *keyword, char *fileName)
//-----------------------------------------------------
// Summary: searches & counts the keyword in a file
// Precondition: only keyword string and fileName should
// be given
// Postcondition: returns nothing but prints error message
// if the file with given fileName couldn't opened
// Functions called: search
// Used in: commandErrorFinderChooser
//-----------------------------------------------------
{
    printf("Number of found %s is: %d.", keyword, search(keyword, fileName));
} // searchC

int search(char *keyword, char *fileName)
//-----------------------------------------------------
// Summary: searches the keyword in a file
// Precondition: only keyword string and fileName should
// be given
// Postcondition: returns number of occurences
// and prints error message if the file with given
// fileName couldn't opened
// Functions called: - fopen, fclose
// Used in: commandErrorFinderChooser
//-----------------------------------------------------
{
    FILE *filePtr = fopen(fileName, "r");
    if (filePtr == 0)
    {
        printf("File %s couldn't opened!", fileName);
        return -1;
    }
    else
    { //file opened
        char *line = NULL, *temp, *tempLine;
        size_t len = 0, nread;
        int count = 0;

        while ((nread = getline(&line, &len, filePtr) != -1))
        {
            tempLine = malloc(len + 2);
            strcpy(tempLine, line);
            while (tempLine != NULL)
            {
                temp = strsep(&tempLine, " ");
                if (strstr(temp, keyword) != NULL)
                {
                    count++;

                    printf("%d.occurance of %s in file\n--%s", count, keyword, line);
                }
            }
        } //while

        free(line);
        fclose(filePtr);
        return count;
    }

} //search

void replaceC(char *target, char *source, char *fileName)
//-----------------------------------------------------
// Summary: replaces the target with source in a file
// Precondition: only target, source string and fileName
// should be given
// Postcondition: returns nothing but prints error message
// if the file with given fileName couldn't opened
// Functions called: replace
// Used in: commandErrorFinderChooser
//-----------------------------------------------------
{
    printf("The number of changed %s into %s is: %d.", target, source, replace(target, source, fileName));
} //replaceC

int replace(char *target, char *source, char *fileName)
//-----------------------------------------------------
// Summary: replaces the target with source in a file
// Precondition: only target, source string and fileName
// should be given
// Postcondition: returns number of replacements done or -1
// and prints error message if the file with given
// fileName couldn't opened
// Functions called: - fopen, fclose, getLine
// Used in: commandErrorFinderChooser, replaceC
//-----------------------------------------------------
{
    FILE *filePtr = fopen(fileName, "r");
    if (filePtr == 0)
    {
        printf("File %s couldn't opened!", fileName);
        return -1;
    }
    else
    { //file opened
        char *line = NULL, *word, *tempLine;
        size_t len = 0, nread;
        int count = 0;

        while ((nread = getline(&line, &len, filePtr) != -1))
        {
            tempLine = malloc(len + (sizeof(source)));
            strcpy(tempLine, "");
            while (line != NULL)
            {
                word = strsep(&line, " ");

                if (strstr(word, target) != NULL)
                {
                    strcat(tempLine, source);
                    strcat(tempLine, " ");
                    count++;

                    printf("%d.occurance of %s in file\n--%s", count, target, line);
                }
                else
                {
                    strcat(tempLine, word);
                    strcat(tempLine, " ");
                }
            }
            printf("%s", tempLine);
        } //while
        free(line);
        fclose(filePtr);
        return count;
    } //file opened properly

} //replace

void insertCa(char *inserted, char *target, char *fileName)
{
    printf("The number of inserted %s after %s is: %d.", inserted, target, inserta(inserted, target, fileName));
} //insertCa

void insertCb(char *inserted, char *target, char *fileName)
//-----------------------------------------------------
// Summary: insertes the inserted after target in a file
// Precondition: only inserted, target string and fileName
// should be given
// Postcondition: returns number of insertions done
// and prints error message if the file with given
// fileName couldn't opened
// Functions called: insertb
// Used in: commandErrorFinderChooser
//-----------------------------------------------------
{
    printf("The number of inserted %s before %s is: %d.", inserted, target, insertb(inserted, target, fileName));
} //insertCb

int inserta(char *inserted, char *target, char *fileName)
//-----------------------------------------------------
// Summary: insertes the inserted after target in a file
// Precondition: only inserted, target string and fileName
// should be given
// Postcondition: returns number of insertions done or -1
// and prints error message if the file with given
// fileName couldn't opened
// Functions called: - fopen, fclose, getLine
// Used in: commandErrorFinderChooser, insertCa
//-----------------------------------------------------
{
    FILE *filePtr = fopen(fileName, "r");
    if (filePtr == 0)
    {
        printf("File %s couldn't opened!", fileName);
        return -1;
    }
    else
    { //file opened
        char *line = NULL, *word, *tempLine;
        size_t len = 0, nread; // just for getLine
        int count = 0;
        while ((nread = getline(&line, &len, filePtr) != -1))
        {
            if (line == NULL)
            {
                count = count;
            }
            else
            {
                tempLine = malloc(len + (sizeof(inserted) + sizeof(target)));
                strcpy(tempLine, "");
                while (line != NULL)
                {
                    word = strsep(&line, " ");
                    if (strstr(word, target) != NULL)
                    {
                        strcat(tempLine, target);
                        strcat(tempLine, " ");
                        strcat(tempLine, inserted); //NEWLY ADDED
                        strcat(tempLine, " ");
                        count++;
                        printf("%d.occurance of %s in file\n--%s", count, target, line);
                    }
                    else
                    {
                        strcat(tempLine, word);
                        strcat(tempLine, " ");
                    }
                }
                printf("%s", tempLine);
            } //else line not NULL
        }     //while
        free(line);
        fclose(filePtr);
        return count;
    } // file opened properly
    // } // inserta

} // inserta

int insertb(char *inserted, char *target, char *fileName)
//-----------------------------------------------------
// Summary: insertes the inserted before target in a file
// Precondition: only inserted, target string and fileName
// should be given
// Postcondition: returns number of insertions done or -1
// and prints error message if the file with given
// fileName couldn't opened
// Functions called: - fopen, fclose, getLine
// Used in: commandErrorFinderChooser, insertCb
//-----------------------------------------------------
{
    FILE *filePtr = fopen(fileName, "r");
    if (filePtr == 0)
    {
        printf("File %s couldn't opened!", fileName);
        return -1;
    }
    else
    { //file opened
        char *line = NULL, *word, *tempLine;
        size_t len = 0, nread; // just for getLine
        int count = 0;
        while ((nread = getline(&line, &len, filePtr) != -1))
        {
            tempLine = malloc(len + (sizeof(inserted) + sizeof(target)));

            strcpy(tempLine, "");
            while (line != NULL)
            {
                word = strsep(&line, " ");

                if (strstr(word, target) != NULL)
                {
                    strcat(tempLine, inserted); //NEWLY ADDED
                    strcat(tempLine, " ");

                    strcat(tempLine, target);
                    strcat(tempLine, " ");

                    count++;

                    printf("%d.occurance of %s in file\n--%s", count, target, line);
                }
                else
                {
                    strcat(tempLine, word);
                    strcat(tempLine, " ");
                }
            }
            printf("%s", tempLine);
        } //while
        free(line);
        fclose(filePtr);
        return count;
    }
} //insertb

int lineCount(char *fileName)
//-----------------------------------------------------
// Summary: prints the number of lines in a file
// Precondition: only one string should be given as a
// fileName
// Postcondition: returns nothing but prints error message
// if the file with given fileName couldn't opened
// Functions called: - fopen,fclose,fseek
// Used in: commandErrorFinderChooser, head, tail, mid
//-----------------------------------------------------
{
    FILE *filePtr = fopen(fileName, "r");
    if (filePtr == 0)
    {
        printf("File %s couldn't opened!", fileName);
        return -1;
    }
    else
    { //file opened
        int count = 0;
        char c;
        fseek(filePtr, 0, SEEK_SET);
        for (c = getc(filePtr); c != EOF; c = getc(filePtr))
        {
            if (c == '\n') // Increment count if this character is newline
            {
                count++;
            }
        }
        printf("File %s has %d lines.\n", fileName, ++count);
        fclose(filePtr);
        return count;
    }
} //lineCount

void split(char *number, char *fileName)
//-----------------------------------------------------
// Summary: prints the number of lines in a file
// Precondition: one string as a fileName and one string
// which can be converted to an integer should be given
// Postcondition: returns nothing but prints error message
// if the limit is not a number or a negative number or
// if the file with given fileName couldn't opened
// Functions called: - fopen,fclose,fseek,getc
// Used in: commandErrorFinderChooser
//-----------------------------------------------------
{
    int limit = atoi(number);
    if (limit <= 0)
    {
        printf("You didn't enter a valid number for split!");
    }
    else
    { //valid number entered
        char *fileName2 = "output.txt";
        FILE *filePtr = fopen(fileName, "r"); //input file will be read
        // and
        FILE *filePtr2 = fopen(fileName2, "w"); //output file will be filled with arranged data of input file

        if (filePtr == 0)
        {
            printf("File %s couldn't opened!", fileName);
        }
        else //file opened
        {
            int count = 0;
            char c;
            fseek(filePtr, 0, SEEK_SET);
            fseek(filePtr2, 0, SEEK_SET);

            for (c = getc(filePtr); c != EOF; c = getc(filePtr))
            {
                count++;

                if (count <= limit)
                {
                    fprintf(filePtr2, "%c", c);
                }
                else
                {
                    if (c != '\n')
                    {
                        fprintf(filePtr2, "%c", '\n');
                    }
                    else
                    {
                        fprintf(filePtr2, "%c", c);
                    }

                    count = 1;
                }
            }
            fclose(filePtr);
            fclose(filePtr2);

            FILE *filePtr3 = fopen(fileName, "w"); //input file will be filled again
            //according to the
            FILE *filePtr4 = fopen(fileName2, "r"); //newly arranged output file will read now
            fseek(filePtr3, 0, SEEK_SET);
            fseek(filePtr4, 0, SEEK_SET);

            for (c = getc(filePtr4); c != EOF; c = getc(filePtr4))
            {
                fprintf(filePtr3, "%c", c);
            }
            fclose(filePtr3);
            fclose(filePtr4);

            printf("File %s has %d chars per line.", fileName, count); //kontrol "sil"

        } //file opened properly

    } // limit is specified well

} //split

void head(char *number, char *fileName)
//-----------------------------------------------------
// Summary: prints the first number of lines in a file
// Precondition: one string as a fileName and one string
// which can be converted to an integer should be given
// Postcondition: returns nothing but prints error message
// if the limit is not a number or a negative number or
// if the file with given fileName couldn't opened
// Functions called: lineCount fopen,fclose,fseek,getLine
// Used in: commandErrorFinderChooser
//-----------------------------------------------------
{
    int limit = atoi(number);
    if (limit <= 0)
    {
        printf("You didn't enter a valid number for head!");
    }
    else
    { //valid number entered
        FILE *filePtr = fopen(fileName, "r");

        if (filePtr == 0)
        {
            printf("File %s couldn't opened!", fileName);
        }
        else
        { //file opened
            fseek(filePtr, 0, SEEK_SET);
            int lineCountOriginal = lineCount(fileName);
            if (limit > lineCountOriginal)
            {
                printf("You wanted first %d lines, in the file %s we have only %d lines.", limit, fileName, lineCountOriginal);
                printf("\nWe are printing all we have!!\n");
            }
            char *line = NULL;
            size_t len = 0, nread; //just for the getLine
            int count = 0;
            while ((nread = getline(&line, &len, filePtr) != -1))
            {
                if (line == NULL)
                {
                    count = count;
                }
                else
                {
                    if (count < limit)
                    {
                        count++;
                        printf("%s", line);
                    }
                }
            } //while
            printf("\nFirst %d lines printed above.", count);
            free(line);
            fclose(filePtr);
        } // file opened properly
    }     //valid number entered
} //head

void tail(char *number, char *fileName)
//-----------------------------------------------------
// Summary: prints the last number of lines in a file
// Precondition: one string as a fileName and one string
// which can be converted to an integer should be given
// Postcondition: returns nothing but prints error message
// if the limit is not a number or a negative number or
// if the file with given fileName couldn't opened
// Functions called: lineCount fopen,fclose,fseek,getLine
// Used in: commandErrorFinderChooser
//-----------------------------------------------------
{
    int limit = atoi(number);
    if (limit <= 0)
    {
        printf("You didn't enter a valid number for tail!");
    }
    else
    { //valid number entered
        FILE *filePtr = fopen(fileName, "r");

        if (filePtr == 0)
        {
            printf("File %s couldn't opened!", fileName);
        }
        else
        { //file opened
            fseek(filePtr, 0, SEEK_SET);

            int lineCountOriginal = lineCount(fileName);
            if (limit > lineCountOriginal)
            {
                printf("You wanted first %d lines, in the file %s we have only %d lines.", limit, fileName, lineCountOriginal);
                printf("\nWe are printing all we have!!\n");
            }
            int lineAt = 1;
            char *line = NULL;
            size_t len = 0, nread; //just for the getLine
            int count = 0;
            int start = lineCountOriginal - limit;
            // fseek(filePtr, 0, SEEK_SET);

            while ((nread = getline(&line, &len, filePtr) != -1))
            {
                if (line == NULL)
                {
                    count = count;
                }
                else
                {
                    if (lineAt > start && lineAt <= lineCountOriginal)
                    {
                        count++;
                        printf("%s", line);
                    }
                    lineAt++;
                }
            } //while
            printf("\nLast %d lines printed above.", count);
            free(line);
            fclose(filePtr);
        } // file opened properly
    }     //valid number entered
} // tail

void mid(char *startLine, char *endLine, char *fileName)
//-----------------------------------------------------
// Summary: prints the last number of lines in a file
// Precondition: one string as a fileName and one string
// which can be converted to an integer should be given
// Postcondition: returns nothing but prints error message
// if the limit is not a number or a negative number or
// if the file with given fileName couldn't opened
// Functions called: lineCount fopen,fclose,fseek,getLine
// Used in: commandErrorFinderChooser
//-----------------------------------------------------
{
    int startNumber = atoi(startLine);
    int endNumber = atoi(endLine);
    if (startNumber > 0 && endNumber > 0)
    { //valid number entered
        FILE *filePtr = fopen(fileName, "r");

        if (filePtr == 0)
        {
            printf("File %s couldn't opened!", fileName);
        }
        else
        { //file opened
            fseek(filePtr, 0, SEEK_SET);

            if (startNumber <= endNumber)
            {
                int lineCountOriginal = lineCount(fileName);

                if (endNumber <= lineCountOriginal)
                {
                    int lineAt = 1;
                    char *line = NULL;
                    size_t len = 0, nread; //just for the getLine
                    int count = 0;         //how many lines are printed
                    // fseek(filePtr, 0, SEEK_SET);

                    while ((nread = getline(&line, &len, filePtr) != -1))
                    {
                        if (line == NULL)
                        {
                            count = count;
                        }
                        else
                        {
                            if (lineAt >= startNumber && lineAt <= endNumber)
                            {
                                count++;
                                printf("%s", line);
                            }
                            lineAt++;
                        }
                    } //while
                    printf("\n%d lines printed above.", count);
                    free(line);
                    fclose(filePtr);
                } //proper limits for the number of lines in the file
                else
                { //unfoutunatelly upper limits are not appropriate for the file
                    printf("The limit you gave %d - %d is not provided.\n", startNumber, endNumber);
                    printf("You can give numbers upTo %d for the file %s", lineCountOriginal, fileName);
                }

            } //startNumber <= endNumber as wanted
            else
            {
                printf("StartNumber %d is greater than EndNumber %d", startNumber, endNumber);
            }
        } // file opened properly
    }     //valid number entered
    else
    {
        printf("You didn't enter a valid number for startLine or endLine!");
    }

} //mid

void quit()
{
    printf("quited");
    exit(0);
}

void strip_extra_spaces(char *text)
//-----------------------------------------------------
// Summary: gets rid of the multiple spaces
// Precondition: only one string should be given
// Postcondition: returns nothing but text arg will be fixed
// Functions called: -
// Source: https://www.programmingsimplified.com/c/source-code/c-program-remove-spaces-string
//-----------------------------------------------------
{
    int length, c, d;
    char *start;

    c = d = 0;

    length = strlen(text);

    start = (char *)malloc(length + 1);

    if (start == NULL)
        exit(EXIT_FAILURE);

    while (*(text + c) != '\0')
    {
        if (*(text + c) == ' ')
        {
            int temp = c + 1;
            if (*(text + temp) != '\0')
            {
                while (*(text + temp) == ' ' && *(text + temp) != '\0')
                {
                    if (*(text + temp) == ' ')
                    {
                        c++;
                    }
                    temp++;
                }
            }
        }
        *(start + d) = *(text + c);
        c++;
        d++;
    }
    *(start + d) = '\0';
    text = start;
} //strip_extra_spaces

char **strsplit(const char *str, const char *delim, size_t *numtokens)
//-----------------------------------------------------
// Summary: the most amazing split code according to the delimiter
// https://www.quora.com/How-do-you-write-a-C-program-to-split-a-string-by-a-delimiter-I-think-it-uses-ktochar
//-----------------------------------------------------
{
    // copy the original string so that we don't overwrite parts of it
    // (don't do this if you don't need to keep the old line,
    // as this is less efficient)
    char *s = strdup(str);
    // these three variables are part of a very common idiom to
    // implement a dynamically-growing array
    size_t tokens_alloc = 1;
    size_t tokens_used = 0;
    char **tokens = calloc(tokens_alloc, sizeof(char *));
    char *token, *strtok_ctx;
    for (token = strtok_r(s, delim, &strtok_ctx);
         token != NULL;
         token = strtok_r(NULL, delim, &strtok_ctx))
    {
        // check if we need to allocate more space for tokens
        if (tokens_used == tokens_alloc)
        {
            tokens_alloc *= 2;
            tokens = realloc(tokens, tokens_alloc * sizeof(char *));
        }
        tokens[tokens_used++] = strdup(token);
    }
    // cleanup
    if (tokens_used == 0)
    {
        free(tokens);
        tokens = NULL;
    }
    else
    {
        tokens = realloc(tokens, tokens_used * sizeof(char *));
    }
    *numtokens = tokens_used;
    free(s);
    return tokens;
} //strsplit

int substring(char *str, char *sub)
//-----------------------------------------------------
// Summary: if substring
// Precondition: 2 strings given
// Postcondition: returns 1 if sub is in str
// returns -1 if sub is not in str
// Functions called: -
// Used in: whichMode
//-----------------------------------------------------
{

    int i, j = 0, k;
    for (i = 0; str[i]; i++)
    {
        if (str[i] == sub[j])
        {
            for (k = i, j = 0; str[k] && sub[j]; j++, k++)
                if (str[k] != sub[j])
                    break;
            if (!sub[j])
            {
                return 1;
            }
        }
    }
    return -1;
} //substring

void whichMode(char *line)
//-----------------------------------------------------
// Summary: finds whichMode
// Precondition: the given command is initial entered by user
// Postcondition: calls normal mode or threaded mode
// Functions called: substring
// Used in: main
//-----------------------------------------------------
{
    if ((substring(line, ":") == 1) && (substring(line, ";") == 1))
    {
        printf("\nMAKE YOUR DECISION\n");
        printf("\nCHOOSE YOUR SIDE!\n");
        printf("\neither THREADED or NORMAL\n");
    }
    else if (substring(line, ":") == 1)
    {
        printf("\nNORMAL MODE:\n");
        printf("\nINSTRUCTIONS ARE EXECUTING SEQUENTIALLY...\n");

        size_t howManyCommandsWillBeInQueue;
        char **commands = strsplit(line, ":", &howManyCommandsWillBeInQueue);
        forAllTokens(commands, howManyCommandsWillBeInQueue);
    }
    else if (substring(line, ";") == 1)
    {
        printf("\nTHREADED MODE:\n");
        printf("\nINSTRUCTIONS ARE EXECUTING SIMULTANEOUSLY...\n");

        size_t howManyCommandsWillBeInQueue;
        char **commands = strsplit(line, ";", &howManyCommandsWillBeInQueue);
        threadedCaller(commands, howManyCommandsWillBeInQueue);

        // forAllTokens(commands, howManyCommandsWillBeInQueue);
    }
    else //again normal mode
    {
        printf("\nNORMAL MODE:\n");
        printf("\nYou have one request, I will do my best!\n");

        size_t howManyCommandsWillBeInQueue;
        char **commands = strsplit(line, ":", &howManyCommandsWillBeInQueue);
        forAllTokens(commands, howManyCommandsWillBeInQueue);
    }
}

// char **wannaOut(char *line, int *out)
// //-----------------------------------------------------
// // Summary: makes the command seperated with the out file
// // if there is any (by using >out.txt)
// // Precondition: the given line command is initial entered by
// // user out will be filled acc. to the existance of ">"
// // Postcondition: returns a string array
// // first string will be the command wanted
// // second string will be the fileName if stated
// // Functions called: substring, strsplit
// // Used in: threadedCaller, forAllTokens
// //-----------------------------------------------------
// {
//     char **tokens =(char **) calloc(1, sizeof(char *));
//     size_t num;
//     if (substring(line, ">") == 1) //there is at least one ">"
//     {
//         tokens = strsplit(line, ">", &num); //MOST AMAZING FUNCTION IS CALLED
//         if (num > 1)                        // more than 1 occurences of ">"
//         {
//             *out = -1;   //soo many ">"
//             return NULL; //nothing to send to CommandErrorFinderChooser
//         }                // there are more than wanted ">"
//         else             // 1 ">"
//         {
//             *out = 1;
//             return tokens;
//         } // 1 ">"

//     } // we have ">" in the line
//     else
//     { //no >
//         *out = 1;
//         return tokens;
//     } //no >

// } //wannaOut

int threadedCaller(char **tokens, size_t numtokens)
//-----------------------------------------------------
// Summary: for a line which can be consists of more
// than one commands to be executed, it runs instructions
// sequentially (normal mode)
// Precondition: commands given as an array
// Postcondition: after execution is returns 1
// Functions called: commandErrorFinderChooser, wannaOut
// Used in: whichMode
//-----------------------------------------------------
{
    pid_t pid;

    for (size_t i = 0; i < numtokens; i++)
    {
        pid = fork();

        // printf("\nExecuting: %s \n", tokens[i]);

        if (pid > 0)
        {
            printf("\nPARENT :%d\n", pid);
            printf("\ngetpit :%d\n", getpid());
            //wait for child to finish
            wait(NULL);
        }
        if (pid == 0)
        {
            //call the input function
            printf("\nExecuting: %s \n", tokens[i]);

            size_t outFileRequested = 0;
            char **request = wannaOut(tokens[i], ">", &outFileRequested);

            switch (outFileRequested)
            {

            case 2:
                printf("The file %s you requested will be ready!\n", request[1]);
                commandErrorFinderChooser(request[0]);

                break;

            case 1:
                printf("All you need is (prompt)!\n");
                commandErrorFinderChooser(request[0]);

                break;

            default:
                printf("What a -meaningless- command!\n");
                printf("You can only use one >.");
                break;

            } // switch

            // commandErrorFinderChooser(tokens[i]);
            printf("\nCHILD :%d\n", getpid());

            printf("\ngetppit :%d\n", getppid());
        }
        if (getpid() < 0)
        {
            printf("\nError!!!\n");
        }
    }
    free(tokens);
    return 1;
} //threadedCaller

// int threadedCaller(char **tokens, size_t numtokens)
// //-----------------------------------------------------
// // Summary: for a line which can be consists of more
// // than one commands to be executed, it runs instructions
// // sequentially (normal mode)
// // Precondition: commands given as an array
// // Postcondition: after execution is returns 1
// // Functions called: commandErrorFinderChooser
// // Used in: whichMode
// //-----------------------------------------------------
// {
//     pid_t pid;

//     for (size_t i = 0; i < numtokens; i++)
//     {
//         pid = fork();

//         // printf("\nExecuting: %s \n", tokens[i]);

//         if (pid > 0)
//         {
//             printf("PARENT :%d", pid);
//             printf("getpit :%d", getpid());
//             //wait for child to finish
//             wait(NULL);
//         }
//         if (pid == 0)
//         {
//             //call the input function
//             printf("\nExecuting: %s \n", tokens[i]);
//             commandErrorFinderChooser(tokens[i]);
//             printf("pidpidpid :%d\n", getpid());

//             printf("getppit :%d\n", getppid());
//         }
//         if (getpid() < 0)
//         {
//             printf("errorooooo");
//         }
//     }
//     free(tokens);
//     return 1;
// } //threadedCaller

int forAllTokens(char **tokens, size_t numtokens)
//-----------------------------------------------------
//
// Summary: for a line which can be consists of more
// than one commands to be executed, it runs instructions
// sequentially (normal mode)
// Precondition: commands given as an array
// Postcondition: after execution is returns 1
// Functions called: commandErrorFinderChooser
// Used in: whichMode
//-----------------------------------------------------
{
    for (size_t i = 0; i < numtokens; i++)
    {
        printf("\nExecuting: %s \n", tokens[i]);
        // commandErrorFinderChooser(tokens[i]);

        size_t outFileRequested = 0;
        char **request = wannaOut(tokens[i], ">", &outFileRequested);

        switch (outFileRequested)
        {

        case 2:
            printf("The file %s you requested will be ready!\n", request[1]);
            commandErrorFinderChooser(request[0]);

            break;

        case 1:
            printf("All you need is (prompt)!\n");
            commandErrorFinderChooser(request[0]);

            break;

        default:
            printf("What a -meaningless- command!\n");
            printf("You can only use one >.");

            break;

        } // switch

    } // for
    free(tokens);
    return 1;
} //forAllTokens


char **wannaOut(const char *str, const char *delim, size_t *numtokens)
//-----------------------------------------------------
// Summary: the most amazing split code just to be used in checking delimiter ">"
// https://www.quora.com/How-do-you-write-a-C-program-to-split-a-string-by-a-delimiter-I-think-it-uses-ktochar
//-----------------------------------------------------
{
    // copy the original string so that we don't overwrite parts of it
    // (don't do this if you don't need to keep the old line,
    // as this is less efficient)
    char *s = strdup(str);
    // these three variables are part of a very common idiom to
    // implement a dynamically-growing array
    size_t tokens_alloc = 1;
    size_t tokens_used = 0;
    char **tokens = calloc(tokens_alloc, sizeof(char *));
    char *token, *strtok_ctx;
    for (token = strtok_r(s, delim, &strtok_ctx);
         token != NULL;
         token = strtok_r(NULL, delim, &strtok_ctx))
    {
        // check if we need to allocate more space for tokens
        if (tokens_used == tokens_alloc)
        {
            tokens_alloc *= 2;
            tokens = realloc(tokens, tokens_alloc * sizeof(char *));
        }
        tokens[tokens_used++] = strdup(token);
    }
    // cleanup
    if (tokens_used == 0)
    {
        free(tokens);
        tokens = NULL;
    }
    else
    {
        tokens = realloc(tokens, tokens_used * sizeof(char *));
    }
    *numtokens = tokens_used;
    free(s);
    return tokens;
} //wannaOut

int main(int argc, char* argv)
{
    char line[100];

    while (1)
    {
        printf("\nprompt>");
        fgets(line, sizeof(line), stdin);
        whichMode(line);

    } //while

    return 0;
}
