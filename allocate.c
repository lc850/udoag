/* Memory allocation and deallocation routines */

# include <stdio.h>
# include <stdlib.h>
# include <math.h>

# include "global.h"
# include "rand.h"

/* Function to allocate memory to a population */
void allocate_memory_pop (population *pop, int size)
{
    int i;
    pop->ind = (individual *)malloc(size*sizeof(individual));
    for (i=0; i<size; i++)
    {
        allocate_memory_ind (&(pop->ind[i]));
    }
    return;
}

/* Function to allocate memory to an individual */
void allocate_memory_ind (individual *ind)
{
    int j;
    if (nreal != 0)
    {
        ind->xreal = (double *)malloc(nreal*sizeof(double));
    }
    if (nbin != 0)
    {
        ind->xbin = (double *)malloc(nbin*sizeof(double));
        ind->gene = (int **)malloc(nbin*sizeof(int *));
        for (j=0; j<nbin; j++)
        {
            ind->gene[j] = (int *)malloc(nbits[j]*sizeof(int));
        }
    }
    ind->obj = (double *)malloc(nobj*sizeof(double));
    if (ncon != 0)
    {
        ind->constr = (double *)malloc(ncon*sizeof(double));
    }
    return;
}

/* Function to deallocate memory to a population */
void deallocate_memory_pop (population *pop, int size)
{
    int i;
    for (i=0; i<size; i++)
    {
        deallocate_memory_ind (&(pop->ind[i]));
    }
    free (pop->ind);
    return;
}

/* Function to deallocate memory to an individual */
void deallocate_memory_ind (individual *ind)
{
    int j;
    if (nreal != 0)
    {
        free(ind->xreal);
    }
    if (nbin != 0)
    {
        for (j=0; j<nbin; j++)
        {
            free(ind->gene[j]);
        }
        free(ind->xbin);
        free(ind->gene);
    }
    free(ind->obj);
    if (ncon != 0)
    {
        free(ind->constr);
    }
    return;
}

/* ########################################################### */
/* Adaptation for the intercriteria parameter problem */
/* ########################################################### */

/* Function to allocate memory to the performance matrix */
/* ------------------------------------------------------- */
void allocate_memory_matrix (double ***matrix, int row, int col)
{
    //int j;

    if (row != 0 && col != 0)
    {
    	/*
    	**matrix = (double **) malloc(   row * sizeof(double*)  );
        for (j=0; j<row; j++)
        {
        	*matrix[j] = (double *) malloc(   col * sizeof(double)   );
        }
        */
    }
    else {
        printf("\n No fue posible crear memoria para 'performance_matrix' \n");
        exit(1);
    }

    return;
}

/* Function to allocate memory to intercriteria parameters */
double* allocate_memory_intercriteria (int size)
{
    if (size > 0 )
    {
    	double *intercriteria = (double *)   malloc (   size * sizeof(double)   );
    	return intercriteria;
    }
    else {
        printf("\n No fue posible crear memoria para los parametros intercriterios \n");
        exit(1);
    }

    return NULL;
}

double** allocate_memory_matrix2 (int row, int col)
{
    int j;

    if (row != 0 && col != 0)
    {
    	double **matrix = (double **) malloc(   row * sizeof(double*)  );
        for (j=0; j<row; j++)
        {
        	matrix[j] = (double *) malloc(   col * sizeof(double)   );
        }
        return matrix;
    }
    else {
        printf("\n No fue posible crear memoria para 'performance_matrix' \n");
        exit(1);
    }

    return NULL;
}

int** int_allocate_memory_matrix (int row, int col)
{
    int j;

    if (row != 0 && col != 0)
    {
    	int **matrix = (int **) malloc(   row * sizeof(int*)  );
        for (j=0; j<row; j++)
        {
        	matrix[j] = (int *) malloc(   col * sizeof(int)   );
        }
        return matrix;
    }
    else {
        printf("\n No fue posible crear memoria con 'int_allocate_memory_matrix' \n");
        exit(1);
    }

    return NULL;
}

void deallocate_memory_matrix (double **matrix, int row, int col)
{
    int j;
    if (row != 0 && col != 0)
    {
    	/*
    	 * int j;
        for(i=0; i<row; i++)
        	for(j=0; j<col; j++) {
        		printf("%e\t", matrix[i][j]);
        	}
		*/
        for (j=0; j<row; j++)
        {
        	//printf("\nj=%d",j);
        	//marca error al liberar memoria, comparar con nsga2-maestria para ver el uso de memoria dinámica
            free((void*)matrix[j]);
        }

        free((void*)matrix);
    }

    return;
}



/* Function to deallocate memory to intercriteria parameters */
void deallocate_memory_intercriteria (double *intercriteria)
{
    free (intercriteria);
    return;
}

void int_deallocate_memory_matrix (int **matrix, int row, int col)
{
    int j;
    if (row != 0 && col != 0)
    {
        for (j=0; j<row; j++)
        {
            free((void*)matrix[j]);
        }

        free((void*)matrix);
    }

    return;
}
