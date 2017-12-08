/* Routines for storing population data into files */

# include <stdio.h>
# include <stdlib.h>
# include <math.h>

# include "global.h"
# include "rand.h"



/* Function to print the information of a population in a file */
void report_pop (population *pop, FILE *fpt)
{
    int i, j, k, sigAlpha;
    for (i=0; i<popsize; i++)
    {
        for (j=0; j<nobj; j++)
        {
        	/*
        	if(j==2) //Max obj
        		fprintf(fpt,"%g\t",  ( -1 * pop->ind[i].obj[j] )   );
        	else*/
        		fprintf(fpt,"%g\t",pop->ind[i].obj[j]);
        }
        if (ncon!=0)
        {
            for (j=0; j<ncon; j++)
            {
                fprintf(fpt,"%g\t",pop->ind[i].constr[j]);
            }
        }
        if (nreal!=0)
        {
            for (j=0; j<nreal; j++)
            {

               	if(j % 7 == 0)  //imprime w
                {
               		fprintf(fpt,"%0.3g\t",pop->ind[i].xreal[j]);
               		fprintf(fpt,"%g\t",pop->ind[i].xreal[j+2]);
               		fprintf(fpt,"%g\t",pop->ind[i].xreal[j+4]);
               		j += 6;
                }

               	/*
            	//q, p y v
               	if(j % 7 == 0)  //imprime w
                {
               		sigAlpha=j+1;
                	fprintf(fpt,"%0.3g\t",pop->ind[i].xreal[j]);
                }
               	else
               		if(sigAlpha != j){  //imprime umbral beta
               			if(pop->ind[i].xreal[j]<1)
               				fprintf(fpt,"%0.3g\t",pop->ind[i].xreal[j]);
               			else
               				fprintf(fpt,"%g\t",pop->ind[i].xreal[j]);
               			//printf("%d, ",j);
               		}
               		else
               		{
               			sigAlpha += 2;
               			//fprintf(fpt,"%g\t",pop->ind[i].xreal[j]); //imprime alpha

               		}

               	*/



            }
        }
        if (nbin!=0)
        {
            for (j=0; j<nbin; j++)
            {
                for (k=0; k<nbits[j]; k++)
                {
                    fprintf(fpt,"%d\t",pop->ind[i].gene[j][k]);
                }
            }
        }
        /*
        fprintf(fpt,"%g\t",pop->ind[i].constr_violation);
        fprintf(fpt,"%d\t",pop->ind[i].rank);
        fprintf(fpt,"%g\n",pop->ind[i].crowd_dist);
        */
        fprintf(fpt,"\n");
    }
    return;
}

