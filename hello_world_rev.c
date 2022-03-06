/********
Name: Pravallika Saladi
Program: Sending message (HELLO WORLD) from Process with Rank 0 to other Processes
*********/
#include<stdio.h>
#include "mpi.h"
#define BUFFER_LENGTH 200

int main(int argc, char *argv[])
{
	//==============variables declaration=======================
	int MyRank, NoProcs; //the rank of the process and NUmber of Processes in total
	int Source, Destination; //source rank and destination rank
	int Source_tag = 0, Destination_tag = 0; // tags
	int root = 0; //the root process rank is 0
	char process_name[100]; //stores the name of the processor
	int length_name; //stores the length of the process_name
	char process_name_root[100]; // stores the name of the root processor
	int length_name_root; //stores the length of process_name_root
	MPI_Status status; // the status of the received message
	
	char Send_Message[BUFFER_LENGTH]; // Message to send is stored 
	char Recv_Message[BUFFER_LENGTH];
	
	
	//============MPI_INITIALIZATION==============
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &MyRank);
	MPI_Comm_size(MPI_COMM_WORLD, &NoProcs);
	
	//===SENDING MESSAGE FROM ROOT 0 TO OTHER PROCESSES======
	
	if(MyRank == 0) {
		for(Destination = 0; Destination < NoProcs; Destination++) {
			sprintf(Send_Message, "Hello World");
			MPI_Get_processor_name(process_name, &length_name); //to get the process name of destination process 
			printf("\n SPMD: Process with Rank %d [Node : %s] Sending MESSAGE \"%s\" to Process with Rank %d \n\n", MyRank, process_name, Send_Message, Destination);
			MPI_Send(Send_Message, BUFFER_LENGTH, MPI_CHAR, Destination, Destination_tag, MPI_COMM_WORLD);
		} //END OF FOR LOOP		
	} // END OF IF 
	
	else {
		Source = 0;
		
		MPI_Get_processor_name(process_name_root, &length_name_root);
		MPI_Recv(Recv_Message, BUFFER_LENGTH, MPI_CHAR, Source, Source_tag, MPI_COMM_WORLD, &status);
		printf("\n SPMD : Process with Rank %d [Node : %s] Received MESSAGE \"%s\"  from Process with Rank %d\n\n", MyRank, process_name_root, Recv_Message, Source);
	} //end of else
	
	//=======MPI Finalization========
	MPI_Finalize();
	
}//end of program
