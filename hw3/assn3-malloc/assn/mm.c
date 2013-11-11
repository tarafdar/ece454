/*
 * Our dynamic memory solution implements a segregated free list
 * We have 15 segregated free lists, with  the first list having a range from 32 bytes to 63 bytes
 * Each subsequent free list is double the range from the previous free list.  We also have our
 * implicit list to keep track of the entire heap.
 * The mm_malloc function looks for an available block in the correct free list (with respect to size) using first fit,
 * if a free block of the requested size cannot be found, move to the next list. If none of the lists can support
 * our block we extend the heap by the maximum between the requested size and chunksize (4096 bytes). 
 * After a free block has been found (either in the free list or by extending the heap) we split the free block into 
 * an allocated portion and a free portion.  We can choose to put the allocated portion of the free block on either
 * the left hand side or the right hand side.  We decide this by looking at the two neighbouring allocated blocks and calculating
 * the average size between them. If the allocated portion of our new block is larger than the average we would put it on the left
 * side, else we put it on the right side. This is to ensure that similar sized blocks are together in memory and this reduces
 * fragmentation.  
 * In mm_free we put the new free block in the beginning of the appropriate free list (with respect to size). We then coalesce,
 * which involves merging the blocks and removing the old free blocks from their respective free lists.  After coalescing we put the
 * newly merged block into it's correct free list.
 * In mm_realloc we wanted to remove the mem_copy overhead as much as possible.  We checked a few cases before resorting to copying
 * the old contents into a newly allocated block.  The first case we looked at was if the requested size is smaller than the current 
 * block size.  If so we return the same pointer after we split the block.  The second case was if the neighbouring free block and 
 * the current block can support the new size, we would merge the blocks into an allocated block and then split the remainder. 
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
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
#define CHUNKSIZE   (1<<12)      /* initial heap size (bytes) */
#define OVERHEAD    DSIZE     /* overhead of header and footer (bytes) */

#define MAX(x,y) ((x) > (y)?(x) :(y))

/* Pack a size and allocated bit into a word */
#define PACK(size, alloc) ((size) | (alloc))

/* Read and write a word at address p */
#define GET(p)          (*(uintptr_t *)(p))
#define PUT(p,val)      (*(uintptr_t *)(p) = (val))

/* Read and write a pointer at address p */
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

/* Given block ptr bp, compute address of next and previous blocks in the free list*/
#define NEXT_FREE_BLOCK(bp) (GETP(bp))
#define PREV_FREE_BLOCK(bp) (GETP(bp + WSIZE))

/* Given block ptr bp, compute address of next block's previous pointer*/
#define NEXT_FREE_BLOCK_PREV(bp) (GETP(bp) + 1)

/* Given block ptr bp, compute address of the right-hand block after a split*/
#define NEW_SPLIT_BLOCK(bp,asize) ((void*)((char*)bp + asize))

/* Given block ptr bp, on the free list compute address of the previous pointer of the block*/
#define CURR_FREE_BLOCK_PREV(bp) (bp + WSIZE)

/* alignment */
#define ALIGNMENT 16
/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0xf)

/* number of free lists */
#define NUM_FREE_LISTS 15 

/* initial request of memory (in words) includes room for all free lists head,
prologue, and epilogue*/ 
#define INITIAL_HEAP (NUM_FREE_LISTS + 3)

/*beginning address of heap*/
#define BEGIN_HEAP (NUM_FREE_LISTS + 1)

/*the highest size the first list can store*/
#define MAX_OF_FIRST_LIST 64

/*When debug is turned on this will turn on mm_check calls in mm_malloc, mm_free and mm_realloc*/
/*We have it commented out by default*/
//#define DEBUG

int mm_check(void);

/*GLOBAL VARIABLES*/

/*heap_listp points to the beginning of the heap*/
void* heap_listp = NULL;
/*free_listp points to the beginning of the free_list headers*/
void* free_listp = NULL;

