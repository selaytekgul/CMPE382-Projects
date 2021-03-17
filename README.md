PROJECT NAME: CONFUSING TEXT EDITOR

The aim of the project is to create a confusing text editor which a user can make file operations.
The provided functionality for file operations can be executed both in threaded mode and sequential mode.

In the threaded mode where the given instructions are executed simultaneously file operations are handled by using fork().

In the sequential execution(normal) mode, the instructions are executed one after and other.

The list of the commands are following:

search in a file is provided in two options:

search <keyword> [-c] <inFile>
Searches a given keyword in a given infile and prints everyline where keyword is found. Also, outputs a message which prints the number of occurences of keyword.

search <keyword> <inFile>
Searches a given keyword in a given infile and prints everyline where keyword is found.

make a replacement in a file is provided in two options:

replace replace <targetKeyword> <sourceKeyword> [-c] <inFile>
Searches a given targetKeyword in a given infile and replaces every occurencies of the targetKeyword with the given sourceKeyword before printing everyline where targetKeyword is found and replaced with sourceKeyword. Also, outputs a message which prints the number of replacements done.

replace <targetKeyword> <sourceKeyword> <inFile>
Searches a given targetKeyword in a given infile and replaces every occurencies of the targetKeyword with the given sourceKeyword before printing everyline where targetKeyword is found and replaced with sourceKeyword.

make an insertion of a given keyword in a file is provided in four different options:

insert <insertedkeyword> [-c] <-a OR -b> <targetkeyword> <inFile>
Searches a given targetKeyword in a given infile and puts the insertedKeyword near every occurencies of the targetKeyword according to the after(-a) or before(-b) options before printing everyline where insertedKeyword is put. Also, outputs a message which prints the number of replacements done.

insert <insertedkeyword> <-a OR -b> <targetkeyword> <inFile>
Searches a given targetKeyword in a given infile and puts the insertedKeyword near every occurencies of the targetKeyword according to the after(-a) or before(-b) options before printing everyline where insertedKeyword is put.

counting the lines in a given infile:

lineCount <inFile>
Prints an output message which shows the number of lines in a given infile.

spliting the lines of the infile according to the given characterCount value:

split <CharacterCount> <inFile>
Spliting the lines of the infile according to the given characterCount value different from the functionalities of other functions split command makes an immediate change in the file which is given with the file name infile.

printing some portion of the text to the screen is provided with 3 different options:

head <lineCount> <inFile>
Prints the first lineCount lines to the screen.

tail <lineCount> <inFile>
Prints the first lineCount lines to the screen.

mid <startLine> <endLine> <inFile>
Prints the lines starting from startLine till endLine lines of the infile to the screen.
-Example usage:
prompt-> mid 10 20 try.txt
If 10 and 20 is in the range of the try.txt file, line 10 will be first line printed,
line 20 will be the last line printed.

prompt-> mid 20 20 try.txt
If 20 is in the range of the try.txt file, only line 20 will be printed.

prompt-> mid 30 20 try.txt
Since startLine is greater than endLine, ERROR message will be printed.

AVAILABLE SYMBOLS TO USE BETWEEN INSTRUCTIONS IN ONE LONE

NORMAL MODE:
For the normal mode user can write one or more commands seperated by ":".

THREADED MODE:
For the threaded mode user can write one or more commands seperated by ";".

-However if a user enteres a command which consists of each symbols,
It is not accepted to execute and ERROR message is printed.

-Also, if no more than one command is entered, it will be considered as an instruction which will be executed in normal mode.

OUTPUT SPECIFICATION:
If a user wants the whatever functionality that is provided in an instruction, its output will be printed to the given output file, it is provided as an option.
Any fileName written after ">" symbol will be opened and be used to print the output message.

(However, project is taking the fileName properly but still doesn't have functionality to print the message to another file.)

-Example usage:
prompt-> mid 10 20 try.txt ; head 10 try.txt > out.txt
prompt-> mid 10 20 try.txt : head 10 try.txt > out.txt
prompt-> mid 10 20 try.txt ; head 10 try.txt > out.txt :mid 10 20 try.txt ; head 10 try.txt > out.txt

EXTRA:
-If a user enteres more than 6 words without using special seperators (">",":",";")
Too long command is entered ERROR message will be printed.

-If a user enters meaningles set of words, appropriate helper messages are provided according to the number of the words that is entered.

try.txt file is provided to test the functionality of the file operations.
Since file operations can cause a problem, the appropriate ERROR messages are provided if file cannot opened in any case.

to quit
User enters quit
-Example usage:
prompt->quit


