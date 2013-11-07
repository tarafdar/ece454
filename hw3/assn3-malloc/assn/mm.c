/*
 * This implementation replicates the implicit list implementation
 * provided in the textbook
 * "Computer Systems - A Programmer's Perspective"
 * Blocks are never coalesced or reused.
 * Realloc is implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "Naif_and_Jordan",
    /* First member's full name */
    "Naif Tarafdar",
    /* First member's email address */
    "naif.tarafdar@mail.utoronto.ca",
    /* Second member's full name (leave blank if none) */
    "Jordan Zannier",
    /* Second member's email address (leave blank if none) */
    "jordan.zannier@mail.utoronto.ca"
};

/*************************************************************************
 * Basic Constants and Macros
 * You are not required to use these macros but may find them helpful.
*************************************************************************/
#define WSIZE       sizeof(void *)         /* word size (bytes) */
#define DSIZE       (2 * WSIZE)            /* doubleword size (bytes) */
#define CHUNKSIZE   (1<<7)      /* initial heap size (bytes) */
#define OVERHEAD    DSIZE     /* overhead of header and footer (bytes) */

#define MAX(x,y) ((x) > (y)?(x) :(y))

/* Pack a size and allocated bit into a word */
#define PACK(size, alloc) ((size) | (alloc))

/* Read and write a word at address p */
#define GET(p)          (*(uintptr_t *)(p))
#define PUT(p,val)      (*(uintptr_t *)(p) = (val))

#define GETP(p)          (*(uintptr_t **)(p))
#define PUTP(p,val)      (*(uintptr_t **)(p) = (uintptr_t *)val)

/* Read the size and allocated fields from address p */
#define GET_SIZE(p)     (GET(p) & ~(DSIZE - 1))
#define GET_ALLOC(p)    (GET(p) & 0x1)

/* Given block ptr bp, compute address of its header and footer */
#define HDRP(bp)        ((char *)(bp) - WSIZE)
#define FTRP(bp)        ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

/* Given block ptr bp, compute address of next and previous blocks */
#define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)))
#define PREV_BLKP(bp) ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)))

#define NEXT_FREE_BLOCK(bp) (GETP(bp))
#define PREV_FREE_BLOCK(bp) (GETP(bp + WSIZE))

#define NEXT_FREE_BLOCK_PREV(bp) (GETP(bp) + 1)
/* alignment */
#define ALIGNMENT 16
/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0xf)
#define NUM_FREE_LISTS 15
#define FREE_LIST_OVERHEAD 18
#define BEGIN_HEAP 16
void* heap_listp = NULL;
void* free_listp = NULL;
/**********************************************************
 * mm_init
 * Initialize the heap, including "allocation" of the
 * prologue and epilogue
 **********************************************************/
 int mm_init(void)
 {
   if ((heap_listp = mem_sbrk((FREE_LIST_OVERHEAD)*WSIZE)) == (void *)-1)
         return -1;
     int i;
     for(i=0; i<NUM_FREE_LISTS; i++){
        PUT(heap_listp + (i * WSIZE), 0);                         // initialize free list pointers to 0 (NULL)

     }
     free_listp=heap_listp;

    PUT(heap_listp +(NUM_FREE_LISTS * WSIZE), PACK(OVERHEAD, 1));   // prologue header
    PUT(heap_listp + ((NUM_FREE_LISTS+1) * WSIZE), PACK(OVERHEAD, 1));   // prologue footer
    PUT(heap_listp + ((NUM_FREE_LISTS + 2) * WSIZE), PACK(0, 1));    // epilogue header
    heap_listp += BEGIN_HEAP*WSIZE;

     return 0;
 }

int find_list(size_t asize)
{
    int i;

    //set currSize to 8, this is the second lowest range of free lists that we can have
    
    int currSize = 8;
    int retVal = -1;
    
    //if asize is less than currSize then we know at that iteration our index will point to the correct list
    for(i=0; i<NUM_FREE_LISTS; i++){
        if(asize < currSize){
           retVal=i;
           break;
        }
        currSize = currSize << 1;
    }
    
    if(retVal == -1)
        retVal = NUM_FREE_LISTS - 1;

    return retVal * WSIZE; //returns the pointer to the free list we are looking at

} 