/**********************************************************
 * mm_init
 * Initialize the heap, including "allocation" of the
 * prologue, epilogue and the free list pointers
 **********************************************************/
 int mm_init(void)
 {
   //extend the heap to make room for the free lists, prologue and the epilogue
   if ((heap_listp = mem_sbrk((INITIAL_HEAP)*WSIZE)) == (void *)-1)
         return -1;
     int i;
    
    // initialize free list pointers to 0 (NULL)
     for(i=0; i<NUM_FREE_LISTS; i++){
        PUT(heap_listp + (i * WSIZE), 0);       

     }
    //set free list pointer to point to the beginning of the lists
    free_listp=heap_listp;

    PUT(heap_listp +(NUM_FREE_LISTS * WSIZE), PACK(OVERHEAD, 1));   // prologue header
    PUT(heap_listp + ((NUM_FREE_LISTS+1) * WSIZE), PACK(OVERHEAD, 1));   // prologue footer
    PUT(heap_listp + ((NUM_FREE_LISTS + 2) * WSIZE), PACK(0, 1));    // epilogue header
    
    //move the heap pointer to point to the footer of the prologue, after all the free lists
    heap_listp += BEGIN_HEAP*WSIZE;

    return 0;
 }

/**********************************************************
 * find_list
 * This function uses the size to figure out which segregated list to look into
 * this returns the the index of the list with appropriate size (index is the offset from the beginning of the free list pointers)
 **********************************************************/
int find_list(size_t asize)
{
    int i;

    //set currSize to 64, this is just out of the range of the first list
    
    int currSize = MAX_OF_FIRST_LIST;
    int retVal = -1;
    
    //if asize is less than currSize then we know at that iteration our index will point to the correct list
    for(i=0; i<NUM_FREE_LISTS; i++){
        if(asize < (currSize)){
           retVal=i;
           break;
        }
        currSize = currSize << 1;
    }
    
    //if the size requested is larger than the maximum of the last list put it in the last list
    if(retVal == -1)
        retVal = NUM_FREE_LISTS - 1;
    
    //returns the pointer to the free list we are looking at (multiplied by WSIZE since each list is a 1 word pointer)
    return retVal * WSIZE;

} 

/**********************************************************
 * remove_from_free_list
 * This function removes a block pointed to by bp from the appropriate free list
 * We determine the free list by looking at the size that is also sent in
 **********************************************************/
void remove_from_free_list(void *bp, size_t bsize){
  void* list= free_listp + find_list(bsize); //find the free list

  if(PREV_FREE_BLOCK(bp) == 0){ //if the block that we want to free is the first block of the list
    PUTP(list, NEXT_FREE_BLOCK(bp)); //have the list point to the next of the current block
    if(NEXT_FREE_BLOCK(bp)!=0) //if the next block is not null then have his previous point to NULL
        PUTP(NEXT_FREE_BLOCK_PREV(bp), 0); //set the next block's previous to point to NULL
  }
  else{
    PUTP(PREV_FREE_BLOCK(bp),NEXT_FREE_BLOCK(bp)); //set the next pointer of the previous to point to the next of the current block
    if(NEXT_FREE_BLOCK(bp)!=0) //if the next block is not null then have his previous point to NULL
        PUTP(NEXT_FREE_BLOCK_PREV(bp), PREV_FREE_BLOCK(bp)); //set the next block's previous to point to the previous block
   }        
}

/**********************************************************
 * add_to_free_list
 * This function adds a block pointed to by bp to the appropriate free list
 * The new block is always added to the head of the free list
 * We determine the free list by looking at the size that is also sent in
 **********************************************************/
void add_to_free_list(void *bp, size_t bsize) {
  void* list= free_listp + find_list(bsize); //find the free list

  PUTP(bp, GETP(list)); //set the next value of the free block to point to the head of list
    
  if(NEXT_FREE_BLOCK(bp)!= 0) // check if next block is null 
    PUTP(NEXT_FREE_BLOCK_PREV(bp), bp); //set next block's prev to point to the current block
  PUTP(CURR_FREE_BLOCK_PREV(bp), 0); //set current blocks prev to point to NULL    
  PUTP(list, bp); //set head of list to point to curr block
  
}

