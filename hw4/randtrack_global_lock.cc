
#include <stdio.h>
#include <stdlib.h>

#include "defs.h"
#include "hash.h"
#include <pthread.h>

#define SAMPLES_TO_COLLECT   10000000
#define RAND_NUM_UPPER_BOUND   100000
#define NUM_SEED_STREAMS            4

/* 
 * ECE454 Students: 
 * Please fill in the following team struct 
 */
team_t team = {
    "Naif_and_jordan",                  /* Team name */

    "Naif Tarafdar",                    /* First member full name */
    "997590566",                 /* First member student number */
    "naif.tarafdar@mail.utoronto.ca",                 /* First member email address */

    "Jordan Zannier",                           /* Second member full name */
    "",                           /* Second member student number */
    "jordan.zannier@mail.utoronto.ca"                            /* Second member email address */
};

unsigned num_threads;
unsigned samples_to_skip;

class sample;

class sample {
  unsigned my_key;
 public:
  sample *next;
  unsigned count;

  sample(unsigned the_key){my_key = the_key; count = 0;};
  unsigned key(){return my_key;}
  void print(FILE *f){printf("%d %d\n",my_key,count);}
};

// This instantiates an empty hash table
// it is a C++ template, which means we define the types for
// the element and key value here: element is "class sample" and
// key value is "unsigned".  
hash<sample,unsigned> h;
pthread_mutex_t lock;

void* stream_process(void* thread_id){
  int i,j,k;
  int rnum;
  unsigned key;
  sample *s;
  
  pthread_mutex_lock(&lock);
  long tid = (long)thread_id;
  int start = tid*(NUM_SEED_STREAMS/num_threads);
  // process streams starting with different initial numbers
  for (i=tid*start; i<start+(NUM_SEED_STREAMS/num_threads); i++){
    rnum = i;
    // collect a number of samples
    for (j=0; j<SAMPLES_TO_COLLECT; j++){

      // skip a number of samples
      for (k=0; k<samples_to_skip; k++){
    	rnum = rand_r((unsigned int*)&rnum);
      }

      // force the sample to be within the range of 0..RAND_NUM_UPPER_BOUND-1
      key = rnum % RAND_NUM_UPPER_BOUND;
//beginning of critical section
      // if this sample has not been counted before
      if (!(s = h.lookup(key))){
	
    	// insert a new element for it into the hash table
  //      pthread_mutex_unlock(&lock);
	    s = new sample(key);
    //    pthread_mutex_lock(&lock);
	    h.insert(s);
      }

      // increment the count for the sample
      s->count++;

//end of critical section
      
    }
  }
 pthread_mutex_unlock(&lock);
 return NULL;  

}


int  
main (int argc, char* argv[]){
 
  long i; 
  // Print out team information
  printf( "Team Name: %s\n", team.team );
  printf( "\n" );
  printf( "Student 1 Name: %s\n", team.name1 );
  printf( "Student 1 Student Number: %s\n", team.number1 );
  printf( "Student 1 Email: %s\n", team.email1 );
  printf( "\n" );
  printf( "Student 2 Name: %s\n", team.name2 );
  printf( "Student 2 Student Number: %s\n", team.number2 );
  printf( "Student 2 Email: %s\n", team.email2 );
  printf( "\n" );

  // Parse program arguments
  if (argc != 3){
    printf("Usage: %s <num_threads> <samples_to_skip>\n", argv[0]);
    exit(1);  
  }
  sscanf(argv[1], " %d", &num_threads); // not used in this single-threaded version
  sscanf(argv[2], " %d", &samples_to_skip);

  // initialize a 16K-entry (2**14) hash of empty lists
  h.setup(14);


  pthread_mutex_init(&lock, NULL);

  pthread_t *tid = (pthread_t *)malloc(sizeof(pthread_t)*num_threads);
  for(i=0; i<num_threads; i++)
    pthread_create(&tid[i], NULL,stream_process, (void*)i);

  for(i=0; i<num_threads; i++)  
    pthread_join(tid[i], NULL);

    
  // print a list of the frequency of all samples
  h.print();

  free(tid);
}



