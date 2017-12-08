/* Crossover routines */

# include <stdio.h>
# include <stdlib.h>
# include <math.h>

# include "global.h"
# include "rand.h"


/* Function to cross two individuals */
void crossover (individual *parent1, individual *parent2, individual *child1, individual *child2)
{
    if (nreal!=0)
    {
        //realcross (parent1, parent2, child1, child2);
    	realcrossParamMS (parent1, parent2, child1, child2);
    }
    if (nbin!=0)
    {
        bincross (parent1, parent2, child1, child2);
    }
    return;
}

/* Routine for real variable SBX crossover */
void realcrossParamMS (individual *parent1, individual *parent2, individual *child1, individual *child2)
{
	int puntoCruza, i;
	//cruza de genes∫
	puntoCruza = rnd(1, nreal-1);

		for( i=0;  i < nreal;  i++){
			if( i < puntoCruza ){
				child1->xreal[i]  =  parent1->xreal[i];
				child2->xreal[i]  =  parent2->xreal[i];
			}
			else{
				child1->xreal[i] = parent2->xreal[i];
				child2->xreal[i]  =  parent1->xreal[i];
			}
		}
		quickSort( child1->xreal, 0, nreal-1);
		quickSort( child2->xreal, 0, nreal-1);
		printf("\nPuntocruza=%i\n",puntoCruza);
		for( i=0;  i < nreal;  i++){
						printf("%lf,",parent1->xreal[i]);
					}
		printf("\n");
		for( i=0;  i < nreal;  i++){
						printf("%lf,",parent2->xreal[i]);
					}
		printf("\n");
		for( i=0;  i < nreal;  i++){
				printf("%lf,",child1->xreal[i]);
			}
		printf("\n");
		for( i=0;  i < nreal;  i++){
						printf("%lf,",child2->xreal[i]);
					}
}

void quickSort( double a[], int l, int r)
{
   int j;

   if( l < r )
   {
   	// divide and conquer
       j = partition( a, l, r);
       quickSort( a, l, j-1);
       quickSort( a, j+1, r);
   }

}



int partition( double a[], int l, int r) {
   int i, j;
   double pivot, t;
   pivot = a[l];
   i = l; j = r+1;

   while( 1)
   {
   	do ++i; while( a[i] >= pivot && i <= r );
   	do --j; while( a[j] < pivot );
   	if( i >= j ) break;
   	t = a[i]; a[i] = a[j]; a[j] = t;
   }
   t = a[l]; a[l] = a[j]; a[j] = t;
   return j;
}


