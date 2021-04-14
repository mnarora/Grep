#define _GNU_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<unistd.h>
#include<getopt.h>
#include<dirent.h>
#include<sys/types.h>
#include<errno.h>
#include "functions.h"
#include "list.h"

#define MAX 1024



void listFilesRecursively(char *basePath, list *l) {
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);
    if (!dir) 
        return;
    while ((dp = readdir(dir)) != NULL) {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
        	if(strcmp(basePath, ".") != 0) {
           		strcpy(path, basePath);
            	strcat(path, "/");
            	strcat(path, dp->d_name);
        } 
        else
        	strcpy(path, dp->d_name);
    
    	int filec = 0;
    		while(path[filec] != '\0') {
    			if(path[filec] == '.') {
    				if((path[strlen(path) - 1] != 'o') && (strcmp(path, "a.out") != 0) && path[strlen(path) - 1] != 'p' && path[strlen(path) - 1] != 'h') {
    	    			add(l,path); 
    				}
    			}
    			filec++;
    		}
    		listFilesRecursively(path, l);
		}
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    int fd, n = 0, flag = 1, i, option,cflag = 0, hflag = 0, Hflag = 0, nflag = 0, iflag = 0, wflag = 0, oflag = 0, lflag = 0, vflag = 0, fflag = 0, rflag = 0, mflag = 0, j, k, m, fdf, bflag = 0, marg, qflag = 0;
    int casesen = 0, wordfind = 0, q;
    list l;
    char line[1024], line1[1024], word[25];
    char *str1;
    
    if(argc == 1) {
    	if(strcmp(argv[0], "./grep --help") == 0) {
    		printhelp();
    		return 0;
    	}
    	printf("Usage: ./grep [OPTION]... PATTERNS [FILE]...\n");
    	printf("Try './grep --help' for more information.\n");
    	return 0;
    }
    
	if(argc == 2 && (strcmp(argv[1], "--help") == 0)) {
		printhelp();
		return 0;
	}
    	
    while((option = getopt(argc, argv, "iwvcHonlhm:bqre:f:")) != -1) {
		switch (option) {
			case 'i':
				casesen = 1;
				iflag = 1;
				break;
			case 'w':
				wordfind = 1;
				wflag = 1;
				break;
				
			case 'v':
				vflag = 1;
				break;
			
			case 'c':
				cflag = 1;
				break;
			
			case 'H':
				Hflag = 1;
				break;

			case 'm':
				mflag = 1;
				marg = atoi(optarg);
				break;
				
			case 'b':
				bflag = 1;
				break;
				
			case 'q':
				qflag = 1;
				break;
				
			case 'h':
				hflag = 1;
				break;
				
			case 'r':
				rflag = 1;
				break;
			case 'l':
				lflag = 1;
				break;
			case 'f':
				fflag = 1;
				break;
			case 'o':
				oflag = 1;
				break;
			case 'n':
				nflag = 1;
				break;
			default :
				flag = 1;
				break;
    
		}
	}

	strcpy(word, argv[argc - 2]);
	init(&l);
	
		
	
	if(qflag) {
		return 0;
	}
	
	if(rflag) {
		int j = 0;
	   	i = 0;
	   	init(&l);
   		listFilesRecursively(argv[argc - 1], &l);

   		while(!isempty((&l))) {
   			str1 = del(&l);
   			fd = open(str1, O_RDONLY);
			if(fd == -1) {
				printf("%s: %s: No such file or directory\n", argv[0], str1);
				continue;
			}
			while(readline(fd, line)) {
   				if(find(word, line, casesen, wordfind) == 1 && !(vflag)) {
    	 			i++;
    	 			if(!cflag)
    	 				Hprint(str1);
    	 			if(bflag && !cflag) {
   						green();
   						printf("%d", j);
   						blue();
   						printf(":");
   						reset();
   					}
    	 		if(!cflag) {
    	 			if(!iflag)	
  	    	 			highlight(word, line);
  	    	 		else
  	    	 			printf("%s\n", line);
  	    	 	}
   			}	
   				else if(!(find(word, line, casesen, wordfind)) && vflag) {
   					i++;
    	 			Hprint(str1);
    	 			if(bflag && !cflag) {
   						green();
   						printf("%d", j);
   						blue();
   						printf(":");
   						reset();
   					}
		
    	 			if(!cflag) {
    	 				if(!iflag)	
  	    	 				highlight(word, line);
  	    	 			else
  	    	 				printf("%s\n", line);
  	    	 		}
   				}
   				j = j + strlen(line) + 1;
   			}
   			j = 0;
   			if(cflag) {
   				Hprint(str1);
   				printf("%d\n", i);
   			}
   			i = 0;
   			close(fd);
   		}
   	 	return 0;
	}
	
	init(&l);
	if(fflag) {
		i = 3;
		flag = 0;
	}
	else
		i = 1;
	j = 0;
	while(i < argc) {
		if(dotpresent(argv[i]) && !(dashpresent(argv[i]))) {
			add(&l, argv[i]);
			j++;
		}
		else if(!(dashpresent(argv[i])) && !(dotpresent(argv[i])))
			strcpy(word, argv[i]);
		else if(dashpresent(argv[i])) {
			flag = 0;
		}
		i++;
	}
	
	if(flag == 1) {
		while(!isempty(&l)) {
			str1 = del(&l);
			fd = open(str1, O_RDONLY);
			if(fd == -1) {
				printf("%s: %s: No such file or directory\n", argv[0], str1);
				continue;
			}
			while(readline(fd, line)) {
    		 	if(find(word, line, casesen, wordfind) == 1) {
    		 		if(j != 1 || Hflag)
    		 			Hprint(str1);				
    		 		if(!cflag) {
    	 				if(!iflag)	
  	    	 				highlight(word, line);
  	    	 			else
  	    	 				printf("%s\n", line);
  	    	 		}	
    			}	   	 
   	 		}
   	 		close(fd);
   		 }
   	 }
   	 
   	 if(fflag) {
   		fdf = open(argv[2], O_RDONLY);
   		while(!isempty(&l)) {
			str1 = del(&l);
			fd = open(str1, O_RDONLY);
			if(fd == -1) {
				printf("%s: %s: No such file or directory\n", argv[0], str1);
				continue;
			}
   			while(readline(fd, line1)) {
   				n = 0;
   				while(readline(fdf, line)) {
   					if(find(line, line1, casesen, wordfind)) {
   						n++;
   						if(n == 1) {
   							if(Hflag || j != 1)
   								Hprint(str1);
   							red();
   							if(!cflag) {
    	 						if(!iflag)	
  	    			 				highlight(line, line1);
  	    		 				else
  	    	 						printf("%s\n", line);
  	    	 				}
   						reset();
   						}
   					}	
   				}
   			lseek(fdf, 0, SEEK_SET);
   			}
   			close(fdf);
   			close(fd);
   		}
   	return 0;
   	}
   	
   	
   	 if(mflag) {
   	 	while(!isempty(&l)) {
			str1 = del(&l);
			fd = open(str1, O_RDONLY);
			q = 0;
			k = 0;
			if(fd == -1) {
				printf("%s: %s: No such file or directory\n", argv[0], str1);
				continue;
			}
			int i = 0;
			while(readline(fd, line)) {
				if(find(word, line, casesen, wordfind) && i < marg && !vflag) {
					i++;
					
					if(!cflag) {
						if(j != 1 || Hflag)
							Hprint(str1);
						if(bflag)
							bprint(k);
    	 				if(!iflag)	
  	    	 				highlight(word, line);
  	    	 			else
  	    	 				printf("%s\n", line);
  	    	 		}
  	    	 	}
  	    	 	else if(!(find(word, line, casesen, wordfind)) && q < marg && vflag) {
  	    	 		q++;
					if(!cflag) {
						if(j != 1 || Hflag)
							Hprint(str1);
						if(bflag)
							bprint(k);
    	 				if(!iflag)	
  	    	 				highlight(word, line);
  	    	 			else
  	    	 				printf("%s\n", line);
  	    	 		}
  	    	 	}
  	    	  k = k + strlen(line) + 1; 	    	 		
  	    	}
  	    	if(cflag && vflag) {
  	    		Hprint(str1);
  	    		printf("%d\n", q);
  	    	}
  	    	if(cflag && !vflag) {
  	    		Hprint(str1);
  	    		printf("%d\n", i);
  	    	}
  	    	k = 0; 
  	    	close(fd);
  	    	i = 0;
  	    	q = 0;
  	    }
  	   
  	    return 0;
	}
				
   	 if(lflag) {
   	 	while(!isempty(&l)) {
			str1 = del(&l);
			fd = open(str1, O_RDONLY);
			if(fd == -1) {
				printf("%s: %s: No such file or directory\n", argv[0], str1);
				continue;
			}
   	 		while(readline(fd, line)) {
    			if(strstr(line, word) != NULL) {
    				purple();
    				printf("%s\n", str1);
    				reset();
    				break;	
    			}	   	 
   	 		}
   	 		close(fd);
   	 	}
		return 0;
	}
   	 
   	 if(hflag) {
		while(!isempty(&l)) {
			str1 = del(&l);
			fd = open(str1, O_RDONLY);
			if(fd == -1) {
				printf("%s: %s: No such file or directory\n", argv[0], str1);
				continue;
			}
			while(readline(fd, line)) {
    	 		if(find(word, line, casesen, wordfind) == 1) {
    	 			if(Hflag)
    	 				Hprint(str1);				
    	 			if(!cflag) {
    	 				if(!iflag)	
  	    		 			highlight(word, line);
  	    		 		else
  	    		 			printf("%s\n", line);
  	    		 	}
    			}	   	 
   	 		}
   	 		close(fd);
   	 	}
   	 return 0;
	}
   	 
	if(iflag) {
		if(wflag) {
   	 		int k = 0, i = 0, f = 0;
   	 		while(!isempty(&l)) {
			str1 = del(&l);
			fd = open(str1, O_RDONLY);
			if(fd == -1) {
				printf("%s: %s: No such file or directory\n", argv[0], str1);
				continue;
			}
   	 		while(readline(fd, line)) {
   	 			if(find(word, line, casesen, wordfind) && !(vflag)) {
   	 				i++;		
   	 				if(j != 1 || Hflag) {
   	 					if(!cflag)
   	 						Hprint(str1);
   	 				}
   	 				if(bflag)
   	 					bprint(k);
   	 				if(!cflag)
   	 					printf("%s\n", line);
   	 				}
   	 			else if(!(find(word, line, casesen, wordfind)) && vflag) {
   	 				f++;
   	 				if(bflag)
   	 					bprint(k);
   	 				if(j != 1 || Hflag) {
   	 					if(!cflag)
   	 						Hprint(str1);
   	 				}
   	 				if(!cflag)
   	 					printf("%s\n", line);
   	 			}
   	 			k = k + strlen(line) + 1;
   	 		}
   	 		if(cflag && vflag) {
   	 			Hprint(str1);
   	 			printf("%d\n", f);
   	 		}
   	 		if(cflag && !vflag) {
   	 			Hprint(str1);
   	 			printf("%d\n", i);
   	 		}
   	 		i = 0;
   	 		f = 0;
   	 		close(fd);
   	 		k = 0;
   	 	}
   	 	return 0;
	}
   	 	else {
   	 		i = 0; m = 0;
   	 		int k = 0;
   	 		while(!isempty(&l)) {
				str1 = del(&l);
				fd = open(str1, O_RDONLY);
				if(fd == -1) {
					printf("%s: %s: No such file or directory\n", argv[0], str1);
					continue;
				}
   		 		while(readline(fd, line)) {   	 			
   		 			if(vflag) {
   		 				if(!(find(word, line , casesen, wordfind))) {
							m++;
   		 					if(cflag == 0) {
   		 						if(nflag) {
   		 							green();
   		 							printf("%d", m);
   		 							blue();
   		 							printf(":");
   		 							reset();
   		 							if(!cflag) {
    		 							if(!iflag)	
  		    	 							highlight(word, line);
  		    	 						else
  		    	 							printf("%s\n", line);
  		    	 					}
   		 						}
   		 					}
   		 					if(j != 1 || Hflag)
   		 						Hprint(str1);
   		 					if(bflag)
   		 						bprint(k);
   		 					if(!cflag) {
    		 					if(!iflag)	
  		    	 					highlight(word, line);
  		    	 				else
  		    	 					printf("%s\n", line);
  		    	 			}
   		 				}
					}		
					else {
						if(find(word, line, casesen, wordfind)) {	
   		 					if(!cflag) {
   		 						if(j != 1 || Hflag)
   		 							Hprint(str1);
   		 						if(bflag)
   		 							bprint(k);
   		 						if(!cflag) {
    		 						if(!iflag)	
  		    	 						highlight(word, line);
  		    	 					else
  		    	 						printf("%s\n", line);
  		    	 				}
   		 					}
   		 					i++;
   		 				}
   		 			}
   		 			k = k + strlen(line) + 1;
				}
				if((cflag) && vflag) {
					if(Hflag || j != 1)
						Hprint(str1);
					printf("%d\n", m);
				}
				if((cflag) && !vflag) {
					if(Hflag || j != 1)
						Hprint(str1);
					printf("%d\n", i);
				}
				i = 0; 	
				k = 0;
				close(fd);
			}
		}
   		return 0; 
	}
   	
   	if(vflag) {
   		i = 0;
   	 	while(!isempty(&l)) {
			str1 = del(&l);
			fd = open(str1, O_RDONLY);
			if(fd == -1) {
				printf("%s: %s: No such file or directory\n", argv[0], str1);
				continue;
			}			
   	 		while(readline(fd, line)) {
    			if(strstr(line, word) == NULL) {
    				i++;
					if(j != 1 || Hflag) {
						if(!cflag)
    						Hprint(str1);
    				}
  	    		 	if(!cflag) {
    	 				printf("%s\n", line);
  	    		 	}
  	  	  	 	}		
    		}  
    		if(cflag) {
   	 			if(j != 1 || Hflag) 
   	 				Hprint(str1);
   	 			printf("%d\n", i);
   	 			}	
   	 			i = 0;	
   	 			close(fd); 	 
   	 		}	
   	 	
   	 		return 0;
   	 	} 
   	 	
   	
   	
      	    	 
   	 if(wflag) {
   	 	while(!isempty(&l)) {
			str1 = del(&l);
			fd = open(str1, O_RDONLY);
			i = 0;
			if(fd == -1) {
				printf("%s: %s: No such file or directory\n", argv[0], str1);
				continue;
			}	
   	 		while(readline(fd, line)) {
   	 			if(find(word, line, casesen, wordfind)) {
   	 				i++;
   	 				if(!cflag) {
   	 					if(j != 1 || Hflag)
   	 						Hprint(str1);
    	 				if(!iflag)	
  	    		 			highlight(word, line);
  	    		 		else
  	    		 			printf("%s\n", line);
  	    		 	}
  	    	 	
   	 			}
			}
			if(cflag) {
				if(j != 1 || Hflag)
   	 				Hprint(str1);
   	 			printf("%d\n", i);
   	 		}
   	 		close(fd);
		}
   	 	return 0;
	}
   	
   	if(cflag) {
   		while(!isempty(&l)) {
   			n = 0;
			str1 = del(&l);
			fd = open(str1, O_RDONLY);
			if(fd == -1) {
				printf("%s: %s: No such file or directory\n", argv[0], str1);
				continue;
			}
   	 		while(readline(fd, line)) {
   	 			if(strstr(line, word) != NULL) {		
   	 				n++;
   	 			}
   	 		}
   	 		close(fd);
   	 		if(j != 1 || Hflag)
   	 			Hprint(str1);
   	 		printf("%d\n", n);
   	 	} 
	} 
   	 if(Hflag) {
   	 	while(!isempty(&l)) {
			str1 = del(&l);
			fd = open(str1, O_RDONLY);
			if(fd == -1) {
				printf("%s: %s: No such file or directory\n", argv[0], str1);
				continue;
			}
   	 		while(readline(fd, line)) {
    			if(strstr(line, word) != NULL) {
    				i = 0;
    				Hprint(str1);
    				while(line[i] != word[0]) {
    					printf("%c", line[i]);
    					i++;
    				}
    				red();
    				printf("%s", word);
    				reset();
    				i = i + strlen(word);
    				while(line[i] != '\0') {
    					printf("%c", line[i]);
    					i++;
    				}
    				printf("\n");
    			}	   	 
   	 		}
   	 		close(fd);
		}
	}
   	 
   	 if(nflag) {
   	 	while(!isempty(&l)) {
			str1 = del(&l);
			fd = open(str1, O_RDONLY);
			if(fd == -1) {
				printf("%s: %s: No such file or directory\n", argv[0], str1);
				continue;
			}
			n = 0;
   	 		while(readline(fd, line)) {
   	 			n++;
    			if(strstr(line, word) != NULL) {
    				if(j != 1 || Hflag)
    					Hprint(str1);
    				green();
    				printf("%d", n);
    				blue();
    				printf(":");
    				reset();
    				i = 0;
    				while(line[i] != word[0]) {
    					printf("%c", line[i]);
    					i++;
    				}
   					red();
   					printf("%s", word);
    				reset();
    				i = i + strlen(word);
    				while(line[i] != '\0') {
    					printf("%c", line[i]);
    					i++;
    				}
    				printf("\n");
    			}	   	 
			}
 	  	 	close(fd);
		}
		return 0;
   	}

   	 if(oflag) {
   	 	while(!isempty(&l)) {
			str1 = del(&l);
			fd = open(str1, O_RDONLY);
			if(fd == -1) {
				printf("%s: %s: No such file or directory\n", argv[0], str1);
				continue;
			}
   	 		while(readline(fd, line)) {
    			if(strstr(line,word) != NULL) {
    				if(j != 1 || Hflag)
    					Hprint(str1);
    				red();
    				printf("%s\n", word);
    			}
    		}
    		close(fd);
    		reset();
		}
		return 0;
    }
    
   	if(bflag) {
   		while(!isempty(&l)) {
			str1 = del(&l);
			fd = open(str1, O_RDONLY);
			if(fd == -1) {
				printf("%s: %s: No such file or directory\n", argv[0], str1);
				continue;
			}
   			i = 0;
   			while(readline(fd, line)) {
   				if(find(word, line, casesen, wordfind)) {
   					if(j != 1 || Hflag)
   						Hprint(str1);
   					green();
   					printf("%d", i);
   					blue();
   					printf(":");
   					reset();
   					if(!cflag) {
    	 				if(!iflag)	
  	    		 			highlight(word, line);
  	    		 		else
  	    		 			printf("%s\n", line);
  	    		 	}
   				}
   				i = i + strlen(line) + 1;
   			}
   			close(fd);
   		}
   		return 0;
   	}	 		
	return 0;
}
