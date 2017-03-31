/* MPI ring communication (array):
 * Exchange between messages between mpirank
 * 0 <-> 1, 1 <-> 2, ...., N <-> 0
 */
#include <stdio.h>
#include <unistd.h>
#include <mpi.h>
#include "util.h"

int main( int argc, char *argv[])
{
  int i, N, arraysize, rank, tag, origin, destination;
  
  sscanf(argv[1], "%d", &N);
  arraysize = 262144;
  
  MPI_Status status;

  MPI_Init(&argc, &argv);
  
  //timestamp_type time1, time2;
  //get_timestamp(&time1);
  
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  double *message_out = (double *) malloc(sizeof(double) * arraysize);
  double *message_in = (double *) malloc(sizeof(double) * arraysize);
  
  for(i = 0; i < arraysize; i++)
  	{
  	  message_out[i] = i + 1.0;
    }

  tag = 99;
  
  int round = 0;
  
  timestamp_type time1, time2;
  get_timestamp(&time1);


  while(round < N)
  {
       if(rank == 0)
       { 
  	    origin = size - 1;
  	    destination = rank + 1;
  	    round ++;
  	    MPI_Send(message_out, arraysize, MPI_DOUBLE, destination, tag, MPI_COMM_WORLD); 
  	    MPI_Recv(message_in, arraysize, MPI_DOUBLE, origin, tag, MPI_COMM_WORLD, &status); 
        printf("The round %d communication finished.\n", round);
       }
       else
      { 
	   origin = rank - 1;
       destination = ( rank + 1 ) % size;
       round ++;
       MPI_Recv(message_in, arraysize, MPI_DOUBLE, origin, tag, MPI_COMM_WORLD, &status);
       MPI_Send(message_out, arraysize, MPI_DOUBLE, destination , tag, MPI_COMM_WORLD);
      }
  } 

  if ( rank == 0)
  {
  	get_timestamp(&time2);
	double elapsed = timestamp_diff_in_seconds(time1,time2);	 
  	printf("The time used for each communication (latency) is %f seconds.\n", elapsed/(N*size)); 
  	printf("The array is %f MB\n", arraysize*sizeof(double)/1e6);
    printf("The amount of data that can be communicated per second (latency) is %f MB/s\n", size*arraysize*sizeof(double)*N/1e6/elapsed); 
  }
  
  free(message_out);
  free(message_in);
  
  MPI_Finalize(); 
  return 0;
}