void realcrossParam (individual *parent1, individual *parent2, individual *child1, individual *child2)
{
	int puntoCruza, i;
	double valor1, valor2;


	int j, posCri, posCriAnt, os, posCriMayor;
	int restante, newW, primario, secundario, pesoAnt, precision;
	double sobrante, wj, W;

	precision=1000;

	double *O;
	int *masc;

	O = (double*) malloc (sizeof(double) * ncriteria);
	masc = (int*) malloc (sizeof(int) * ncriteria);

    for (j=0; j<ncriteria; j++) {
    	masc[j] = rnd(0,1);
    }

	//cruza sección pesos
	//--------------------------------------------------

    for(os=0; os<2; os++)
    {
    	restante=1000;

    	for (j=0; j<ncriteria; j++) O[j]=0;

		for (j=0; j<ncriteria; j++)
		{


			posCri = criterion_position_array[j]-1;


			if(os==0){
				if(masc[j]==0){
					primario = (int) roundf(  parent1->xreal[   posCri   ]  * precision   );
					secundario = (int) roundf(  parent2->xreal[   posCri   ]  * precision   );
				}
				else{
					primario = (int) roundf(  parent2->xreal[   posCri   ]  * precision   );
					secundario = (int) roundf(  parent1->xreal[   posCri   ]  * precision   );
				}
			}
			else{
				if(masc[j]==1){
					primario = (int) roundf(  parent1->xreal[   posCri   ]  * precision   );
					secundario = (int) roundf(  parent2->xreal[   posCri   ]  * precision   );
				}
				else{
					primario = (int) roundf(  parent2->xreal[   posCri   ]  * precision   );
					secundario = (int) roundf(  parent1->xreal[   posCri   ]  * precision   );
				}

			}

			newW = primario;

			if(j>0){
				posCriAnt = criterion_position_array[j-1]-1;
				pesoAnt = roundf(  O[posCriAnt] * precision  );
				if( pesoAnt <= newW )  newW = secundario;
				if( pesoAnt <= newW ){
					if(restante > 0)
						newW = rnd(1, restante);
				}

				if( restante - newW < 0) newW = restante;

				if(j < ncriteria-1 && newW==0){
					if(restante > 0)
						newW = rnd(1, restante);
					else{
						if(pesoAnt > 1){
							newW = rnd(1, pesoAnt-1);
							O[  criterion_position_array[0]-1  ] -= (double)newW / precision;
						}
					}

				}

				if(j == ncriteria-1){
					if( restante - newW > 0) {
						if (  restante < pesoAnt  ) newW=restante;
						else{
							sobrante = (double) restante - newW;
							restante -= (int)sobrante;
							sobrante = sobrante / (double)precision;

							O[  criterion_position_array[0]-1  ] += sobrante;


						}
					}

				}


			}

			if(  newW > 0  ) {
				wj = (double) newW / precision;
			}
			else wj=0;

			O[ posCri ] = wj;

			restante -= newW;

		}

	    W=0;

	    //printf("\n");
	    for (j=0; j<ncriteria; j++){
	    	//printf("%f, ", O[j] );
	    	W += O[j];
	    }


	    /*
	    //corregimos la suma de los pesos por el problema de la precision
	    posCriMayor = criterion_position_array[0]-1;

	    if(W != 1) {
	    	if(W > 1){
	    		O[ posCriMayor  ] = W-1;
	    	}
	    	if(W < 1)
	    		O[ posCriMayor  ] = 1-W;

	    }
	    */

	    W=0.0;
	    posCriMayor = criterion_position_array[0]-1;

	    for (j=0; j<ncriteria; j++){
	    	W += O[j];
	    }

	    if(W > 1.0){
	    	child1->xreal[posCriMayor] = child1->xreal[posCriMayor] - (W-1.0);
	    }
	    if(W < 1.0){
	    	child1->xreal[posCriMayor] = child1->xreal[posCriMayor] + (1.0-W);
	    }


	    for (j=0; j<ncriteria; j++){
	    	W += O[j];
	    	if(os==0) child1->xreal[j] = O[j];
	    	else child2->xreal[j] = O[j];
	    }

    }

    /*
    printf("\nParent 1:\n");
    for (j=0; j<ncriteria; j++){
    	printf("%f, ", parent1->xreal[j] );
    }
    printf("\nParent 2:\n");
    for (j=0; j<ncriteria; j++){
    	printf("%f, ", parent2->xreal[j] );
    }
    */



    free(O);
    free(masc);




	//cruza del resto de genes
	puntoCruza = rnd(ncriteria, nreal-1);
	for( i=0;  i < nreal;  i++){
		if(i==1){
			valor1  =  parent1->xreal[i];
			valor2  =  parent2->xreal[i];
		}

		if( i < puntoCruza ){
			child1->xreal[i]  =  parent1->xreal[i];
			child2->xreal[i]  =  parent2->xreal[i];
		}
		else{
			child1->xreal[i] = parent2->xreal[i];
			child2->xreal[i]  =  parent1->xreal[i];
		}
	}
}

