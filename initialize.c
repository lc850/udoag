
/* Data initialization routines */

# include <stdio.h>
# include <stdlib.h>
# include <math.h>

# include "global.h"
# include "rand.h"



/* Function to initialize a population randomly */
void initialize_pop (population *pop)
{
    int i;

    //w
    //pop->ind[0].xreal[0]=0.4; pop->ind[0].xreal[1]=0.25; pop->ind[0].xreal[2]=0.15; pop->ind[0].xreal[3]=0.1; pop->ind[0].xreal[4]=0.1;
    /*
    for (i=0; i<popsize; i++)
    {
    	pop->ind[i].xreal[0]=0.4; pop->ind[i].xreal[1]=0.25; pop->ind[i].xreal[2]=0.15; pop->ind[i].xreal[3]=0.1; pop->ind[i].xreal[4]=0.1;
    	initialize_ind_notWeights (&(pop->ind[i]));
    }

    */

    for (i=0; i<popsize; i++)
    {
        initialize_ind (&(pop->ind[i]));
    }

    return;
}

/* Function to initialize an individual randomly */
void initialize_ind (individual *ind)
{

	int j;
	double low=90, high=100;
	double aleatorio;

	for(j=0; j<nreal; j++){
		aleatorio = rndreal ( low,  high);
		high = aleatorio - 0.1 ;
		low = low - 2;
		if (low<=10) low=low-1;
		if (low<=5) low=low-0.1;
		if (low<=1) low=low-0.01;
		if (low<=0.01) low=low-0.001;

		ind->xreal[j] = aleatorio;
	}

	/*
	for(j=0; j<nreal; j++){
		printf("%f,", ind->xreal[j]);
	}
	printf("%f,", ind->xreal[j]);
*/



    return;
}



