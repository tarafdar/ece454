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
	attempt_seven(dim,src,dst);

}


char rotate_two_descr[] = "second attempt";
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

char rotate_three_descr[] = "third attempt";
void attempt_three(int dim, pixel *src, pixel *dst) 
{
    int i, j, k;
    int i_mult_dim;
    int dim_squared_minus_dim =dim*dim -dim;
    int dim_squared_minus_dim_plus_i;
    int dim_min_one = dim -1;
    //for(k=0; k<10; k++){
    pixel * src_temp;
    for (i = 0; i < dim; i++){
	i_mult_dim = i*dim;
        dim_squared_minus_dim_plus_i= dim_squared_minus_dim + i;
	src_temp = src+i_mult_dim;
	for (j = 0; j < dim; j++){
//	    dst[RIDX(i, dim_min_one-j, dim)] = src_temp[j];

//	    dst[RIDX(i, dim-1-j, dim)] = src_temp[j];
	    dst[dim_squared_minus_dim_plus_i -j*dim] = src_temp[j];
	}
    }
    //}

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

char rotate_five_descr[] = "five attempt: i and j switch plus unroll";
void attempt_five(int dim, pixel *src, pixel *dst) 
{
    int i, j;
    int dim_min_one_min_j_mult_dim;



    for (j = 0; j < dim; j+=16){
	for (i = 0; i < dim; i+=16){
	    dim_min_one_min_j_mult_dim = (dim - 1 -j) *dim;
	    dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 1] = src[RIDX(i + 1, j, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 2] = src[RIDX(i + 2, j, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 3] = src[RIDX(i + 3, j, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 4] = src[RIDX(i + 4, j, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 5] = src[RIDX(i + 5, j, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 6] = src[RIDX(i + 6, j, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 7] = src[RIDX(i + 7, j, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 8] = src[RIDX(i + 8, j, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 9] = src[RIDX(i + 9, j, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 10] = src[RIDX(i + 10, j, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 11] = src[RIDX(i + 11, j, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 12] = src[RIDX(i + 12, j, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 13] = src[RIDX(i + 13, j, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 14] = src[RIDX(i + 14, j, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 15] = src[RIDX(i + 15, j, dim)];
	

	    dim_min_one_min_j_mult_dim = (dim - 1 -(j+1)) *dim;	    
	    dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j + 1, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 1] = src[RIDX(i + 1, j + 1, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 2] = src[RIDX(i + 2, j + 1, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 3] = src[RIDX(i + 3, j + 1, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 4] = src[RIDX(i + 4, j + 1, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 5] = src[RIDX(i + 5, j + 1, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 6] = src[RIDX(i + 6, j + 1, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 7] = src[RIDX(i + 7, j + 1, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 8] = src[RIDX(i + 8, j + 1, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 9] = src[RIDX(i + 9, j + 1, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 10] = src[RIDX(i + 10, j + 1, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 11] = src[RIDX(i + 11, j + 1, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 12] = src[RIDX(i + 12, j + 1, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 13] = src[RIDX(i + 13, j + 1, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 14] = src[RIDX(i + 14, j + 1, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 15] = src[RIDX(i + 15, j + 1, dim)];
	    
	    dim_min_one_min_j_mult_dim = (dim - 1 -(j+2)) *dim;	    
	    dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j + 2, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 1] = src[RIDX(i + 1, j + 2, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 2] = src[RIDX(i + 2, j + 2, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 3] = src[RIDX(i + 3, j + 2, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 4] = src[RIDX(i + 4, j + 2, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 5] = src[RIDX(i + 5, j + 2, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 6] = src[RIDX(i + 6, j + 2, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 7] = src[RIDX(i + 7, j + 2, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 8] = src[RIDX(i + 8, j + 2, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 9] = src[RIDX(i + 9, j + 2, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 10] = src[RIDX(i + 10, j + 2, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 11] = src[RIDX(i + 11, j + 2, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 12] = src[RIDX(i + 12, j + 2, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 13] = src[RIDX(i + 13, j + 2, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 14] = src[RIDX(i + 14, j + 2, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 15] = src[RIDX(i + 15, j + 2, dim)];

	    dim_min_one_min_j_mult_dim = (dim - 1 -(j+3)) *dim;	    
	    dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j + 3, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 1] = src[RIDX(i + 1, j + 3, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 2] = src[RIDX(i + 2, j + 3, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 3] = src[RIDX(i + 3, j + 3, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 4] = src[RIDX(i + 4, j + 3, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 5] = src[RIDX(i + 5, j + 3, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 6] = src[RIDX(i + 6, j + 3, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 7] = src[RIDX(i + 7, j + 3, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 8] = src[RIDX(i + 8, j + 3, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 9] = src[RIDX(i + 9, j + 3, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 10] = src[RIDX(i + 10, j + 3, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 11] = src[RIDX(i + 11, j + 3, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 12] = src[RIDX(i + 12, j + 3, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 13] = src[RIDX(i + 13, j + 3, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 14] = src[RIDX(i + 14, j + 3, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 15] = src[RIDX(i + 15, j + 3, dim)];

	    dim_min_one_min_j_mult_dim = (dim - 1 -(j+4)) *dim;	    
	    dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j + 4, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 1] = src[RIDX(i + 1, j + 4, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 2] = src[RIDX(i + 2, j + 4, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 3] = src[RIDX(i + 3, j + 4, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 4] = src[RIDX(i + 4, j + 4, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 5] = src[RIDX(i + 5, j + 4, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 6] = src[RIDX(i + 6, j + 4, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 7] = src[RIDX(i + 7, j + 4, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 8] = src[RIDX(i + 8, j + 4, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 9] = src[RIDX(i + 9, j + 4, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 10] = src[RIDX(i + 10, j + 4, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 11] = src[RIDX(i + 11, j + 4, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 12] = src[RIDX(i + 12, j + 4, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 13] = src[RIDX(i + 13, j + 4, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 14] = src[RIDX(i + 14, j + 4, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 15] = src[RIDX(i + 15, j + 4, dim)];

	    dim_min_one_min_j_mult_dim = (dim - 1 -(j+5)) *dim;	    
	    dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j + 5, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 1] = src[RIDX(i + 1, j + 5, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 2] = src[RIDX(i + 2, j + 5, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 3] = src[RIDX(i + 3, j + 5, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 4] = src[RIDX(i + 4, j + 5, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 5] = src[RIDX(i + 5, j + 5, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 6] = src[RIDX(i + 6, j + 5, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 7] = src[RIDX(i + 7, j + 5, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 8] = src[RIDX(i + 8, j + 5, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 9] = src[RIDX(i + 9, j + 5, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 10] = src[RIDX(i + 10, j + 5, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 11] = src[RIDX(i + 11, j + 5, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 12] = src[RIDX(i + 12, j + 5, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 13] = src[RIDX(i + 13, j + 5, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 14] = src[RIDX(i + 14, j + 5, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 15] = src[RIDX(i + 15, j + 5, dim)];

	    dim_min_one_min_j_mult_dim = (dim - 1 -(j+6)) *dim;	    
	    dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j + 6, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 1] = src[RIDX(i + 1, j + 6, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 2] = src[RIDX(i + 2, j + 6, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 3] = src[RIDX(i + 3, j + 6, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 4] = src[RIDX(i + 4, j + 6, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 5] = src[RIDX(i + 5, j + 6, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 6] = src[RIDX(i + 6, j + 6, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 7] = src[RIDX(i + 7, j + 6, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 8] = src[RIDX(i + 8, j + 6, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 9] = src[RIDX(i + 9, j + 6, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 10] = src[RIDX(i + 10, j + 6, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 11] = src[RIDX(i + 11, j + 6, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 12] = src[RIDX(i + 12, j + 6, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 13] = src[RIDX(i + 13, j + 6, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 14] = src[RIDX(i + 14, j + 6, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 15] = src[RIDX(i + 15, j + 6, dim)];

	    dim_min_one_min_j_mult_dim = (dim - 1 -(j+7)) *dim;	    
	    dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j + 7, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 1] = src[RIDX(i + 1, j + 7, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 2] = src[RIDX(i + 2, j + 7, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 3] = src[RIDX(i + 3, j + 7, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 4] = src[RIDX(i + 4, j + 7, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 5] = src[RIDX(i + 5, j + 7, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 6] = src[RIDX(i + 6, j + 7, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 7] = src[RIDX(i + 7, j + 7, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 8] = src[RIDX(i + 8, j + 7, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 9] = src[RIDX(i + 9, j + 7, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 10] = src[RIDX(i + 10, j + 7, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 11] = src[RIDX(i + 11, j + 7, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 12] = src[RIDX(i + 12, j + 7, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 13] = src[RIDX(i + 13, j + 7, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 14] = src[RIDX(i + 14, j + 7, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 15] = src[RIDX(i + 15, j + 7, dim)];

	    dim_min_one_min_j_mult_dim = (dim - 1 -(j+8)) *dim;
	    dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j + 8, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 1] = src[RIDX(i + 1, j + 8, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 2] = src[RIDX(i + 2, j + 8, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 3] = src[RIDX(i + 3, j + 8, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 4] = src[RIDX(i + 4, j + 8, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 5] = src[RIDX(i + 5, j + 8, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 6] = src[RIDX(i + 6, j + 8, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 7] = src[RIDX(i + 7, j + 8, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 8] = src[RIDX(i + 8, j + 8, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 9] = src[RIDX(i + 9, j + 8, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 10] = src[RIDX(i + 10, j + 8, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 11] = src[RIDX(i + 11, j + 8, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 12] = src[RIDX(i + 12, j + 8, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 13] = src[RIDX(i + 13, j + 8, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 14] = src[RIDX(i + 14, j + 8, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 15] = src[RIDX(i + 15, j + 8, dim)];
	

	    dim_min_one_min_j_mult_dim = (dim - 1 -(j+9)) *dim;	    
	    dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j + 9, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 1] = src[RIDX(i + 1, j + 9, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 2] = src[RIDX(i + 2, j + 9, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 3] = src[RIDX(i + 3, j + 9, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 4] = src[RIDX(i + 4, j + 9, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 5] = src[RIDX(i + 5, j + 9, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 6] = src[RIDX(i + 6, j + 9, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 7] = src[RIDX(i + 7, j + 9, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 8] = src[RIDX(i + 8, j + 9, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 9] = src[RIDX(i + 9, j + 9, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 10] = src[RIDX(i + 10, j + 9, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 11] = src[RIDX(i + 11, j + 9, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 12] = src[RIDX(i + 12, j + 9, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 13] = src[RIDX(i + 13, j + 9, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 14] = src[RIDX(i + 14, j + 9, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 15] = src[RIDX(i + 15, j + 9, dim)];
	    
	    dim_min_one_min_j_mult_dim = (dim - 1 -(j+10)) *dim;	    
	    dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j + 10, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 1] = src[RIDX(i + 1, j + 10, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 2] = src[RIDX(i + 2, j + 10, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 3] = src[RIDX(i + 3, j + 10, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 4] = src[RIDX(i + 4, j + 10, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 5] = src[RIDX(i + 5, j + 10, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 6] = src[RIDX(i + 6, j + 10, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 7] = src[RIDX(i + 7, j + 10, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 8] = src[RIDX(i + 8, j + 10, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 9] = src[RIDX(i + 9, j + 10, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 10] = src[RIDX(i + 10, j + 10, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 11] = src[RIDX(i + 11, j + 10, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 12] = src[RIDX(i + 12, j + 10, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 13] = src[RIDX(i + 13, j + 10, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 14] = src[RIDX(i + 14, j + 10, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 15] = src[RIDX(i + 15, j + 10, dim)];

	    dim_min_one_min_j_mult_dim = (dim - 1 -(j+11)) *dim;	    
	    dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j + 11, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 1] = src[RIDX(i + 1, j + 11, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 2] = src[RIDX(i + 2, j + 11, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 3] = src[RIDX(i + 3, j + 11, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 4] = src[RIDX(i + 4, j + 11, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 5] = src[RIDX(i + 5, j + 11, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 6] = src[RIDX(i + 6, j + 11, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 7] = src[RIDX(i + 7, j + 11, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 8] = src[RIDX(i + 8, j + 11, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 9] = src[RIDX(i + 9, j + 11, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 10] = src[RIDX(i + 10, j + 11, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 11] = src[RIDX(i + 11, j + 11, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 12] = src[RIDX(i + 12, j + 11, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 13] = src[RIDX(i + 13, j + 11, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 14] = src[RIDX(i + 14, j + 11, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 15] = src[RIDX(i + 15, j + 11, dim)];

	    dim_min_one_min_j_mult_dim = (dim - 1 -(j+12)) *dim;	    
	    dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j + 12, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 1] = src[RIDX(i + 1, j + 12, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 2] = src[RIDX(i + 2, j + 12, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 3] = src[RIDX(i + 3, j + 12, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 4] = src[RIDX(i + 4, j + 12, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 5] = src[RIDX(i + 5, j + 12, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 6] = src[RIDX(i + 6, j + 12, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 7] = src[RIDX(i + 7, j + 12, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 8] = src[RIDX(i + 8, j + 12, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 9] = src[RIDX(i + 9, j + 12, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 10] = src[RIDX(i + 10, j + 12, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 11] = src[RIDX(i + 11, j + 12, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 12] = src[RIDX(i + 12, j + 12, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 13] = src[RIDX(i + 13, j + 12, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 14] = src[RIDX(i + 14, j + 12, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 15] = src[RIDX(i + 15, j + 12, dim)];

	    dim_min_one_min_j_mult_dim = (dim - 1 -(j+13)) *dim;	    
	    dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j + 13, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 1] = src[RIDX(i + 1, j + 13, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 2] = src[RIDX(i + 2, j + 13, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 3] = src[RIDX(i + 3, j + 13, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 4] = src[RIDX(i + 4, j + 13, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 5] = src[RIDX(i + 5, j + 13, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 6] = src[RIDX(i + 6, j + 13, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 7] = src[RIDX(i + 7, j + 13, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 8] = src[RIDX(i + 8, j + 13, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 9] = src[RIDX(i + 9, j + 13, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 10] = src[RIDX(i + 10, j + 13, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 11] = src[RIDX(i + 11, j + 13, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 12] = src[RIDX(i + 12, j + 13, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 13] = src[RIDX(i + 13, j + 13, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 14] = src[RIDX(i + 14, j + 13, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 15] = src[RIDX(i + 15, j + 13, dim)];

	    dim_min_one_min_j_mult_dim = (dim - 1 -(j+14)) *dim;	    
	    dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j + 14, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 1] = src[RIDX(i + 1, j + 14, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 2] = src[RIDX(i + 2, j + 14, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 3] = src[RIDX(i + 3, j + 14, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 4] = src[RIDX(i + 4, j + 14, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 5] = src[RIDX(i + 5, j + 14, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 6] = src[RIDX(i + 6, j + 14, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 7] = src[RIDX(i + 7, j + 14, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 8] = src[RIDX(i + 8, j + 14, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 9] = src[RIDX(i + 9, j + 14, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 10] = src[RIDX(i + 10, j + 14, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 11] = src[RIDX(i + 11, j + 14, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 12] = src[RIDX(i + 12, j + 14, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 13] = src[RIDX(i + 13, j + 14, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 14] = src[RIDX(i + 14, j + 14, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 15] = src[RIDX(i + 15, j + 14, dim)];

	    dim_min_one_min_j_mult_dim = (dim - 1 -(j+15)) *dim;	    
	    dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j + 15, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 1] = src[RIDX(i + 1, j + 15, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 2] = src[RIDX(i + 2, j + 15, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 3] = src[RIDX(i + 3, j + 15, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 4] = src[RIDX(i + 4, j + 15, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 5] = src[RIDX(i + 5, j + 15, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 6] = src[RIDX(i + 6, j + 15, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 7] = src[RIDX(i + 7, j + 15, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 8] = src[RIDX(i + 8, j + 15, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 9] = src[RIDX(i + 9, j + 15, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 10] = src[RIDX(i + 10, j + 15, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 11] = src[RIDX(i + 11, j + 15, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 12] = src[RIDX(i + 12, j + 15, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 13] = src[RIDX(i + 13, j + 15, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 14] = src[RIDX(i + 14, j + 15, dim)];
	    dst[dim_min_one_min_j_mult_dim + i + 15] = src[RIDX(i + 15, j + 15, dim)];
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

char rotate_six_descr[] = "sixth attempt: switch i and j code motion and tile";
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

char rotate_seven_descr[] = "sixth attempt: switch i and j code motion and tile and unroll";
void attempt_seven(int dim, pixel *src, pixel *dst) 
{
    int i_block, j_block;
    int dim_min_one_min_j_mult_dim;
    int i, j;
    int blocksize = 16;

//    if (dim==4096)
//	blocksize = 64;


    for (j_block = 0; j_block < dim; j_block+=blocksize){
	for (i_block = 0; i_block < dim; i_block+=blocksize){
	    for(j=j_block; j<j_block+blocksize;j++){   
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
	        	
		//	dim_min_one_min_j_mult_dim = (dim - 1 - (j+1)) * dim;
	    	//	dst[dim_min_one_min_j_mult_dim + i] = src[RIDX(i, j + 1, dim)];
	    	//	dst[dim_min_one_min_j_mult_dim + i + 1] = src[RIDX(i + 1, j + 1, dim)];
	    	//	dst[dim_min_one_min_j_mult_dim + i + 2] = src[RIDX(i + 2, j + 1, dim)];
	    	//	dst[dim_min_one_min_j_mult_dim + i + 3] = src[RIDX(i + 3, j + 1, dim)];
	    	//	dst[dim_min_one_min_j_mult_dim + i + 4] = src[RIDX(i + 4, j + 1, dim)];
	    	//	dst[dim_min_one_min_j_mult_dim + i + 5] = src[RIDX(i + 5, j + 1, dim)];
	    	//	dst[dim_min_one_min_j_mult_dim + i + 6] = src[RIDX(i + 6, j + 1, dim)];
		}
	     }
	}

    }
}

void register_rotate_functions() 
{
    add_rotate_function(&naive_rotate, naive_rotate_descr);   
//    add_rotate_function(&rotate, rotate_descr);   

//    add_rotate_function(&attempt_two, rotate_two_descr);   
    //add_rotate_function(&attempt_three, rotate_three_descr);   
//    add_rotate_function(&attempt_four, rotate_four_descr);   
//    add_rotate_function(&attempt_five, rotate_five_descr);   
    //add_rotate_function(&attempt_six, rotate_six_descr);   
    //add_rotate_function(&attempt_seven, rotate_seven_descr);   
    //add_rotate_function(&attempt_eight, rotate_eight_descr);   
    //add_rotate_function(&attempt_nine, rotate_nine_descr);   
    //add_rotate_function(&attempt_ten, rotate_ten_descr);   
    //add_rotate_function(&attempt_eleven, rotate_eleven_descr);   

    /* ... Register additional rotate functions here */
}