void remove_from_free_list(void *bp, size_t bsize){
  void* list= free_listp + find_list(bsize); //find the free list

  if(PREV_FREE_BLOCK(bp) == 0){ //if the block that we want to free is the first block of the list
    PUTP(list, NEXT_FREE_BLOCK(bp)); //have the list point to the next of the current block
    if(NEXT_FREE_BLOCK(bp)!=0) //if the next guy is not null then have his previous point to NULL
        PUTP(NEXT_FREE_BLOCK_PREV(bp), 0); //set the next block's previous to point to NULL
  }
  else{
    PUTP(PREV_FREE_BLOCK(bp),NEXT_FREE_BLOCK(bp)); //set the next pointer of the previous to point to the next of the current block
    if(NEXT_FREE_BLOCK(bp)!=0) //if the next guy is not null then have his previous point to NULL
        PUTP(NEXT_FREE_BLOCK_PREV(bp), PREV_FREE_BLOCK(bp)); //set the next block's previous to point to the previous block
   }        



 }

/**********************************************************
 * coalesce
 * Covers the 4 cases discussed in the text:
 * - both neighbours are allocated
 * - the next block is available for coalescing
 * - the previous block is available for coalescing
 * - both neighbours are available for coalescing
 **********************************************************/
void *coalesce(void *bp)
{
    size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    size_t size = GET_SIZE(HDRP(bp));
    size_t bsize_next;
    size_t bsize_prev;
    if (prev_alloc && next_alloc) {       /* Case 1 */
        return bp;
    }

    else if (prev_alloc && !next_alloc) { /* Case 2 */
        //remove the next guy from his free list
        bsize_next = GET_SIZE(HDRP(NEXT_BLKP(bp)));              
        remove_from_free_list(NEXT_BLKP(bp), bsize_next);  
        size += bsize_next;
        PUT(HDRP(bp), PACK(size, 0));
        PUT(FTRP(bp), PACK(size, 0));
        return (bp);
    }

    else if (!prev_alloc && next_alloc) { /* Case 3 */
        //remove the prev guy from his free list
        bsize_prev = GET_SIZE(HDRP(PREV_BLKP(bp)));              
        remove_from_free_list(PREV_BLKP(bp), bsize_prev);  
        size += bsize_prev;
        PUT(FTRP(bp), PACK(size, 0));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        return (PREV_BLKP(bp));
    }

    else {            /* Case 4 */
        //remove both prev and next guy from their from list
        bsize_next = GET_SIZE(HDRP(NEXT_BLKP(bp)));              
        bsize_prev = GET_SIZE(HDRP(PREV_BLKP(bp)));              
        
        remove_from_free_list(NEXT_BLKP(bp), bsize_next);  
        remove_from_free_list(PREV_BLKP(bp), bsize_prev);  
        
        size += bsize_prev  + bsize_next;
        PUT(HDRP(PREV_BLKP(bp)), PACK(size,0));
        PUT(FTRP(NEXT_BLKP(bp)), PACK(size,0));
        return (PREV_BLKP(bp));
    }
}

/**********************************************************
 * extend_heap
 * Extend the heap by "words" words, maintaining alignment
 * requirements of course. Free the former epilogue block
 * and reallocate its new header
 **********************************************************/
void *extend_heap(size_t words)
{
    char *bp;
    size_t size;

    /* Allocate an even number of words to maintain alignments */
    size = (words % 2) ? (words+1) * WSIZE : words * WSIZE;
    if ( (bp = mem_sbrk(size)) == (void *)-1 )
        return NULL;

    /* Initialize free block header/footer and the epilogue header */
    PUT(HDRP(bp), PACK(size, 1));                // free block header
    PUT(FTRP(bp), PACK(size, 1));                // free block footer
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1));        // new epilogue header

    /* Coalesce if the previous block was free */
   // return coalesce(bp);
   return bp;
}


/**********************************************************
 * find_fit
 * Traverse the heap searching for a block to fit asize
 * Return NULL if no free blocks can handle that size
 * Assumed that asize is aligned
 **********************************************************/
void * find_fit(size_t asize)
{
    int list_index= find_list(asize);
    void *listp = free_listp + list_index;
    void *bp = GETP(listp);
    int i; 
    for(i=list_index; i<NUM_FREE_LISTS; i++){  
        for (; bp!=0; bp = GETP(bp))
        {
            if (!GET_ALLOC(HDRP(bp)) && (asize <= GET_SIZE(HDRP(bp))))
            {
                return bp;
            }
            //else if(GETP(bp)==0)
            //    break;
        }
        listp = free_listp + list_index;
        bp = GETP(listp);
    }
    return NULL;
}


