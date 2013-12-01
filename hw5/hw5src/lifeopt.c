
/*****************************************************************************
 * life.c
 * The original sequential implementation resides here.
 * Do not modify this file, but you are encouraged to borrow from it
 ****************************************************************************/
#include "life.h"
#include "util.h"


/**
 * Swapping the two boards only involves swapping pointers, not
 * copying values.
 */
#define SWAP_BOARDS( b1, b2 )  do { \
  char* temp = b1; \
  b1 = b2; \
  b2 = temp; \
} while(0)


//#define BOARD( __board, __i, __j )  (__board[(__i) + LDA*(__j)])
#define BOARD( __board, __i, __j )  (__board[(__j) + LDA*(__i)])




void *thread_board(void * vargp){
    
    int curgen, i, j, ii, jj;
    thread_args targs = *((thread_args *)vargp);    
    int start_i, start_j;
    
    int quadrant = targs.quadrant;
    int nrows = targs.nrows;
    int ncols = targs.ncols;
    char* outboard = targs.outboard;
    char* inboard = targs.inboard;
    int gens_max = targs.gens_max;
    pthread_barrier_t * barrier = targs.barrier; 

    int block_size;

    if(ncols>=512)
        block_size = 256;
     else
         block_size = ncols/2;
    


    if(quadrant == 0 || quadrant == 1)
        start_i = 0;
    else
        start_i = nrows/2;

    if(quadrant == 0 || quadrant == 2)
        start_j = 0;
    else
        start_j = ncols/2;

    int isouth, inorth, jeast, jwest;
    char neighbor_count;
    int LDA = ncols;
    unsigned int mask = ncols - 1;

    /* HINT: you'll be parallelizing these loop(s) by doing a
       geometric decomposition of the output */
    for (curgen = 0; curgen < gens_max; curgen++)
    {
        for (i = start_i; i < start_i + nrows/2; i+= block_size)
        {
            for (j = start_j; j < start_j +  ncols/2; j+= block_size)
            {
                for (ii = i; ii<i+block_size;ii++) {   
                    //isouth = mod (ii+1, nrows);
                   // inorth = mod (ii-1, nrows);
                    isouth = (ii+1)&mask;
                    inorth = (ii-1)&mask;
                   
                    for (jj= j; jj<j+block_size;jj++){ 
                        //jwest = mod (jj-1, ncols);
                        //jeast = mod (jj+1, ncols);
                        jwest = (jj-1)&mask;
                        jeast = (jj+1)&mask;

                        neighbor_count = 
                            BOARD (inboard, inorth, jwest) + 
                            BOARD (inboard, inorth, jj) + 
                            BOARD (inboard, inorth, jeast) + 
                            BOARD (inboard, ii, jwest) +
                            BOARD (inboard, ii, jeast) + 
                            BOARD (inboard, isouth, jwest) +
                            BOARD (inboard, isouth, jj) + 
                            BOARD (inboard, isouth, jeast);

                        //BOARD(outboard, ii, jj) = alivep (neighbor_count, BOARD (inboard, ii, jj));
                        BOARD(outboard, ii, jj) = (!BOARD (inboard, ii, jj)  && (neighbor_count == (char) 3)) ||(BOARD(inboard, ii, jj) && (neighbor_count >= 2) && (neighbor_count <= 3));
                    }
                }    
            }
        }
        pthread_barrier_wait(barrier);          
        SWAP_BOARDS( outboard, inboard );

     }



    return NULL;

}



    char*
optimized_game_of_life (char* outboard, 
        char* inboard,
        const int nrows,
        const int ncols,
        const int gens_max)
{
    /* HINT: in the parallel decomposition, LDA may not be equal to
       nrows! */
    int i;
    pthread_t thrd[NUM_THREADS];
    thread_args targs[NUM_THREADS];
    pthread_barrier_t barrier;
    pthread_barrier_init(&barrier, NULL, NUM_THREADS);
    
    
    
    for(i=0; i<NUM_THREADS;i++){
       targs[i].barrier = &barrier;
       targs[i].quadrant = i;
       targs[i].inboard = inboard;
       targs[i].outboard = outboard;
       targs[i].nrows = nrows;
       targs[i].ncols = ncols;
       targs[i].gens_max = gens_max;
       pthread_create(&thrd[i], NULL, thread_board, (void*)&(targs[i]));
    }
    
    
    for(i=0; i<NUM_THREADS;i++){
        pthread_join(thrd[i], NULL);
    }

    /* 
     * We return the output board, so that we know which one contains
     * the final result (because we've been swapping boards around).
     * Just be careful when you free() the two boards, so that you don't
     * free the same one twice!!! 
     */
    return inboard;
}


