/*
Satyam Todkar
pwordcount.cpp

The following code is a creation of my own and “I did not use any external sources for this assignment”
The sources that I used were mainly the lecture notes, professors solution and the inbuilt man command in the terminal were used to understand the syntax and the required libraries.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdbool.h> 
#include <string.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024		//declaring a buffer of size 1024
#define READ_END 0				
#define WRITE_END 1

bool fileExists(char* file);					//function prototype declaration
int countWords(char read_msg[BUFFER_SIZE]);		//function prototype declaration

int main(int argc, char **argv){
	
	FILE *input_file;				//Pointer to input file
	char write_msg[BUFFER_SIZE];	//Space for message to send
	char read_msg[BUFFER_SIZE];		//Space for message received
	int count = 0;					//Counter to track the number of words
	
	int input_fd[2];		//Input Pipe File Descriptor for making a link from producer to consumer i.e parent to child
	int output_fd[2];		//Output Pipe File Descriptor for making a link from consumer to producer i.e child to parent 
	
	if(argc != 2){			//Checking whether file name is supplied as an argument
		printf("Please enter a file name.\nUsage: ./pwordcount <filename>\n");
		return 1;
	}
	
	if(!fileExists(argv[1])){		//Checking if the file with given name exists
		printf("Error: File %s does not exists\n", argv[1]);
		return 1;
	}
	
	input_file = fopen(argv[1],"r");	//Opening the inputted file name in the readin mode
	if(input_file == NULL){			//Checking if file cann be opened
		printf("\nCould not open file: %s.",argv[1]);
		return 1;
	}
	
	if(pipe(input_fd) == -1){		//Creating a pipe
		fprintf(stderr, "Pipe Creation failed");
		return 1;
	}
	
	if(pipe(output_fd) == -1){		//Creating a pipe
		fprintf(stderr, "Pipe Creation failed");
		return 1;
	}
	
	else{
		pid_t pid = fork();			//Creating a child process
		if(pid > 0){				//Parent Process
			/*Closing the unused end of the pipe*/
			close(input_fd[READ_END]);									
			
			printf("\nProcess 1 is reading file “input.txt” now ...");
			
			/*Reading the contents of the file using fgets*/
			if(fgets(write_msg, 1024, input_file)!=NULL){		
				printf("\nProcess 1 starts sending data to Process 2 ...");
				/*Sending the contents to the child process using the write function*/
				write(input_fd[WRITE_END], write_msg, strlen(write_msg)+1);
				
				/*Closing the write end of the pipe*/
				close(input_fd[WRITE_END]);	
				
				/*Waiting for the child process to finish its execution*/
				wait(NULL);
			
				/*Closing the unused end of the pipe*/
				close(output_fd[WRITE_END]);
				
				/*Reading the number of words sent by the child*/
				read(output_fd[READ_END], read_msg, BUFFER_SIZE);
				printf("\nProcess 1: The total number of words is %s. \n",read_msg);
				
				/*Closing the read end of the pipe*/
				close(output_fd[READ_END]);
			}
			else{
				printf("\nThe file is empty or has only one blank line",read_msg);
				return 1;
			}
		}
		
		else if(pid ==0){
			/*Closing the unused end of the pipe*/
			close(input_fd[WRITE_END]);
			
			/*Reading the contents sent by the parent process*/
			read(input_fd[READ_END], read_msg, BUFFER_SIZE);
			printf("\nProcess 2 finishes receiving data from Process 1 ...");
			
			/*Counting the number of words in the received message*/
			count = countWords(read_msg);
			
			printf("\nProcess 2 is counting words now ...");
			
			/*Closing the read end of the pipe*/
			close(input_fd[READ_END]);
			
			/*Closing the unused end of the pipe*/
			close(output_fd[READ_END]);
			
			/*Transferring the data to the write_msg string/array of characters*/
			sprintf(write_msg, "%d", count+1);
			
			printf("\nProcess 2 is sending the result back to Process 1 ...");
			/*Sending the number of words computed to the parent Process*/
			write(output_fd[WRITE_END], write_msg, strlen(write_msg)+1);
			
			/*Closing the write end of the pipe*/
			close(output_fd[WRITE_END]);
		}
		
		else{
			/*Creating child process fails*/
			fprintf(stderr,"Fork failed");
			return 1;
		}
	}

	return 0;
}

/*Function to check if file exists using the stat command in linux operating system*/
bool fileExists(char* file){
	struct stat buf;
	return (stat(file,&buf) == 0);
}

/*Function to compute the number of words; returns the count*/
int countWords(char read_msg[BUFFER_SIZE]){
	int i = 0;
	int count=0;
	int flag = 0;
	while(read_msg[i] != '\0'){
		if((read_msg[i] == ' '|| read_msg[i] == '\t' ) && flag == 0){
			count++;
			flag = -1;
		}
		
		else if(read_msg[i] != ' ' && read_msg[i] != '\t'){
			flag = 0;
		}
		i++;
	}
	return count;
}