void split(void* bp, size_t asize, size_t bsize, size_t split_size)
{
  PUT(HDRP(bp), PACK(asize, 1));
  PUT(FTRP(bp), PACK(asize, 1));

  remove_from_free_list(bp, bsize);
   
  //allocate the new split block into the appropriate free list
}

/**********************************************************
 * place
 * Mark the block as allocated
 **********************************************************/
void place_find_fit(void* bp, size_t asize)
{
  /* Get the current block size */
  size_t bsize = GET_SIZE(HDRP(bp));
  //size_t split_size = bsize - asize;
  //if (split_size == OVERHEAD)  { //Not enough room for anything else except some overhead, do not split
    PUT(HDRP(bp), PACK(bsize, 1));
    PUT(FTRP(bp), PACK(bsize, 1));
    //REMOVE FROM FREE LIST
  //}
  //else {
  
  //}

  //REMOVE FROM FREE LIST
  remove_from_free_list(bp, bsize);  
}

void place_extend_heap(void* bp, size_t asize)
{
  /* Get the current block size */
  size_t bsize = GET_SIZE(HDRP(bp));

  PUT(HDRP(bp), PACK(bsize, 1));
  PUT(FTRP(bp), PACK(bsize, 1));


}
/**********************************************************
 * mm_free
 * Free the block and coalesce with neighbouring blocks
 **********************************************************/
void mm_free(void *bp)
{
    if(bp == NULL){
      return;
    }

    bp = coalesce(bp);
    size_t size = GET_SIZE(HDRP(bp));
    PUT(HDRP(bp), PACK(size,0));
    PUT(FTRP(bp), PACK(size,0));
    
    //ADD TO FREE LIST
    void* list= free_listp + find_list(size); //find the free list
    PUTP(bp, GETP(list)); //set the next value of the free block to point to the head of list
    
    if(NEXT_FREE_BLOCK(bp)!= 0) // check if next guy is null 
        PUTP(NEXT_FREE_BLOCK_PREV(bp), bp); //set next guy's prev to point to the curr
    PUTP(bp + WSIZE, 0); //set curr guys prev to point to NULL    
    PUTP(list, bp); //set head of list to point to curr guy

}


/**********************************************************
 * mm_malloc
 * Allocate a block of size bytes.
 * The type of search is determined by find_fit
 * The decision of splitting the block, or not is determined
 *   in place(..)
 * If no block satisfies the request, the heap is extended
 **********************************************************/
void *mm_malloc(size_t size)
{
    size_t asize; /* adjusted block size */
    size_t extendsize; /* amount to extend heap if no fit */
    char * bp;

    /* Ignore spurious requests */
    if (size == 0)
        return NULL;

    /* Adjust block size to include overhead and alignment reqs. */
    if (size <= DSIZE)
        asize = DSIZE + OVERHEAD;
    else
        asize = DSIZE * ((size + (OVERHEAD) + (DSIZE-1))/ DSIZE);

    /* Search the free list for a fit */
    bp = find_fit(asize);
    if (bp != NULL) {
        place_find_fit(bp, asize);
        return bp;
    }

    /* No fit found. Get more memory and place the block */
    //extendsize = MAX(asize, CHUNKSIZE);
    extendsize = asize;
    if ((bp = extend_heap(extendsize/WSIZE)) == NULL)
        return NULL;
    place_extend_heap(bp, asize);
    return bp;

}

/**********************************************************
 * mm_realloc
 * Implemented simply in terms of mm_malloc and mm_free
 *********************************************************/
void *mm_realloc(void *ptr, size_t size)
{
    /* If size == 0 then this is just free, and we return NULL. */
    if (size == 0){
      mm_free(ptr);
      return NULL;
    }

    /* If old ptr is NULL, then this is just malloc. */
    if (ptr == NULL)
      return (mm_malloc(size));

    void *oldptr = ptr;
    void *newptr;
    size_t copySize;

    newptr = mm_malloc(size);
    if (newptr == NULL)
      return NULL;

    /* Copy the old data. */
    copySize = GET_SIZE(HDRP(oldptr));
    if (size < copySize)
      copySize = size;
    memcpy(newptr, oldptr, copySize);
    mm_free(oldptr);
    return newptr;
}

/**********************************************************
 * mm_check
 * Check the consistency of the memory heap
 * Return nonzero if the heap is consistant.
 *********************************************************/
int mm_check(void){
  return 1;
}