/**********************************************************
 * coalesce
 * Covers  4 cases:
 * Case 1)both neighbours are allocated
 * Case 2)the next block is available for coalescing
 * Case 3)the previous block is available for coalescing
 * Case 4)both neighbours are available for coalescing
 * This merges neighbouring free blocks into one large free block
 * Once a match has been found the sizes are combined , the previously free block(s)
 * is removed from it's respective free list. It is deallocated and will be added to 
 * the appropriate free list outside this function.
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
        //remove the next block from it's free list
        bsize_next = GET_SIZE(HDRP(NEXT_BLKP(bp)));              
        remove_from_free_list(NEXT_BLKP(bp), bsize_next);
        
        //the new size will be the combination of the current and next block's sizes  
        size += bsize_next;
        //deallocate the block
        PUT(HDRP(bp), PACK(size, 0));
        PUT(FTRP(bp), PACK(size, 0));
        return (bp);
    }

    else if (!prev_alloc && next_alloc) { /* Case 3 */
        //remove the previous block from it's free list
        bsize_prev = GET_SIZE(HDRP(PREV_BLKP(bp)));              
        remove_from_free_list(PREV_BLKP(bp), bsize_prev);
        
          
        //the new size will be the combination of the current and previous block's sizes  
        size += bsize_prev;

        //deallocate the block
        PUT(FTRP(bp), PACK(size, 0));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        
        //return the pointer of the previous block as that is now the new pointer of the coalesced block
        return (PREV_BLKP(bp));
    }

    else {            /* Case 4 */
        //remove both previous and next blocks from their free lists
        bsize_next = GET_SIZE(HDRP(NEXT_BLKP(bp)));              
        bsize_prev = GET_SIZE(HDRP(PREV_BLKP(bp)));              
        remove_from_free_list(NEXT_BLKP(bp), bsize_next);  
        remove_from_free_list(PREV_BLKP(bp), bsize_prev);  
        
        //the new size will be the combination of the current, previous and next block's sizes  
        size += bsize_prev  + bsize_next;
        
        //deallocate the block
        PUT(HDRP(PREV_BLKP(bp)), PACK(size,0));
        PUT(FTRP(NEXT_BLKP(bp)), PACK(size,0));
        
        //return the pointer of the previous block as that is now the new pointer of the coalesced block
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
    PUT(HDRP(bp), PACK(size, 0));                // free block header
    PUT(FTRP(bp), PACK(size, 0));                // free block footer
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1));        // new epilogue header

    /* Coalesce if the previous block was free */
    bp = coalesce(bp);
    return bp;
}


/**********************************************************
 * find_fit
 * Traverse the appropriate free list for a free block of asize
 * If no block in original list can be found we check the next higher size free list
 * If no block can be found in any of the free lists (of at least asize) then return NULL
 * Assumed that asize is aligned
 **********************************************************/
void * find_fit(size_t asize)
{
    int list_index= find_list(asize);

    //pointer of the first free list we want to check based on asize
    void *listp = free_listp + find_list(asize);
    
    //first element of the free list
    void *bp = GETP(listp);
    int i;
    
    //traverse through each free list starting from the list pointed to by listp
    //if an available block of at least size asize can't be found move to a larger capacity list 
    for(i=list_index; i<NUM_FREE_LISTS*WSIZE; i+=WSIZE){ 
        //iterate through each of the blocks in the free list 
        for (; bp!=0; bp = GETP(bp))
        {
            //found a fit in the free list, return the pointer to that block
            if (asize <= GET_SIZE(HDRP(bp)))
            {
                return bp;
            }
        }
        //couldn't find a block to fit in the list, move to the next list
        listp = free_listp + i;
        bp = GETP(listp);
    }
   
    //no block was found in all of the appropriate lists, return NULL 
    return NULL;
}

/**********************************************************
 * left_split
 * Splits the large free block into an allocated left portion and a free right portion
 * It returns a pointer to the allocated block(left hand side)
 * If removes the original free block from it's free list(if it was previously in a free list)
 * It add's the right hand portion to it's respective free list
 * Assumed that asize is aligned
 **********************************************************/
void* left_split(void* bp, size_t asize, size_t bsize, size_t split_size, bool split_from_flist)
{
  //allocate the new allocated block (left hand side)
  PUT(HDRP(bp), PACK(asize, 1));
  PUT(FTRP(bp), PACK(asize, 1));

  //if the block was previously in the free list remove it
  if (split_from_flist)  
    remove_from_free_list(bp, bsize);
   
  //pointer to the new free block (right hand side)
  void* split_bp = NEW_SPLIT_BLOCK(bp,asize);
  
  //deallocate the new free block
  PUT(HDRP(split_bp), PACK(split_size,0));
  PUT(FTRP(split_bp), PACK(split_size,0));
  
  //add the new free block to it's free list
  add_to_free_list(split_bp, split_size);

  //return pointer to the allocated block (left hand side)
  return bp;
}

