#define _GNU_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<strings.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<unistd.h>
#include<getopt.h>
#include<dirent.h>
#include<sys/types.h>
#include<errno.h>
#include "functions.h"
		
char *uppertolowercase(char *string) {
	int i;
	for(i = 0; i < strlen(string); i++) {
		if(string[i] >= 65 && string[i] <= 90)
			string[i] = string[i] + 32;
	}
	return string;
}

void highlight(char *word, char *line) {
	int h, i =0, j = strlen(word);
	char *str;
	str = strstr(line, word);
	h = strlen(str);
	h = strlen(line) - h;
	while(line[i] != '\0') {
		if(i == h) {
			while(j) {
				red();
				printf("%c", line[i]);
				reset();
				j--;
				i++;
			}
		}
		else {
			printf("%c", line[i]);
			i++;
		}
	}
	printf("\n");
}

int readline(int fd, char *line) {
    int i =0;
    char ch;
    while(read(fd, &ch, 1) ) {
    	if(ch != '\n')
			line[i++] = ch;
		else if(ch == '\n') {
			line[i] = '\0';
			return i + 1;
		}
	}
	line[i] = '\0';
    return i;    
}

int dotpresent(char *str) {
	int i;
	for(i = 0; i < strlen(str); i++) {
		if(str[i] == '.') {
			return 1;
		}
	}
	return 0;
}

int dashpresent(char *str) {
	int i;
	for(i = 0; i < strlen(str); i++) {
		if(str[i] == '-')
			return 1;
	}
	return 0;
}


void red () {
  printf("\033[1;31m");
}


void reset () {
  printf("\033[0m");
}

void purple() {
	printf("\033[0;35m");
}

void blue() {
	printf("\033[0;34m");
}

void yellow() {
	printf("\033[0;33m");
}

void green() {
	printf("\033[0;32m");
}


int find(char *word, char *line, int casesen, int wordfind) {
	int i, j, k;
	char str[500][205];
	if((casesen == 0) && (wordfind == 0)) {
		if((strstr(line, word)) != NULL) 
			return 1;
		else
			return 0;
	}
	
	if((casesen) && !(wordfind)) {
		
		if(strcasestr(line, word) != NULL) {
			return 1;
		}
		else 
			return 0;
	}
	
	if(!(casesen) && (wordfind)) {
			i = 0;
   	 		j = 0;
   	 		k = 0;
   	 		while(i < strlen(line) + 1) {
   	 			 if((line[i] >= 65 && line[i] <=90) || (line[i] >= 97 && line[i] <=122)) {
   	 				str[j][k] = line[i];
   	 				i++;
   	 				k++;
   	 				
   	 			}
   	 			else {
   	 				str[j][k] = '\0';
   	 				j++;
   	 				i++;
   	 				k = 0;
   	 			}
   	 		}
   	 		for(i = 0; i < j; i++) {
   	 			if(strcmp(word, str[i]) == 0)
   	 				return 1;
   	 		}
   	 		return 0;
   	 }
	
	if(casesen && wordfind) {
		char lowercaseline[1028];
		i = 0; j = 0; k = 0;
		strcpy(lowercaseline, line);
		strcpy(word, uppertolowercase(word));
		strcpy(lowercaseline, uppertolowercase(lowercaseline));
		
		
		while(i < strlen(lowercaseline) + 1) {
   	 			if((lowercaseline[i] >= 97 && lowercaseline[i] <= 122)) {
   	 				str[j][k] = lowercaseline[i];
   	 				i++;
   	 				k++;
   	 			}
   	 			else {
   	 				str[j][k] = '\0';
   	 				j++;
   	 				i++;
   	 				k = 0;
   	 			}
   	 		}
   	 		for(i = 0; i < j; i++) {
   	 			if(strcmp(word, str[i]) == 0)
   	 				return 1;
   	 		}
   	 		return 0;
   	 }
}

void bprint(int k) {
	green();
	printf("%d", k);
	blue();
	printf(":");
	reset();
}

void Hprint(char *filename) {
	purple();
    printf("%s", filename);
    blue();
    printf(":");
    reset();
}	


