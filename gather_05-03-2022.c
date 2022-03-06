/******
NAME 		: PRAVALLIKA SALADI
DATE 		: 05 - 04 - 2022
PROGRAM 	: GATHER.C
Input           : Data from files (gdata0,gdata1,gdata2,gdata3,gdata4,gdata5,gdata6,gdata7) is available on each process.
Output          : Gathered data is printed on each process.
Condition 	 : Number of Processes should be equal to 4
******/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "mpi.h"

int main(int argc, char** argv){
	/*****VARIABLES INITIALIZATION*******/
		int MyRank, NoProcs;				//to store my rank and the total no of processes
		int Root = 0,Destination_tag = 0;		//root/source process is process with rank 0 
		int index, total = 0;					//used for iterations
		int *Input_Buffer, *Recv_Buffer;		//input and recieve buffers
		int Data_size, Total_gather_size;		//Data size is the size of data in each process (here 6) and total gather size is the memory alloted in Process 0 to gather all the data from other Processes
		int n_size;
		char	str[4], filename[50] = "./data/gdata"; //to read input files from the give address
		FILE *fp;					//FILE 
		MPI_Status status;
		
	/*******MPI_INITIALIZATION******/
		MPI_Init(&argc, &argv);
		MPI_Comm_rank(MPI_COMM_WORLD, &MyRank);
		MPI_Comm_size(MPI_COMM_WORLD, &NoProcs);
		
	/***TO CHECK IF THE NUMBER OF PROCESSES ARE COMPATIBLE WITH THE CODE***/
		if(NoProcs != 8) {
			if(MyRank == Root)
				printf("\n ERROR: Number of Processes must be eight in number\n");
			MPI_Finalize();
			exit(-1);
		} 
		
	/*****READING THE INPUTS FROM THE INPUT FILES*****/
		sprintf(str, "%d", MyRank);			//store my rank(int) value in the str (string)
    		strcat(filename, str);				//concat the file location with the rank
    		strcat(filename, ".inp");
    		if ((fp = fopen (filename, "r")) == NULL)	//if there is no data in the input file, error flag is set
		    {
		       printf("\nCan't open input file");
		       MPI_Finalize(); 			//ending the MPI
		       exit(-1);				//CLEAN EXIT
		    }
		    fscanf(fp, "%d", &Data_size); 		//if there is data, the size is stored at the address of Data Size
		    
	/*****ALLOCATING MEMORY AT ROOT NODE(PROCESS 0)****/
		Input_Buffer = (int *)malloc(Data_size*sizeof(int));
		Total_gather_size = Data_size * NoProcs; //total gather size
		
	/****TO GET THE TOTAL NUMBER OF DATA FROM EACH PROCESS****/
		for(index=0; index< NoProcs; index++){
			total += Data_size; 
			} 
			
	/********Sending the data size of each process to Root******/
		if(MyRank != 0) {			
	       	MPI_Send(&Data_size, 1, MPI_INT, Root, Destination_tag,MPI_COMM_WORLD);
			printf("\n Process with Rank %d Sending BUFFER SIZE \"%d\" to Process with Rank %d \n\n",MyRank, Data_size,Root);
		}
	
	/*******Recieving the data sizes from other processes*****/
		int Recv_Message; 
		if(MyRank == 0){
			for(index = 1 ; index < NoProcs ; index++) {
			   int Source = index;
			   int Source_tag = 0;
			   MPI_Recv(&Recv_Message, 1, MPI_INT, Source, Source_tag, 
				    MPI_COMM_WORLD, &status);
			   printf("\n Process with Rank %d Recieved BUFFER SIZE \"%d\" from Process with Rank %d \n\n",Root, Data_size,MyRank);
			   }
		}		
	
			
	/****TO CHECK IF RECIEVED DATA SIZE AND TOTAL GATHER SIZE IS SAME****/
		if( total != Total_gather_size) {
			printf("Size incompatible\n");
			//MPI_Bcast(message,BUFFER_SIZE,MPI_CHAR,Root,MPI_COMM_WORLD);
			MPI_Finalize();
			exit(-1); //clean exit
		}
		
	/******READING DATA******/
		for(index=0; index< Data_size; index++) 
        		fscanf(fp, "%d", &Input_Buffer[index]); 
        
        /****ALLOCATING MEMORY TO RECIEVE****/
		if(MyRank == Root)		
			Recv_Buffer = (int *)malloc(Total_gather_size*sizeof(int));
        
        /*******INITIATING MPI_GATHER******/
        	MPI_Gather(Input_Buffer, Data_size, MPI_INT, Recv_Buffer, Data_size, MPI_INT, Root, MPI_COMM_WORLD);
	
	/*******PRINTING VALUES*******/
		//Sent/Input from other processes to Root
		for(index = 0; index < Data_size; index++){
			printf("MyRank = %d, Input_Buffer[%d] = %d \n", MyRank, index, Input_Buffer[index]);
		}
		printf("\n");
	
		//Recieved data from other processes
		if(MyRank == Root){
		for(index = 0; index < Total_gather_size; index++)
			printf("MyRank = %d, Recv_Buffer[%d] = %d \n", MyRank, index, Recv_Buffer[index]);
		}
		printf("\n");
		
				
	/*****FInalizing MPI*****/
		MPI_Finalize();
}