/* Function to print the information of feasible and non-dominated population in a file */
void report_feasible (population *pop, FILE *fpt)
{
    int i, j, k, primero=0, acuerdos, desacuerdos;
    int primer_factible=0;
    //int buscaInd=0;
    int pos_x, pos_y, pos_z, pos_t;
    double qj, pj, vj;

    int impresos=0;

    int *ranking;

    ranking = (int *)   malloc (   nalternatives * sizeof(int)   );

    for (k=0; k<nalternatives; k++){
    	ranking[k]=0;

    }


    for (i=0; i<popsize; i++)
    {
    	/*
    	if (pop->ind[i].constr_violation == 0.0 && pop->ind[i].rank==1   ){
			buscaInd = busca_ind_repetido(&pop->ind[i]);
			if(buscaInd == 0) agregaIndividuo_best_pop(pop->ind[i]);
    	}


*/

    	//printf("\nInd %d\t", i);
    	//printf("Constrains:%f \trank:%d", pop->ind[i].constr_violation, pop->ind[i].rank);


        //if (pop->ind[i].constr_violation == 0.0 && pop->ind[i].rank==1 && buscaInd == 0 )
    	if (pop->ind[i].constr_violation == 0.0 && pop->ind[i].rank==1  )
        {
            for (j=0; j<nobj; j++)
            {
            	if(j==0) fprintf(fpt,"\n%g  ",pop->ind[i].obj[j]);

            }
            if (ncon!=0)
            {
                for (j=0; j<ncon; j++)
                {
                    //fprintf(fpt,"%e\t",pop->ind[i].constr[j]);
                }
            }
            if (nreal!=0)
            {
            	/*
                for (j=0; j<nreal; j++)
                {
                    //fprintf(fpt,"%0.2g\t",pop->ind[i].xreal[j]);
                	//q, p y v


                }
                */


                for (j=0; j<ncriteria; j++) //parametro w
                {
                	fprintf(fpt,"%0.4g ",pop->ind[i].xreal[j]);

                	if(!primero){
                		w[j] = pop->ind[i].xreal[j];
                	}
                }

                pos_x = ncriteria * 1;
                pos_y = ncriteria * 2;
                pos_z = ncriteria * 3;
                pos_t = ncriteria * 4;

                j=0;

                while (j < ncriteria)  //parametros y, z
                {
                	//calculamos pj
                	pj = ( pop->ind[i].xreal[ pos_z ] + pop->ind[i].xreal[ pos_y ] ) / 2 ;
					qj = ( pop->ind[i].xreal[ pos_z ] - pop->ind[i].xreal[ pos_y ] ) / 2 ;
					vj = pop->ind[i].xreal[pos_x] + (0.5*pop->ind[i].xreal[ pos_y ]) + (0.5 * pop->ind[i].xreal[ pos_z ]) ;

					fprintf(fpt, "%g\t%g\t%g\t", qj, pj, vj );

                	if(!primero){
                		q[j] = qj;
                		p[j] = pj;
                		v[j] = vj;
                	}

					pos_x++;
					pos_y++;
					pos_z++;


					j++;

                }


                for (j=0; j<ncriteria; j++) //parametro t
                {
                	fprintf(fpt,"%g ",pop->ind[i].xreal[pos_t] );

                	if(!primero){
                		t[j] = pop->ind[i].xreal[ pos_t ];
                	}

                	pos_t++;
                }

                primero=1;


                //imprime lambda
                fprintf(fpt, "%0.2g\t",  pop->ind[i].xreal[nreal-1] );

                credibility_matrix(pop->ind[i].xreal);

                if (primer_factible==0){
                	printf("entra");
                	credibility_matrix_imprime(archivo_VOR, pop->ind[i].xreal);
                	primer_factible=1;
                }

                median_preOrder( orderInd );

                /*
                //imprime el valor de la funci�n objetivo y el orden final
                //---------------------------------------------------------
               	printf("\n\nPares en desacuerdo");
               	acuerdos=0; desacuerdos=0;

               	//comparar ordenamientos - objetivo 1
            	for(  k=0;   k < nalternatives-1;   k++  ){
            		for(   j=k+1;   j < nalternatives;   j++  ){

            				//si a no es mejor b en Odm y Oind		 ó		//si a es mejor b en Odm y Oind
            			if(  (  orderDM[k] > orderDM[j]  && orderInd[k] > orderInd[j]  )   ||
            				 (  orderDM[k] < orderDM[j]  && orderInd[k] < orderInd[j]  )   ||
            				 (  orderDM[k] == orderDM[j]  && orderInd[k] == orderInd[j]  )
            			)
            				acuerdos++;
            			else{
            				desacuerdos++;

            				if(i == 0){ //primer individuo
            					printf("\nA%d, A%d", k+1, j+1);
            				}

            			}

            		}

            	}

            	*/



                //Imprime la posici�n de las alternativas del primer individuo
                //-------------------------------------------------------------
            	if(i==-1){
            		printf("\nOrder position Individuo i=0\n" );
                	for(  j=0;   j < nalternatives;   j++  ){
                		printf("A%d -> %d\n", j+1, orderInd[j]+1 );

                		//ranking[ orderInd[j] ]=j+1;
                	}

                	/*
            		printf("\nRanking Individuo 0\n" );
                	for(  j=0;   j < nalternatives;   j++  ){
                		printf("A%d\n", ranking[j] );

                	}
                	*/
            	}

            	//Imprime el cromosoma del individuo
            	//-------------------------------------------------------------
               	/*
            	printf("\n%d: ", desacuerdos);
            	for(  j=0;   j < nalternatives;   j++  ){
            		printf("%d, ", orderInd[j]+1 );
            	}
            	*/





            } //if (nreal!=0)
            if (nbin!=0)
            {
                for (j=0; j<nbin; j++)
                {
                    for (k=0; k<nbits[j]; k++)
                    {
                        fprintf(fpt,"%d\t",pop->ind[i].gene[j][k]);
                    }
                }
            }

            //fprintf(fpt,"%f\t",pop->ind[i].constr_violation);
            //fprintf(fpt,"%d\n",pop->ind[i].rank);
            //fprintf(fpt,"%f\n",pop->ind[i].crowd_dist);

            impresos++;

        }
    }

    fprintf(fpt, "\n" );

    //printf("\n\nImpresos: %d", impresos);
    //printf("\ntotal_best_pop: %d\n", total_best_pop);

    //credibility_matrix_final();

    free(ranking);

    return;
}


void print_ind (individual *ind, int printAlpha){
	int j;
	printf("\n");
    for (j=0; j<nreal; )
    {
    	if( printAlpha == 1 ){
    		printf("%g\t",ind->xreal[j]);
    		if(j % 7 == 0)
    		{
    			printf("%g\t",ind->xreal[j+1]);
    			j++;
    		}
    		j++;
    	}
    	else{
    		printf("%g\t",ind->xreal[j]); //w
    		printf("%g\t",ind->xreal[j+2]); //q
    		printf("%g\t",ind->xreal[j+4]); //p
    		printf("%g\t",ind->xreal[j+6]); //v
    		j += 7;
    	}

    }

}


int busca_ind_repetido (individual *ind){

    int i,j, mismoIndividuo;
    double i1, i2;

    if(total_best_pop==0) mismoIndividuo=0; //si es el primer individuo no hay otro igual
    else mismoIndividuo=1;

    for (i=0; i<total_best_pop && mismoIndividuo; i++){

        if (nreal!=0)
        {
            for (j=0; j<nreal; j++)
            {

               	if(j % 7 == 0)  //imprime w
                {



					if( ind->xreal[j] != best_pop->ind[i].xreal[j]      ||
					    ind->xreal[j+2] != best_pop->ind[i].xreal[j+2]  ||
						ind->xreal[j+4] != best_pop->ind[i].xreal[j+4]
					){
						i1 = ind->xreal[j];
						i2 = best_pop->ind[i].xreal[j];
						mismoIndividuo = 0;
						j=nreal;
					}

               		j += 6;

                }


            }
        }

    }

    return mismoIndividuo;


}


void agregaIndividuo_best_pop(individual ind){
	best_pop->ind[total_best_pop] = ind;
	total_best_pop++;


}