/**********************************************************
 * right_split
 * Splits the large free block into an allocated right portion and a free left portion
 * It returns a pointer to the allocated block (right hand side)
 * If removes the original free block from it's free list(if it was previously in a free list)
 * It add's the left hand portion to it's respective free list
 * Assumed that asize is aligned
 **********************************************************/
void* right_split(void* bp, size_t asize, size_t bsize, size_t split_size, bool split_from_flist)
{

  //if previously in free list remove from free list
  if (split_from_flist)  
    remove_from_free_list(bp, bsize);
  //create the new free block(left hand side), and add it to the free list
  PUT(HDRP(bp), PACK(split_size, 0));
  PUT(FTRP(bp), PACK(split_size, 0));
  add_to_free_list(bp, split_size);
  
  //allocate the new allocated block (right hand side)
  void* split_bp = NEW_SPLIT_BLOCK(bp,split_size);
  PUT(HDRP(split_bp), PACK(asize,1));
  PUT(FTRP(split_bp), PACK(asize,1));
  
  
  //return pointer to the allocated block (right hand side)
  return split_bp; 
}

/**********************************************************
 * place
 * Checks to see if we can split and calls the appropriate split function
 * If we can split we check the following:
 *  -we check to see if it's the first and second memory allocation and if so
 *   allocate each block to opposite sides of the initial heap. This is to set up the heap 
 *   so that later calls to this function can use the correct average block size
 * - calculate the average block size of the two neighbouring blocks, if the current block is larger than 
 *   the average we would place the allocated portion on the left side (left_split) else we would put it on
 *   the right side (right_split). This is to keep all the larger blocks together which helps with coalescing and reduces fragmentation.
 * - If this function was called by realloc then it will always be a left split as we want to keep the same initial pointer
 **********************************************************/
void* place(void* bp, size_t asize, bool place_from_flist, bool realloc)
{
  /* Get the current block size */
  size_t bsize = GET_SIZE(HDRP(bp));
  size_t split_size = bsize - asize;
  void* ret_ptr = bp;
  int avg_size;

  //Not enough room for anything else except some overhead, do not split
  if (split_size <= OVERHEAD*2)  {
    
    //allocate the block
    PUT(HDRP(bp), PACK(bsize, 1));
    PUT(FTRP(bp), PACK(bsize, 1));
    
    //remove from free list if the block was from the free list (not if was allocated with extend heap)
    if (place_from_flist)
        remove_from_free_list(bp, bsize);
      
  }
  //room to split
  else {
    //place after realloc, put the block on the left
    if(realloc)
       ret_ptr = left_split(bp, asize, bsize, split_size, place_from_flist);
    else {  
        //calculate average size using the sizes of the two neighbouring allocated blocks
        avg_size = (GET_SIZE(HDRP(NEXT_BLKP(bp))) + GET_SIZE(HDRP(PREV_BLKP(bp))))/ 2;

        //larger block: place on the left side
        if (asize >= avg_size)
            ret_ptr = left_split(bp, asize, bsize, split_size, place_from_flist);
        //if not larger block: place on the right side    
        else
            ret_ptr = right_split(bp, asize, bsize, split_size, place_from_flist);
    }        
  }
  
  //return the pointer to the allocated block (after the potential splitting)
  return ret_ptr;
}

/**********************************************************
 * mm_free
 * Free the block and coalesce with neighbouring blocks
 * After coalescing add the free block to the appropriate free list
 **********************************************************/
