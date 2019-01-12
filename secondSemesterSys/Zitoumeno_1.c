#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>


int main(int argc, char const *argv[]){
	
	int inFile,outFile,flag=1,lines=1;
	ssize_t readBytes,writeBytes;  
	char  _1digit=lines+'0',_2digit[2],bracket[2]={']',' '},buff,buff11X[11]={' ','[','2','0','1','5','0','2','6',']','\n'}; 
	
	//Error controls of arguments and files.
	if (argc !=3 ){ 
		printf("\nERROR: You should provide 2 arguments!\n\n");
		return -1; 
	}
	if ((inFile = open(argv[1],O_RDONLY)) == -1){ 
		printf("\nERROR: Can't open input file!\n\n");
		return -1;
	}
	if ((outFile = open(argv[2],O_WRONLY|O_CREAT,S_IRWXU)) == -1){
		printf("\nERROR: Can't open/create output file!\n\n"); 
		return -1; 
	}
	while(( readBytes = read(inFile,&buff,1)) >0 ){
		
		if (flag==1){							//flag: indication of new line
			if (lines<10){						//one-digit line numer
        		_1digit=lines+'0';					//convert from int to char
        		write(outFile,&_1digit,(ssize_t)sizeof(_1digit));	//write line number to output file
        		write(outFile,&bracket,(ssize_t)sizeof(bracket));
        	}else{								//two-digit line number
        		int first=lines/10;					//firt digit of two-digit line number
        		int second=lines%10;					//second digit of two-digit line number
        		_2digit[0]=first+'0';									
        		_2digit[1]=second+'0';
        		write(outFile,&_2digit,(ssize_t)sizeof(_2digit));	//write line number to output file
        		write(outFile,&bracket,(ssize_t)sizeof(bracket));
        	}flag=0;        	
		} 
		if(buff=='\n'){							//end of current line
			write(outFile,&buff11X,(ssize_t)sizeof(buff11X));	//write [2015026] to the end of current line
			flag=1;	 
			lines++;	
		 }else
		 	writeBytes = write(outFile,&buff,1);			//copy characters from input to output file
		 if (writeBytes != readBytes){
			 printf("ERROR: Unequal number of read/write bytes in line #%d \n\n",lines); ////in case of cpu goes crazy
			 return -1; 
		 }	
	}
	if (close(inFile)<0) printf("\nERROR: Can't close %s file\n\n",argv[1]);
	if (close(outFile)<0) printf("\nERROR: Can't close %s file\n\n",argv[2]);
	return 0;
}
