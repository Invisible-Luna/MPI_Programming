/*
Name: Pravallika Saladi
Program: Braodcasting the message/info "Hello World from Process 0 to the rest of the processes.
*/
#include <stdio.h>
#include "mpi.h"
#include <string.h>
#define BUFFER_SIZE 100

int main(int argc, char **argv) {

	//================VARIABLES INITIALIZATION================
	int MyRank, NoProcs; 		//to store my processors rank and total no of processes involved
	int Root = 0; 			//as we are broadcasting from Process of Rank 0 to the rest of the processes
	char message[BUFFER_SIZE]; 	//the message/information to be conveyed/broadcasted to the rest of the Processes
	
	//===============MPI INTIALIZATION=================
	MPI_Init(&argc, &argv);                      	//initialization of MPI
	MPI_Comm_rank(MPI_COMM_WORLD, &MyRank);	//rank of our process is stored in MyRank
	MPI_Comm_size(MPI_COMM_WORLD, &NoProcs);	//TOTAL NO OF PROCESSES ARE STORED IN NoProcs
	
	//=============MESSAGE=========
	if(MyRank == 0){
		strcpy(message, "HELLO WORLD");
		
		/* instead of strcpy we can also use the below without including string header file(string.h)
		sprintf(message, "Hello World");
		*/
		
	} //END OF IF STATEMENT
	
	//========CALLING FOR A BROADCAST OF THE ABOVE MESSAGE=======
	
	MPI_Bcast(message, BUFFER_SIZE, MPI_CHAR, Root, MPI_COMM_WORLD);
	
	//===========printing=================
	printf("\n Process with Rank: %d recieved the message %s from Process: %d \n\n", MyRank, message, Root);

} //end of program