void mm_free(void *bp)
{
    #ifdef DEBUG
    if(mm_check()==0){
       printf("heap inconsistent\n");
       exit(1);
    }
    #endif
    
    //if calling free on a null pointer do nothing
    if(bp == NULL){
      return;
    }
    bp = coalesce(bp);

    
    //calculate size of block after coalescing
    size_t size = GET_SIZE(HDRP(bp));
    
    //deallocate the block
    PUT(HDRP(bp), PACK(size,0));
    PUT(FTRP(bp), PACK(size,0));


    //add to free list

    add_to_free_list(bp, size);
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
    
    #ifdef DEBUG
    if(mm_check()==0){
       printf("heap inconsistent\n");
       exit(1);
    }
    #endif
    
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
        //call place and tell it the block is from a free list
        bp = place(bp, asize, 1, 0);
        return bp;
    }

    /* No fit found. Get more memory and place the block */
    extendsize = MAX(asize, CHUNKSIZE);
    
    if ((bp = extend_heap(extendsize/WSIZE)) == NULL)
        return NULL;
    //call place and tell it the block is not from a free list
    bp = place(bp, asize, 0, 0);
    
    return bp;

}

/**********************************************************
 * mm_realloc
 * Checks a few cases:
 * Case 1) if size is 0, call free, return null
 * Case 2) if pointer is null, call malloc
 * Case 3) if requested size is less than the size of the block,
 *         split block and return original pointer
 * Case 4) if requested size is larger than size of block but smaller than
 *         then the sum of the current block and neighbouring free block,
 *         merge both blocks, then split blocks and return original pointer
 * Case 5) If none of the previous cases happen, allocate new block of correct size and mem_copy old
           contents into new block and free old block. 
 *********************************************************/
