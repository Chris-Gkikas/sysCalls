#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
int main(){
	int smokePipe[2],inFile;
	pid_t pid;
	char buff[4];
	if ( pipe(smokePipe)!=0 ) { printf("\nError during pipe creation!\n"); return -1; }		//pipe creation and error check
	if ( (pid=fork()) <0 ) { printf("\nError during forking!\n"); return -1; }				//child-client creation and error check								
	else if( pid==0 ){																		//child-client code
		close(smokePipe[0]);																//close read-end since child-client should only be writing
		printf("\n>Hi! Client here, type aver or summ or exit and press Enter.\n");
		while(1){
			read(0,buff,4);																	//read from stdin--->0
			write(smokePipe[1],&buff,4);													//child-client writes to buff
			if ( strcmp(buff,"exit") ==0 )													//loop brake when user types <exit>					
				break;
			}
			close(smokePipe[1]);															//close write-end after loop brake
			_exit(1);																		//terminate child process without doing anything else, like closing files
		}else{																				//parent-server code
			close(smokePipe[1]);															//close write-end since parent-server should only be reading
			char serverBuffer[4],fileData;
			ssize_t readBytes;									
			while(1){
				read(smokePipe[0],&serverBuffer,4);											//read from pipe
				if( strcmp(serverBuffer,"aver") == 0){
					inFile = open("database.txt", O_RDONLY);								//open file
					if ( inFile<0 ) { printf("\nError opening file!\n"); return -1; }
					float sum=0;
					while( (readBytes = read(inFile,&fileData,1) ) > 0){					//read from file			
						if (fileData != '\n'){
							int charToInt= fileData - '0';									//convert from char to int
							sum += charToInt;
						}
					}
					printf("-----------------------\n");
					printf("The average is: [%.2f]\n",sum/6 );
					printf("-----------------------\n");
					printf("\n\n>Do it again!\n");
					if ( close(inFile)<0 ) { printf("\nError closing file!\n\n"); }			//close file and error check
				}else if( strcmp(serverBuffer,"summ") == 0){								
					inFile = open("database.txt", O_RDONLY);								//open file
					if (inFile<0 ) { printf("\nError opening file!\n"); return -1; }
					float sum=0;
					while( (readBytes = read(inFile,&fileData,1)) > 0){
						if (fileData != '\n'){
							int charToInt= fileData - '0';									//convert from char to int
							sum += charToInt;
						}
					}
					printf("-----------------\n");
					printf("The sum is: [%d]\n",(int)sum );
					printf("-----------------\n");
					printf("\n\n>Do it again!\n");
					if (close(inFile)<0) printf("\nError closing file!\n\n");
				}
				if ( strcmp(serverBuffer,"exit") == 0 ){
					printf("-----------------\n");
					printf("exiting...\n");
					printf("-----------------\n\n");
					break;
				}																			//loop brake when user types <exit>
				close(smokePipe[1]);														//close write-end after loop brake									
			}
			wait(NULL);																		//parent wating for child to terminate										
			exit(0);																		//terminate parent process closing files, etc.
		}							
	return 0;
}
