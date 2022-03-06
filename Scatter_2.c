#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mpi.h"

int main(int argc, char** argv) {


	//=========VARIABLES INITIALIZATION===========
	
	int MyRank, NoProcs;
	int Root = 0;
	int index;
	int *Input_Buffer, *Recv_Buffer;
	int Data_size, Scatter_size;
	FILE *fp;
	MPI_Status status;
	
	//========MPI INITIALIZATION=======
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &MyRank);
	MPI_Comm_size(MPI_COMM_WORLD, &NoProcs);
	
	//
	if( MyRank == 0) {
		//reading the input file for data
		fp = fopen ("./data/sdata.inp", "r");
		if(fp == NULL) {
			printf("\n Data invalid/unable to open the input file \n");
			exit(-1);
		} //end of if statement
		
		fscanf(fp, "\n %d", &Data_size);
		
		//====MEMORY ALLOCATION=====
		Input_Buffer = (int *)malloc(Data_size * sizeof(int));
		
		//=====READING DATA TO MEMORY=====
		for(index=0; index< Data_size; index++){
			fscanf(fp, "%d", &Input_Buffer[index]); 
		} //end of for loop
	} //end of if statement
	
	//Brocasting the data size required to allocate memory in other processes
	
	MPI_Bcast(&Data_size, 1, MPI_INT, Root, MPI_COMM_WORLD);
	
	if(Data_size % NoProcs != 0){ //error flag set to 1
		if(MyRank == Root){
			printf("\nError: Input cant be divided by number of processors.\n");
		}
		
		MPI_Finalize();
		exit(-1); 								//CLEAN EXIT
	} //end of if statement
	
	Scatter_size = Data_size / NoProcs;
	Recv_Buffer= (int *)malloc(Scatter_size * sizeof(int)); //allocating memory for the data to stored in other processes buffers
	
	//=======CALLING MPI SCATTER=======
	MPI_Scatter(Input_Buffer, Scatter_size, MPI_INT, Recv_Buffer, Scatter_size, MPI_INT, Root, MPI_COMM_WORLD);
	
	//prininting the values
	for(index = 0; index < Scatter_size; index++){
		printf("MyRank = %d, RecvBuffer[%d] = %d \n", MyRank, index, Recv_Buffer[index]);
	}
		
	
	
	//MPI FINALIZATION
	MPI_Finalize();
}