void *mm_realloc(void *ptr, size_t size)
{
    
    void *oldptr = ptr;
    void *newptr;
    size_t copySize;
    size_t nextSize = GET_SIZE(HDRP(NEXT_BLKP(ptr)));              
    size_t currSize = GET_SIZE(HDRP(ptr));
    void * next = NEXT_BLKP(ptr);
    size_t asize;
    
    
    #ifdef DEBUG
    if(mm_check()==0){
       printf("heap inconsistent\n");
       exit(1);
    }
    #endif
    
    //calculate allocated size (factor in overhead)
    if (size <= DSIZE)
        asize = DSIZE + OVERHEAD;
    else
        asize = DSIZE * ((size + (OVERHEAD) + (DSIZE-1))/ DSIZE);
    
    //case 1
    /* If size == 0 then this is just free, and we return NULL. */
    if (size == 0){
      mm_free(ptr);
      return NULL;
    }

    //case 2
    /* If old ptr is NULL, then this is just malloc. */
    if (ptr == NULL)
      return (mm_malloc(size));
    
    //case 3
    //size requested less than size of block, split return original pointer  
    if (asize<= currSize){
      PUT(HDRP(ptr), PACK(currSize, 0));
      PUT(FTRP(ptr), PACK(currSize, 0));
      ptr = place(ptr, asize, 0, 1);
      return ptr;   
        
    }    
    //case 4
    //size requested less than size of current block + size of neighboring block
    //merge blocks then split appropriately and return original pointer
    else if((asize< currSize+nextSize) && !GET_ALLOC(HDRP(NEXT_BLKP(ptr)))){
        remove_from_free_list(next, nextSize);
        PUT(HDRP(ptr), PACK(currSize + nextSize,0));
        PUT(FTRP(ptr), PACK(currSize + nextSize ,0));
        ptr = place(ptr, asize, 0, 1);
        return ptr;

    }
   
   
    //case 5
    //allocate new block, mem_copy old block data into new block and free old block 
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
 * in_free_list
 * Checks the correct free_list to see if the block that was in the implicit list
 * can be found in the correct free_list. 
 * Returns 1 if found, else returns 0
 *********************************************************/

bool in_free_list(void * bp_implicit, size_t bsize){

  void* list= free_listp + find_list(bsize); //find the free list
  void *bp; 
  bp = GETP(list);  
  for (; bp!=0; bp = GETP(bp)){
    if (bp==bp_implicit)
        return 1;

  }

  return 0;

}


/**********************************************************
 * in_implicit_list
 * Checks the entire heap(implicit list) to see if it has the block pointed to by a free list
 * Returns 1 if found, else returns 0
 *********************************************************/

bool in_implicit_list(void * bp_freelist) {
    void *bp;
    
    for (bp = heap_listp; GET_SIZE(HDRP(bp)) > 0; bp = NEXT_BLKP(bp)){
        if(bp == bp_freelist) 
            return 1;
    }
    
    return 0;
}


/**********************************************************
 * mm_check
 * Check the consistency of the memory heap
 * Return nonzero if the heap is consistant.
 *********************************************************/

int mm_check(void){
  void *bp;
  size_t bsize;
  int num_free_blocks_implicit = 0;
  int num_free_blocks_free_list = 0;
  bool is_prev_free = 0;
  void* listp;
  void* prev;
  void *epilogue;
  int i;
   


  //implicit list (entire heap checks) 
  //traverse through heap via the implicit list
  //We will be checking
  //a) If two consecutive blocks are free (checking to see if coalescing worked) 
  //b) how many free nodes are in the list 
  //   (we will compare this later with how many free nodes are in the segregated free lists)
  //c) if every free node in the heap can be found in the appropriate segregated free list
  //d) if the header and footer of every block matches in the heap
  //e) if the address of each block is 16 b aligned
  //f) if the size of each block is 16 b aligned

  for (bp = heap_listp; GET_SIZE(HDRP(bp)) > 0; bp = NEXT_BLKP(bp)){
      bsize = GET_SIZE(HDRP(bp));
     
      //if block is free 
      if(!GET_ALLOC(HDRP(bp))){
        //two consecutive free blocks (failed coalescing), heap is inconsistent
        if(is_prev_free)
            return 0;
        
        //count how many free blocks in the implicit list
        num_free_blocks_implicit++;
       
        //looks for the free block in the appropriate free list
        //if free block can't be found in free list heap is incosistent
        if(!in_free_list(bp, bsize)){
            return 0;   
        }
        is_prev_free = 1; 

      }
      else{
        is_prev_free = 0;

      }
      //if footer doesn't match the header heap is incosistent
      if(GET_SIZE(FTRP(bp)) != GET_SIZE(HDRP(bp)))
        return 0;
      
      if(GET_ALLOC(FTRP(bp)) != GET_ALLOC(HDRP(bp)))
        return 0;
        
      
      //if address is not 16 b aligned heap is inconsistent  
      if(((uintptr_t)bp) % ALIGNMENT != 0)
          return 0;
          
      //if size of block is not divisible by 16 heap is inconsistent    
      if(bsize % ALIGNMENT != 0)
          return 0;        
      
   }


   epilogue = bp;

  //for each free list
    //for each block in a free list
        //check if each block is 16 b aligned
        //check that block is in correct segregated list by size
        //check that the block can be found in the implicit list
        //check that the header and the footer match
        //check that the block is free 
        //check that the previous of the current block correctly points to the last blocks next pointer
        //check that the block is within the bounds of the heap
  
   for(i=0; i<NUM_FREE_LISTS*WSIZE; i+=WSIZE){
        prev = NULL;  
        listp = free_listp + i;
        bp = GETP(listp);
        for (; bp!=0; bp = NEXT_FREE_BLOCK(bp)){
            bsize = GET_SIZE(HDRP(bp));
            //double word alligned address            
            if(((uintptr_t)bp) % DSIZE != 0)
                return 0;
            //size of block is double word aligned    
            if(bsize % DSIZE != 0)
                return 0;    
            //element is in wrong list
            if (i != find_list(bsize))
                return 0;
            //can be found in the heap       
            if (!in_implicit_list(bp))
                return 0;
            //header and footer match    
            if (GET_SIZE(HDRP(bp)) != GET_SIZE(FTRP(bp)))
                return 0;
            if (GET_ALLOC(HDRP(bp)) != GET_ALLOC(FTRP(bp)))
                return 0;
            //block is not allocated    
            if (GET_ALLOC(HDRP(bp)) != 0)
                return 0;
            //prev of the current block is equal to the pointer of the last block executed in loop    
            if (prev != PREV_FREE_BLOCK(bp))
                return 0;
            //past the end of the heap    
            if (bp > epilogue)
                return 0;
            //past the beginning of the heap    
            if (bp < heap_listp)
                return 0;    
            num_free_blocks_free_list++;
            prev = bp;        
        }
  }
  //compare count of free_blocks in free list to free blocks in implicit list
  if (num_free_blocks_free_list != num_free_blocks_implicit)
    return 0;
  return 1;
}

