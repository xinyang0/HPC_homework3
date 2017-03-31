/* MPI ring communication:
 * Exchange between messages between mpirank
 * 0 <-> 1, 1 <-> 2, ...., N <-> 0
 */
#include <stdio.h>
#include <unistd.h>
#include <mpi.h>
#include "util.h"

int main( int argc, char *argv[])
{
  int i, N, rank, tag, origin, destination;
  
  sscanf(argv[1], "%d", &N);
  
  MPI_Status status;

  MPI_Init(&argc, &argv);
  
  timestamp_type time1, time2;
  //get_timestamp(&time1); 
  
  
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int message_out = 0;
  int message_in;
  tag = 99;

  int round = 0;
  
  //timestamp_type time1, time2;
  get_timestamp(&time1);
  
  while(round < N)
  { 
       if( rank == 0 )
       { 
  	    origin = size - 1;
  	    destination = rank + 1;
  	    round ++;
  	    message_out = message_in + rank;
  	    MPI_Send(&message_out, 1, MPI_INT, destination, tag, MPI_COMM_WORLD); 
  	    MPI_Recv(&message_in, 1, MPI_INT, origin, tag, MPI_COMM_WORLD, &status); 
  	    printf("At round %d rank 0 received message %d\n", round, message_in);
       }
       else
      { 
       origin = rank - 1;
       destination = ( rank + 1 ) % size;
       round ++;
       MPI_Recv(&message_in, 1, MPI_INT, origin, tag, MPI_COMM_WORLD, &status);
       message_out = message_in + rank;
       MPI_Send(&message_out, 1, MPI_INT, destination , tag, MPI_COMM_WORLD);
      }
  }
   
  if ( rank == 0 )
  {
     get_timestamp(&time2);
     double elapsed = timestamp_diff_in_seconds(time1,time2);
     printf("The time used for each communication (latency) is %f seconds.\n", elapsed/(N*size));
  }
     
  MPI_Finalize(); 
  return 0;
}
