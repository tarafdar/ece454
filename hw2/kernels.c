/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
#include "math.h"
void attempt_three(int dim, pixel *src, pixel *dst); 
void attempt_four(int dim, pixel *src, pixel *dst); 
void attempt_five(int dim, pixel *src, pixel *dst); 
void attempt_six(int dim, pixel *src, pixel *dst); 
void attempt_seven(int dim, pixel *src, pixel *dst); 
void attempt_eight(int dim, pixel *src, pixel *dst); 
void attempt_nine(int dim, pixel *src, pixel *dst); 
void attempt_ten(int dim, pixel *src, pixel *dst); 

/* 
 * ECE454 Students: 
 * Please fill in the following team struct 
 */
team_t team = {
    "Naif_and_Jordan",              /* Team name */

    "Naif Tarafdar",     /* First member full name */
    "naif.tarafdar@mail.utoronto.ca",  /* First member email address */

    "Jordan Zannier",                   /* Second member full name (leave blank if none) */
    "jordan.zannier@mail.utoronto.ca"                    /* Second member email addr (leave blank if none) */
};

/***************
 * ROTATE KERNEL
 ***************/

/******************************************************
 * Your different versions of the rotate kernel go here
 ******************************************************/

/* 
 * naive_rotate - The naive baseline version of rotate 
 */
char naive_rotate_descr[] = "naive_rotate: Naive baseline implementation";
void naive_rotate(int dim, pixel *src, pixel *dst) 
{
    int i, j;
    for (i = 0; i < dim; i++)
	for (j = 0; j < dim; j++)
	    dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];
}

/*
 * ECE 454 Students: Write your rotate functions here:
 */ 

/* 
 * rotate - Your current working version of rotate
 * IMPORTANT: This is the version you will be graded on
 */