/* Routine for real variable SBX crossover */
void realcross (individual *parent1, individual *parent2, individual *child1, individual *child2)
{
    int i, isNaN;
    double rand;
    double y1, y2, yl, yu;
    double c1, c2;
    double alpha, beta, betaq;
    if (randomperc() <= pcross_real)
    {
    	isNaN=0;
        nrealcross++;
        for (i=0; i<nreal; i++)
        {
            if (randomperc()<=0.5 )
            {
                if (fabs(parent1->xreal[i]-parent2->xreal[i]) > EPS)
                {
                    if (parent1->xreal[i] < parent2->xreal[i])
                    {
                        y1 = parent1->xreal[i];
                        y2 = parent2->xreal[i];
                    }
                    else
                    {
                        y1 = parent2->xreal[i];
                        y2 = parent1->xreal[i];
                    }
                    yl = min_realvar[i];
                    yu = max_realvar[i];
                    rand = randomperc();
                    beta = 1.0 + (2.0*(y1-yl)/(y2-y1));
                    alpha = 2.0 - pow(beta,-(eta_c+1.0));
                    if (rand <= (1.0/alpha))
                    {
                        betaq = pow ((rand*alpha),(1.0/(eta_c+1.0)));
                    }
                    else
                    {
                        betaq = pow ((1.0/(2.0 - rand*alpha)),(1.0/(eta_c+1.0)));
                    }
                    c1 = 0.5*((y1+y2)-betaq*(y2-y1));
                    beta = 1.0 + (2.0*(yu-y2)/(y2-y1));
                    alpha = 2.0 - pow(beta,-(eta_c+1.0));
                    if (rand <= (1.0/alpha))
                    {
                        betaq = pow ((rand*alpha),(1.0/(eta_c+1.0)));
                    }
                    else
                    {
                        betaq = pow ((1.0/(2.0 - rand*alpha)),(1.0/(eta_c+1.0)));
                    }
                    c2 = 0.5*((y1+y2)+betaq*(y2-y1));
                    if (c1<yl)
                        c1=yl;
                    if (c2<yl)
                        c2=yl;
                    if (c1>yu)
                        c1=yu;
                    if (c2>yu)
                        c2=yu;

                    if (randomperc()<=0.5)
                    {
                        child1->xreal[i] = c2;
                        child2->xreal[i] = c1;
                    }
                    else
                    {
                        child1->xreal[i] = c1;
                        child2->xreal[i] = c2;
                    }
                }
                else
                {
                    child1->xreal[i] = parent1->xreal[i];
                    child2->xreal[i] = parent2->xreal[i];
                }
            }
            else
            {
                child1->xreal[i] = parent1->xreal[i];
                child2->xreal[i] = parent2->xreal[i];
                //printf("%g,%g, -> %g,%g\n",  parent1->xreal[i],  parent2->xreal[i], child1->xreal[i], child2->xreal[i]);
            }
        }
    }
    else
    {
        for (i=0; i<nreal; i++)
        {
            child1->xreal[i] = parent1->xreal[i];
            child2->xreal[i] = parent2->xreal[i];
        }
    }



    return;
}

/* Routine for two point binary crossover */
void bincross (individual *parent1, individual *parent2, individual *child1, individual *child2)
{
    int i, j;
    double rand;
    int temp, site1, site2;
    for (i=0; i<nbin; i++)
    {
        rand = randomperc();
        if (rand <= pcross_bin)
        {
            nbincross++;
            site1 = rnd(0,nbits[i]-1);
            site2 = rnd(0,nbits[i]-1);
            if (site1 > site2)
            {
                temp = site1;
                site1 = site2;
                site2 = temp;
            }
            for (j=0; j<site1; j++)
            {
                child1->gene[i][j] = parent1->gene[i][j];
                child2->gene[i][j] = parent2->gene[i][j];
            }
            for (j=site1; j<site2; j++)
            {
                child1->gene[i][j] = parent2->gene[i][j];
                child2->gene[i][j] = parent1->gene[i][j];
            }
            for (j=site2; j<nbits[i]; j++)
            {
                child1->gene[i][j] = parent1->gene[i][j];
                child2->gene[i][j] = parent2->gene[i][j];
            }
        }
        else
        {
            for (j=0; j<nbits[i]; j++)
            {
                child1->gene[i][j] = parent1->gene[i][j];
                child2->gene[i][j] = parent2->gene[i][j];
            }
        }
    }
    return;
}