void printhelp() {
		printf("Usage: grep [OPTION]... PATTERNS [FILE]...\n");
		printf("Search for PATTERNS in each FILE.\n");
		printf("Example: grep -i 'hello world' menu.h main.c\n");
		printf("PATTERNS can contain multiple patterns separated by newlines.\n\n");

		printf("Pattern selection and interpretation:\n");	
		printf("\t-E, --extended-regexp     PATTERNS are extended regular expressions\n");
		printf("\t-F, --fixed-strings       PATTERNS are strings\n");
		printf("\t-G, --basic-regexp        PATTERNS are basic regular expressions\n");
  		printf("\t-P, --perl-regexp         PATTERNS are Perl regular expressions\n");
  		printf("\t-e, --regexp=PATTERNS     use PATTERNS for matching\n");
  		printf("\t-f, --file=FILE           take PATTERNS from FILE\n");
  		printf("\t-i, --ignore-case         ignore case distinctions\n");
  		printf("\t-w, --word-regexp         match only whole words\n");
  		printf("\t-x, --line-regexp         match only whole lines\n");
  		printf("\t-z, --null-data           a data line ends in 0 byte, not newline\n\n");

		printf("Miscellaneous:\n");
  		printf("\t-s, --no-messages         suppress error messages\n");
  		printf("\t-v, --invert-match        select non-matching lines\n");
  		printf("\t-V, --version             display version information and exit\n");
      	printf("\t--help                display this help text and exit\n\n");

		printf("Output control:\n");
  		printf("\t-m, --max-count=NUM       stop after NUM selected lines\n");
  		printf("\t-b, --byte-offset         print the byte offset with output lines\n");
  		printf("\t-n, --line-number         print line number with output lines\n");
      	printf("\t--line-buffered       flush output on every line\n");
  		printf("\t-H, --with-filename       print file name with output lines\n");
  		printf("\t-h, --no-filename         suppress the file name prefix on output\n");
      	printf("\t\t--label=LABEL         use LABEL as the standard input file name prefix\n");
  		printf("\t-o, --only-matching       show only nonempty parts of lines that match\n");
  		printf("\t-q, --quiet, --silent     suppress all normal output\n");
      	printf("\t--binary-files=TYPE   assume that binary files are TYPE;\n");
        printf("\t\t\t\tTYPE is 'binary', 'text', or 'without-match'\n");
  		printf("\t-a, --text                equivalent to --binary-files=text\n");
  		printf("\t-I                        equivalent to --binary-files=without-match\n");
  		printf("\t-d, --directories=ACTION  how to handle directories;\n");
        printf("	                    ACTION is 'read', 'recurse', or 'skip'\n");
  		printf("\t-D, --devices=ACTION      how to handle devices, FIFOs and sockets;\n");
        printf("	                    ACTION is 'read' or 'skip'\n");
  		printf("\t-r, --recursive           like --directories=recurse\n");
  		printf("\t-R, --dereference-recursive  likewise, but follow all symlinks\n");
      	printf("\t--include=GLOB        search only files that match GLOB (a file pattern)\n");
      	printf("\t--exclude=GLOB        skip files and directories matching GLOB\n");
      	printf("\t--exclude-from=FILE   skip files matching any file pattern from FILE\n");
      	printf("\t--exclude-dir=GLOB    skip directories that match GLOB\n");
  		printf("\t-L, --files-without-match  print only names of FILEs with no selected lines\n");
  		printf("\t-l, --files-with-matches  print only names of FILEs with selected lines\n");
 		printf("\t-c, --count               print only a count of selected lines per FILE\n");
  		printf("\t-T, --initial-tab         make tabs line up (if needed)\n");
  		printf("\t-Z, --null                print 0 byte after FILE name\n\n");

		printf("Context control:\n");
  		printf("\t-B, --before-context=NUM  print NUM lines of leading context\n");
  		printf("\t-A, --after-context=NUM   print NUM lines of trailing context\n");
  		printf("\t-C, --context=NUM         print NUM lines of output context\n");
  		printf("\t-NUM                      same as --context=NUM\n");
      	printf("\t--color[=WHEN],\n");
      	printf("\t--colour[=WHEN]       use markers to highlight the matching strings;\n");
        printf("\t		                    WHEN is 'always', 'never', or 'auto'\n");
  		printf("\t-U, --binary              do not strip CR characters at EOL (MSDOS/Windows)\n\n");

		printf("When FILE is '-', read standard input.  With no FILE, read '.' if\n");
		printf("recursive, '-' otherwise.  With fewer than two FILEs, assume -h.\n");
		printf("Exit status is 0 if any line (or file if -L) is selected, 1 otherwise;\n");
		printf("\tif any error occurs and -q is not given, the exit status is 2.\n\n");

		printf("Report bugs to: bug-grep@gnu.org\n");
		printf("GNU grep home page: <http://www.gnu.org/software/grep/>\n");
		printf("General help using GNU software: <https://www.gnu.org/gethelp/>\n");
		return;
}