char rotate_descr[] = "rotate: Current working version";
void rotate(int dim, pixel *src, pixel *dst) 
{
    //this is equivalent to attempt 10
    int i_block, j_block;
    int dim_min_one_min_j_mult_dim;
    int i, j;
    int blocksize = 16;
//    if (dim%64==0)
//	blocksize=64;

    if (dim>=1024 && dim%128==0)
    //if (dim%128==0)
	blocksize = 128;

    if(dim>=1024){
    for (j_block = 0; j_block < dim; j_block+=blocksize){
	for (i_block = 0; i_block < dim; i_block+=blocksize){
	    for(j=j_block; j<j_block+blocksize;j+=8){   
	        for(i=i_block; i<i_block+blocksize;i+=8){
	        	dim_min_one_min_j_mult_dim = (dim - 1 - j) * dim;
	    		dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 1] = src[RIDX(i + 1, j, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 2] = src[RIDX(i + 2, j, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 3] = src[RIDX(i + 3, j, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 4] = src[RIDX(i + 4, j, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 5] = src[RIDX(i + 5, j, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 6] = src[RIDX(i + 6, j, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 7] = src[RIDX(i + 7, j, dim)];
	        	
	        	dim_min_one_min_j_mult_dim = (dim - 1 - j - 1) * dim;
	    		dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j + 1, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 1] = src[RIDX(i + 1, j + 1, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 2] = src[RIDX(i + 2, j + 1, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 3] = src[RIDX(i + 3, j + 1, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 4] = src[RIDX(i + 4, j + 1, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 5] = src[RIDX(i + 5, j + 1, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 6] = src[RIDX(i + 6, j + 1, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 7] = src[RIDX(i + 7, j + 1, dim)];
		
	        	dim_min_one_min_j_mult_dim = (dim - 1 - j - 2) * dim;
	    		dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j + 2, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 1] = src[RIDX(i + 1, j + 2, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 2] = src[RIDX(i + 2, j + 2, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 3] = src[RIDX(i + 3, j + 2, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 4] = src[RIDX(i + 4, j + 2, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 5] = src[RIDX(i + 5, j + 2, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 6] = src[RIDX(i + 6, j + 2, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 7] = src[RIDX(i + 7, j + 2, dim)];
		

	        	dim_min_one_min_j_mult_dim = (dim - 1 - j - 3) * dim;
	    		dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j + 3, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 1] = src[RIDX(i + 1, j + 3, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 2] = src[RIDX(i + 2, j + 3, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 3] = src[RIDX(i + 3, j + 3, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 4] = src[RIDX(i + 4, j + 3, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 5] = src[RIDX(i + 5, j + 3, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 6] = src[RIDX(i + 6, j + 3, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 7] = src[RIDX(i + 7, j + 3, dim)];
	        	
			dim_min_one_min_j_mult_dim = (dim - 1 - j - 4) * dim;
	    		dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j + 4, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 1] = src[RIDX(i + 1, j + 4, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 2] = src[RIDX(i + 2, j + 4, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 3] = src[RIDX(i + 3, j + 4, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 4] = src[RIDX(i + 4, j + 4, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 5] = src[RIDX(i + 5, j + 4, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 6] = src[RIDX(i + 6, j + 4, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 7] = src[RIDX(i + 7, j + 4, dim)];
			
			dim_min_one_min_j_mult_dim = (dim - 1 - j - 5) * dim;
	    		dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j + 5, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 1] = src[RIDX(i + 1, j + 5, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 2] = src[RIDX(i + 2, j + 5, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 3] = src[RIDX(i + 3, j + 5, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 4] = src[RIDX(i + 4, j + 5, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 5] = src[RIDX(i + 5, j + 5, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 6] = src[RIDX(i + 6, j + 5, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 7] = src[RIDX(i + 7, j + 5, dim)];
			
			dim_min_one_min_j_mult_dim = (dim - 1 - j - 6) * dim;
	    		dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j + 6, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 1] = src[RIDX(i + 1, j + 6, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 2] = src[RIDX(i + 2, j + 6, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 3] = src[RIDX(i + 3, j + 6, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 4] = src[RIDX(i + 4, j + 6, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 5] = src[RIDX(i + 5, j + 6, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 6] = src[RIDX(i + 6, j + 6, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 7] = src[RIDX(i + 7, j + 6, dim)];
			
			dim_min_one_min_j_mult_dim = (dim - 1 - j - 7) * dim;
	    		dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j + 7, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 1] = src[RIDX(i + 1, j + 7, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 2] = src[RIDX(i + 2, j + 7, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 3] = src[RIDX(i + 3, j + 7, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 4] = src[RIDX(i + 4, j + 7, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 5] = src[RIDX(i + 5, j + 7, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 6] = src[RIDX(i + 6, j + 7, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 7] = src[RIDX(i + 7, j + 7, dim)];
		}
	     }
	}

    }
    }
    else{
    for (j_block = 0; j_block < dim; j_block+=blocksize){
	for (i_block = 0; i_block < dim; i_block+=blocksize){
	    for(j=j_block; j<j_block+blocksize;j++){   
	        dim_min_one_min_j_mult_dim = (dim - 1 - j) * dim;
	        for(i=i_block; i<i_block+blocksize;i++)
	    		dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j, dim)];
	     }
	}

    }



    }
}


char rotate_two_descr[] = "second attempt minor unroll of the outer loop and removal of loop invariance";
void attempt_two(int dim, pixel *src, pixel *dst) 
{
    //naive_rotate(dim, src, dst);
    int i, j;
    int i_mult_dim;
    int src_index;
    int dst_index;
    int dim_min_1 = dim*dim - dim;
    int dim_min_1_plus_i;
    int j_dim;

    for (i = 0; i < dim; i+=4){
	i_mult_dim = i*dim;
	dim_min_1_plus_i = dim_min_1 +i;
	for (j = 0; j < dim; j++){
	    src_index= i_mult_dim+j;
	    j_dim=j*dim;
	    dst_index=dim_min_1_plus_i - j_dim;
	    dst[dst_index] = src[src_index];
      
    	}
	i_mult_dim = (i+1)*dim;
	dim_min_1_plus_i = dim_min_1 +i+1;
	for (j = 0; j < dim; j++){
	    src_index= i_mult_dim+j;
	    j_dim=j*dim;
	    dst_index=dim_min_1_plus_i - j_dim;
	    dst[dst_index] = src[src_index];
      
    	}
	i_mult_dim = (i+2)*dim;
	dim_min_1_plus_i = dim_min_1 +i+2;
	for (j = 0; j < dim; j++){
	    src_index= i_mult_dim+j;
	    j_dim=j*dim;
	    dst_index=dim_min_1_plus_i - j_dim;
	    dst[dst_index] = src[src_index];
      
    	}
	i_mult_dim = (i+3)*dim;
	dim_min_1_plus_i = dim_min_1 +i+3;
	for (j = 0; j < dim; j++){
	    src_index= i_mult_dim+j;
	    j_dim=j*dim;
	    dst_index=dim_min_1_plus_i - j_dim;
	    dst[dst_index] = src[src_index];
      
    	}



    }
}

char rotate_three_descr[] = "third attempt no unroll fixed loop invariance";
void attempt_three(int dim, pixel *src, pixel *dst) 
{
    int i, j;
    int i_mult_dim;
    int dim_squared_minus_dim =dim*dim -dim;
    int dim_squared_minus_dim_plus_i;
    pixel * src_temp;
    for (i = 0; i < dim; i++){
	i_mult_dim = i*dim;
        dim_squared_minus_dim_plus_i= dim_squared_minus_dim + i;
	src_temp = src+i_mult_dim;
	for (j = 0; j < dim; j++){

	    dst[dim_squared_minus_dim_plus_i -j*dim] = src_temp[j];
	}
    }

}

char rotate_four_descr[] = "fourth attempt: switch i and j code motion";
void attempt_four(int dim, pixel *src, pixel *dst) 
{
    int i, j;
    int dim_min_one_min_j_mult_dim;



    for (j = 0; j < dim; j++){
	dim_min_one_min_j_mult_dim = (dim - 1 -j) *dim;
	for (i = 0; i < dim; i++)
	    dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j, dim)];

    }
}

char rotate_five_descr[] = "five attempt: attempt 4 plus unroll";
void attempt_five(int dim, pixel *src, pixel *dst) 
{
    int i, j;
    int dim_min_one_min_j_mult_dim;



    for (j = 0; j < dim; j+=8){
	for (i = 0; i < dim; i+=8){
	    dim_min_one_min_j_mult_dim = (dim - 1 -j) *dim;
	    dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 1] = src[RIDX(i + 1, j, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 2] = src[RIDX(i + 2, j, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 3] = src[RIDX(i + 3, j, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 4] = src[RIDX(i + 4, j, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 5] = src[RIDX(i + 5, j, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 6] = src[RIDX(i + 6, j, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 7] = src[RIDX(i + 7, j, dim)];
	

	    dim_min_one_min_j_mult_dim = (dim - 1 -(j+1)) *dim;	    
	    dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j + 1, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 1] = src[RIDX(i + 1, j + 1, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 2] = src[RIDX(i + 2, j + 1, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 3] = src[RIDX(i + 3, j + 1, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 4] = src[RIDX(i + 4, j + 1, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 5] = src[RIDX(i + 5, j + 1, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 6] = src[RIDX(i + 6, j + 1, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 7] = src[RIDX(i + 7, j + 1, dim)];
	    
	    dim_min_one_min_j_mult_dim = (dim - 1 -(j+2)) *dim;	    
	    dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j + 2, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 1] = src[RIDX(i + 1, j + 2, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 2] = src[RIDX(i + 2, j + 2, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 3] = src[RIDX(i + 3, j + 2, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 4] = src[RIDX(i + 4, j + 2, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 5] = src[RIDX(i + 5, j + 2, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 6] = src[RIDX(i + 6, j + 2, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 7] = src[RIDX(i + 7, j + 2, dim)];

	    dim_min_one_min_j_mult_dim = (dim - 1 -(j+3)) *dim;	    
	    dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j + 3, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 1] = src[RIDX(i + 1, j + 3, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 2] = src[RIDX(i + 2, j + 3, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 3] = src[RIDX(i + 3, j + 3, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 4] = src[RIDX(i + 4, j + 3, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 5] = src[RIDX(i + 5, j + 3, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 6] = src[RIDX(i + 6, j + 3, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 7] = src[RIDX(i + 7, j + 3, dim)];

	    dim_min_one_min_j_mult_dim = (dim - 1 -(j+4)) *dim;	    
	    dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j + 4, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 1] = src[RIDX(i + 1, j + 4, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 2] = src[RIDX(i + 2, j + 4, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 3] = src[RIDX(i + 3, j + 4, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 4] = src[RIDX(i + 4, j + 4, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 5] = src[RIDX(i + 5, j + 4, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 6] = src[RIDX(i + 6, j + 4, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 7] = src[RIDX(i + 7, j + 4, dim)];

	    dim_min_one_min_j_mult_dim = (dim - 1 -(j+5)) *dim;	    
	    dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j + 5, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 1] = src[RIDX(i + 1, j + 5, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 2] = src[RIDX(i + 2, j + 5, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 3] = src[RIDX(i + 3, j + 5, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 4] = src[RIDX(i + 4, j + 5, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 5] = src[RIDX(i + 5, j + 5, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 6] = src[RIDX(i + 6, j + 5, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 7] = src[RIDX(i + 7, j + 5, dim)];

	    dim_min_one_min_j_mult_dim = (dim - 1 -(j+6)) *dim;	    
	    dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j + 6, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 1] = src[RIDX(i + 1, j + 6, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 2] = src[RIDX(i + 2, j + 6, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 3] = src[RIDX(i + 3, j + 6, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 4] = src[RIDX(i + 4, j + 6, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 5] = src[RIDX(i + 5, j + 6, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 6] = src[RIDX(i + 6, j + 6, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 7] = src[RIDX(i + 7, j + 6, dim)];

	    dim_min_one_min_j_mult_dim = (dim - 1 -(j+7)) *dim;	    
	    dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j + 7, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 1] = src[RIDX(i + 1, j + 7, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 2] = src[RIDX(i + 2, j + 7, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 3] = src[RIDX(i + 3, j + 7, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 4] = src[RIDX(i + 4, j + 7, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 5] = src[RIDX(i + 5, j + 7, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 6] = src[RIDX(i + 6, j + 7, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 7] = src[RIDX(i + 7, j + 7, dim)];

	

	}

	//for(;i<dim;i++){
	//    dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j, dim)];
	//}
	
	
        
        

    }
}
/*********************************************************************
 * register_rotate_functions - Register all of your different versions
 *     of the rotate kernel with the driver by calling the
 *     add_rotate_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

char rotate_six_descr[] = "sixth attempt: attempt 4 and tile";
void attempt_six(int dim, pixel *src, pixel *dst) 
{
    int i_block, j_block;
    int dim_min_one_min_j_mult_dim;
    int i, j;
    int blocksize = 16;
    for (j_block = 0; j_block < dim; j_block+=blocksize){
	for (i_block = 0; i_block < dim; i_block+=blocksize){
	    for(j=j_block; j<j_block+blocksize;j++){   
	        dim_min_one_min_j_mult_dim = (dim - 1 - j) * dim;
	        for(i=i_block; i<i_block+blocksize;i++)
	    		dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j, dim)];
	     }
	}

    }
}

char rotate_seven_descr[] = "seventh attempt: attempt 6 with larger blocksize";
void attempt_seven(int dim, pixel *src, pixel *dst) 
{
    int i_block, j_block;
    int dim_min_one_min_j_mult_dim;
    int i, j;
    int blocksize = 16;
    if (dim%128==0)
	blocksize = 128;
    for (j_block = 0; j_block < dim; j_block+=blocksize){
	for (i_block = 0; i_block < dim; i_block+=blocksize){
	    for(j=j_block; j<j_block+blocksize;j++){   
	        dim_min_one_min_j_mult_dim = (dim - 1 - j) * dim;
	        for(i=i_block; i<i_block+blocksize;i++)
	    		dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j, dim)];
	     }
	}

    }
}

char rotate_eight_descr[] = "eighth attempt: attempt 6 and unroll";
void attempt_eight(int dim, pixel *src, pixel *dst) 
{
    int i_block, j_block;
    int dim_min_one_min_j_mult_dim;
    int i, j;
    int blocksize = 16;


    for (j_block = 0; j_block < dim; j_block+=blocksize){
	for (i_block = 0; i_block < dim; i_block+=blocksize){
	    for(j=j_block; j<j_block+blocksize;j+=8){   
	        for(i=i_block; i<i_block+blocksize;i+=8){
	        	dim_min_one_min_j_mult_dim = (dim - 1 - j) * dim;
	    		dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 1] = src[RIDX(i + 1, j, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 2] = src[RIDX(i + 2, j, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 3] = src[RIDX(i + 3, j, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 4] = src[RIDX(i + 4, j, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 5] = src[RIDX(i + 5, j, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 6] = src[RIDX(i + 6, j, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 7] = src[RIDX(i + 7, j, dim)];
	        	
	        	dim_min_one_min_j_mult_dim = (dim - 1 - j - 1) * dim;
	    		dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j + 1, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 1] = src[RIDX(i + 1, j + 1, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 2] = src[RIDX(i + 2, j + 1, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 3] = src[RIDX(i + 3, j + 1, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 4] = src[RIDX(i + 4, j + 1, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 5] = src[RIDX(i + 5, j + 1, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 6] = src[RIDX(i + 6, j + 1, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 7] = src[RIDX(i + 7, j + 1, dim)];
		
	        	dim_min_one_min_j_mult_dim = (dim - 1 - j - 2) * dim;
	    		dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j + 2, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 1] = src[RIDX(i + 1, j + 2, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 2] = src[RIDX(i + 2, j + 2, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 3] = src[RIDX(i + 3, j + 2, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 4] = src[RIDX(i + 4, j + 2, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 5] = src[RIDX(i + 5, j + 2, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 6] = src[RIDX(i + 6, j + 2, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 7] = src[RIDX(i + 7, j + 2, dim)];
		

	        	dim_min_one_min_j_mult_dim = (dim - 1 - j - 3) * dim;
	    		dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j + 3, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 1] = src[RIDX(i + 1, j + 3, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 2] = src[RIDX(i + 2, j + 3, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 3] = src[RIDX(i + 3, j + 3, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 4] = src[RIDX(i + 4, j + 3, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 5] = src[RIDX(i + 5, j + 3, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 6] = src[RIDX(i + 6, j + 3, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 7] = src[RIDX(i + 7, j + 3, dim)];
	        	
			dim_min_one_min_j_mult_dim = (dim - 1 - j - 4) * dim;
	    		dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j + 4, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 1] = src[RIDX(i + 1, j + 4, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 2] = src[RIDX(i + 2, j + 4, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 3] = src[RIDX(i + 3, j + 4, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 4] = src[RIDX(i + 4, j + 4, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 5] = src[RIDX(i + 5, j + 4, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 6] = src[RIDX(i + 6, j + 4, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 7] = src[RIDX(i + 7, j + 4, dim)];
			
			dim_min_one_min_j_mult_dim = (dim - 1 - j - 5) * dim;
	    		dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j + 5, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 1] = src[RIDX(i + 1, j + 5, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 2] = src[RIDX(i + 2, j + 5, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 3] = src[RIDX(i + 3, j + 5, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 4] = src[RIDX(i + 4, j + 5, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 5] = src[RIDX(i + 5, j + 5, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 6] = src[RIDX(i + 6, j + 5, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 7] = src[RIDX(i + 7, j + 5, dim)];
			
			dim_min_one_min_j_mult_dim = (dim - 1 - j - 6) * dim;
	    		dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j + 6, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 1] = src[RIDX(i + 1, j + 6, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 2] = src[RIDX(i + 2, j + 6, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 3] = src[RIDX(i + 3, j + 6, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 4] = src[RIDX(i + 4, j + 6, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 5] = src[RIDX(i + 5, j + 6, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 6] = src[RIDX(i + 6, j + 6, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 7] = src[RIDX(i + 7, j + 6, dim)];
			
			dim_min_one_min_j_mult_dim = (dim - 1 - j - 7) * dim;
	    		dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j + 7, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 1] = src[RIDX(i + 1, j + 7, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 2] = src[RIDX(i + 2, j + 7, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 3] = src[RIDX(i + 3, j + 7, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 4] = src[RIDX(i + 4, j + 7, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 5] = src[RIDX(i + 5, j + 7, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 6] = src[RIDX(i + 6, j + 7, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 7] = src[RIDX(i + 7, j + 7, dim)];
		}
	     }
	}

    }




}


char rotate_nine_descr[] = "ninth attempt: attempt 8 with bigger blocksize";
void attempt_nine(int dim, pixel *src, pixel *dst) 
{
    int i_block, j_block;
    int dim_min_one_min_j_mult_dim;
    int i, j;
    int blocksize = 16;
    if(dim%128==0)
	blocksize=128;


    for (j_block = 0; j_block < dim; j_block+=blocksize){
	for (i_block = 0; i_block < dim; i_block+=blocksize){
	    for(j=j_block; j<j_block+blocksize;j+=8){   
	        for(i=i_block; i<i_block+blocksize;i+=8){
	        	dim_min_one_min_j_mult_dim = (dim - 1 - j) * dim;
	    		dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 1] = src[RIDX(i + 1, j, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 2] = src[RIDX(i + 2, j, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 3] = src[RIDX(i + 3, j, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 4] = src[RIDX(i + 4, j, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 5] = src[RIDX(i + 5, j, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 6] = src[RIDX(i + 6, j, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 7] = src[RIDX(i + 7, j, dim)];
	        	
	        	dim_min_one_min_j_mult_dim = (dim - 1 - j - 1) * dim;
	    		dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j + 1, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 1] = src[RIDX(i + 1, j + 1, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 2] = src[RIDX(i + 2, j + 1, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 3] = src[RIDX(i + 3, j + 1, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 4] = src[RIDX(i + 4, j + 1, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 5] = src[RIDX(i + 5, j + 1, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 6] = src[RIDX(i + 6, j + 1, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 7] = src[RIDX(i + 7, j + 1, dim)];
		
	        	dim_min_one_min_j_mult_dim = (dim - 1 - j - 2) * dim;
	    		dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j + 2, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 1] = src[RIDX(i + 1, j + 2, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 2] = src[RIDX(i + 2, j + 2, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 3] = src[RIDX(i + 3, j + 2, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 4] = src[RIDX(i + 4, j + 2, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 5] = src[RIDX(i + 5, j + 2, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 6] = src[RIDX(i + 6, j + 2, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 7] = src[RIDX(i + 7, j + 2, dim)];
		

	        	dim_min_one_min_j_mult_dim = (dim - 1 - j - 3) * dim;
	    		dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j + 3, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 1] = src[RIDX(i + 1, j + 3, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 2] = src[RIDX(i + 2, j + 3, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 3] = src[RIDX(i + 3, j + 3, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 4] = src[RIDX(i + 4, j + 3, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 5] = src[RIDX(i + 5, j + 3, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 6] = src[RIDX(i + 6, j + 3, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 7] = src[RIDX(i + 7, j + 3, dim)];
	        	
			dim_min_one_min_j_mult_dim = (dim - 1 - j - 4) * dim;
	    		dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j + 4, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 1] = src[RIDX(i + 1, j + 4, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 2] = src[RIDX(i + 2, j + 4, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 3] = src[RIDX(i + 3, j + 4, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 4] = src[RIDX(i + 4, j + 4, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 5] = src[RIDX(i + 5, j + 4, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 6] = src[RIDX(i + 6, j + 4, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 7] = src[RIDX(i + 7, j + 4, dim)];
			
			dim_min_one_min_j_mult_dim = (dim - 1 - j - 5) * dim;
	    		dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j + 5, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 1] = src[RIDX(i + 1, j + 5, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 2] = src[RIDX(i + 2, j + 5, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 3] = src[RIDX(i + 3, j + 5, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 4] = src[RIDX(i + 4, j + 5, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 5] = src[RIDX(i + 5, j + 5, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 6] = src[RIDX(i + 6, j + 5, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 7] = src[RIDX(i + 7, j + 5, dim)];
			
			dim_min_one_min_j_mult_dim = (dim - 1 - j - 6) * dim;
	    		dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j + 6, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 1] = src[RIDX(i + 1, j + 6, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 2] = src[RIDX(i + 2, j + 6, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 3] = src[RIDX(i + 3, j + 6, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 4] = src[RIDX(i + 4, j + 6, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 5] = src[RIDX(i + 5, j + 6, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 6] = src[RIDX(i + 6, j + 6, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 7] = src[RIDX(i + 7, j + 6, dim)];
			
			dim_min_one_min_j_mult_dim = (dim - 1 - j - 7) * dim;
	    		dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j + 7, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 1] = src[RIDX(i + 1, j + 7, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 2] = src[RIDX(i + 2, j + 7, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 3] = src[RIDX(i + 3, j + 7, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 4] = src[RIDX(i + 4, j + 7, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 5] = src[RIDX(i + 5, j + 7, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 6] = src[RIDX(i + 6, j + 7, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 7] = src[RIDX(i + 7, j + 7, dim)];
		}
	     }
	}

    }




}
char rotate_ten_descr[] = "tenth attempt:  only do unroll and tile on larger dimensions (1024 and bigger)(attempt 9), else only do tiling (attempt 6)";
void attempt_ten(int dim, pixel *src, pixel *dst) 
{
    int i_block, j_block;
    int dim_min_one_min_j_mult_dim;
    int i, j;
    int blocksize = 16;
//    if (dim%64==0)
//	blocksize=64;

    if (dim>=1024 && dim%128==0)
    //if (dim%128==0)
	blocksize = 128;

    if(dim>=1024){
    for (j_block = 0; j_block < dim; j_block+=blocksize){
	for (i_block = 0; i_block < dim; i_block+=blocksize){
	    for(j=j_block; j<j_block+blocksize;j+=8){   
	        for(i=i_block; i<i_block+blocksize;i+=8){
	        	dim_min_one_min_j_mult_dim = (dim - 1 - j) * dim;
	    		dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 1] = src[RIDX(i + 1, j, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 2] = src[RIDX(i + 2, j, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 3] = src[RIDX(i + 3, j, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 4] = src[RIDX(i + 4, j, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 5] = src[RIDX(i + 5, j, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 6] = src[RIDX(i + 6, j, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 7] = src[RIDX(i + 7, j, dim)];
	        	
	        	dim_min_one_min_j_mult_dim = (dim - 1 - j - 1) * dim;
	    		dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j + 1, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 1] = src[RIDX(i + 1, j + 1, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 2] = src[RIDX(i + 2, j + 1, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 3] = src[RIDX(i + 3, j + 1, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 4] = src[RIDX(i + 4, j + 1, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 5] = src[RIDX(i + 5, j + 1, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 6] = src[RIDX(i + 6, j + 1, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 7] = src[RIDX(i + 7, j + 1, dim)];
		
	        	dim_min_one_min_j_mult_dim = (dim - 1 - j - 2) * dim;
	    		dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j + 2, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 1] = src[RIDX(i + 1, j + 2, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 2] = src[RIDX(i + 2, j + 2, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 3] = src[RIDX(i + 3, j + 2, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 4] = src[RIDX(i + 4, j + 2, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 5] = src[RIDX(i + 5, j + 2, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 6] = src[RIDX(i + 6, j + 2, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 7] = src[RIDX(i + 7, j + 2, dim)];
		

	        	dim_min_one_min_j_mult_dim = (dim - 1 - j - 3) * dim;
	    		dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j + 3, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 1] = src[RIDX(i + 1, j + 3, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 2] = src[RIDX(i + 2, j + 3, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 3] = src[RIDX(i + 3, j + 3, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 4] = src[RIDX(i + 4, j + 3, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 5] = src[RIDX(i + 5, j + 3, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 6] = src[RIDX(i + 6, j + 3, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 7] = src[RIDX(i + 7, j + 3, dim)];
	        	
			dim_min_one_min_j_mult_dim = (dim - 1 - j - 4) * dim;
	    		dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j + 4, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 1] = src[RIDX(i + 1, j + 4, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 2] = src[RIDX(i + 2, j + 4, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 3] = src[RIDX(i + 3, j + 4, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 4] = src[RIDX(i + 4, j + 4, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 5] = src[RIDX(i + 5, j + 4, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 6] = src[RIDX(i + 6, j + 4, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 7] = src[RIDX(i + 7, j + 4, dim)];
			
			dim_min_one_min_j_mult_dim = (dim - 1 - j - 5) * dim;
	    		dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j + 5, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 1] = src[RIDX(i + 1, j + 5, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 2] = src[RIDX(i + 2, j + 5, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 3] = src[RIDX(i + 3, j + 5, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 4] = src[RIDX(i + 4, j + 5, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 5] = src[RIDX(i + 5, j + 5, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 6] = src[RIDX(i + 6, j + 5, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 7] = src[RIDX(i + 7, j + 5, dim)];
			
			dim_min_one_min_j_mult_dim = (dim - 1 - j - 6) * dim;
	    		dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j + 6, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 1] = src[RIDX(i + 1, j + 6, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 2] = src[RIDX(i + 2, j + 6, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 3] = src[RIDX(i + 3, j + 6, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 4] = src[RIDX(i + 4, j + 6, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 5] = src[RIDX(i + 5, j + 6, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 6] = src[RIDX(i + 6, j + 6, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 7] = src[RIDX(i + 7, j + 6, dim)];
			
			dim_min_one_min_j_mult_dim = (dim - 1 - j - 7) * dim;
	    		dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j + 7, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 1] = src[RIDX(i + 1, j + 7, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 2] = src[RIDX(i + 2, j + 7, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 3] = src[RIDX(i + 3, j + 7, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 4] = src[RIDX(i + 4, j + 7, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 5] = src[RIDX(i + 5, j + 7, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 6] = src[RIDX(i + 6, j + 7, dim)];
	    		dst[dim_min_one_min_j_mult_dim + i + 7] = src[RIDX(i + 7, j + 7, dim)];
		}
	     }
	}

    }
    }
    else{
    for (j_block = 0; j_block < dim; j_block+=blocksize){
	for (i_block = 0; i_block < dim; i_block+=blocksize){
	    for(j=j_block; j<j_block+blocksize;j++){   
	        dim_min_one_min_j_mult_dim = (dim - 1 - j) * dim;
	        for(i=i_block; i<i_block+blocksize;i++)
	    		dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j, dim)];
	     }
	}

    }



    }
}


void register_rotate_functions() 
{
    add_rotate_function(&naive_rotate, naive_rotate_descr);   
    add_rotate_function(&rotate, rotate_descr);   

   add_rotate_function(&attempt_two, rotate_two_descr);   
   add_rotate_function(&attempt_three, rotate_three_descr);  
   add_rotate_function(&attempt_four, rotate_four_descr);   
   add_rotate_function(&attempt_five, rotate_five_descr);   
   add_rotate_function(&attempt_six, rotate_six_descr);   
   add_rotate_function(&attempt_seven, rotate_seven_descr);   
   add_rotate_function(&attempt_eight, rotate_eight_descr);   
   add_rotate_function(&attempt_nine, rotate_nine_descr);   
   add_rotate_function(&attempt_ten, rotate_ten_descr);   
    //add_rotate_function(&attempt_eleven, rotate_eleven_descr);   

    /* ... Register additional rotate functions here */
}

