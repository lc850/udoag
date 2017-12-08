/*
 * distillation.c
 *
 *  Created on: 23/08/2016
 *      Author: pavel
 */

#include <stdio.h>
#include <stdlib.h>

# include "global.h"

double cut_k;

int *A; //Set Ak={a1, a2, ..., am} Elements remain the credebility matrix.
int *D;
int *power, *weakness, *qualification;
int **crisp_OR; //crisp outranking relation aSb
int *posDis_asc, *posDis_des, *posTem, *dif_des_asc;
int *sum_medianPreorder, *sum_incomparability, *sum_equivalent;
int *medianPreorder;


int elements_in_D;
int cardA;


void start();
double find_highest();
void finish();
double find_distillation_threshold(double cut_level);
int crisp_outranking_relation(double alpha, double betha);
void D_equal_to_A();
void calculate_qualification();
void obtain_maximum_qualification();
void obtain_minimum_qualification();
void A_minus_C();
void median_preOrder(int *tot_preOrder);


void distillation_Des(double alpha, double betha){

	double s_cut_k;
	int i, k, posOrd, posDistillation, count;
	int count_outrankings;


	//initialize variables
	start();

	cardA = nalternatives;

	//printf("\nDistillation beginning ");
	//printf("\n$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");

	posOrd=1;
	posDistillation =1;

	while( cardA > 1  ){

		D_equal_to_A();

		cut_k = find_highest();
		//cut_k = 0.846;

		s_cut_k = alpha * cut_k + betha;

		/*
		printf("\n\ncut_k: %g\n", cut_k);
		printf("s_cut_k: %g\n", s_cut_k);
		printf("cut_k-s_cut_k: %g\n", cut_k-s_cut_k);
		*/

		cut_k = find_distillation_threshold( cut_k-s_cut_k );


		//printf("cut_k: %g\n", cut_k);


		count_outrankings = crisp_outranking_relation(alpha, betha);
		if( count_outrankings  > 0 ){
			calculate_qualification();
			obtain_maximum_qualification();
		}

		if( elements_in_D == 1   ||   cut_k == 0    ){


			//take of elements from A which are in D
			// A = A \ D
			A_minus_C();

			//printing C
			/*
			printf("\nSet C%d = ",posOrd); posOrd++;
			printf("pd: %d, ",posDistillation);
			*/
			count=0;
			for (i=0; i<nalternatives; i++){

				if( D[i]==1 ){

					//printf("a%d, ", i+1);
					posDis_des[i] = posDistillation;
					count++;

				}
			}

			posDistillation += count;


		}
		else{

			k=0;

			/*
			printf("\nelements_in_D: %d", elements_in_D);
			printf("\ncut_k: %g", cut_k);
			*/

			while(elements_in_D > 1  &&   cut_k != 0 ){


				//printf("\n\nStep k=%d\n-------------------\n", k);

				//printf("\n\ncut_k: %g\n", cut_k);
				s_cut_k = alpha * cut_k + betha;
				cut_k = find_distillation_threshold( cut_k - s_cut_k );

				/*
				printf("cut_k: %g\n", cut_k);
				printf("s_cut_k: %g\n", s_cut_k);
				*/


				count_outrankings = crisp_outranking_relation(alpha, betha);

				if( count_outrankings  > 0 ){
					calculate_qualification();
					obtain_maximum_qualification();
				}

				if( elements_in_D == 1  ||   cut_k == 0   ){
					//take of elements from A which are in D
					// A = A \ D
					A_minus_C();

					//printing C
					/*
					printf("\nSet C%d = ",posOrd); posOrd++;
					printf("pd: %d, ",posDistillation);
					*/
					count=0;

					for (i=0; i<nalternatives; i++){

						if( D[i]==1 ){

							//printf("a%d, ", i+1);
							posDis_des[i] = posDistillation;
							count++;
						}
					}
					posDistillation += count;

				}

				k++;

			}


		}



	}//end while


	if (cardA==1){

		/*
		printf("\ncardA:%d\nA[i]: ", cardA);
		for (i=0; i<nalternatives; i++){
			printf("%d, ",A[i]);

		}*/

		for (i=0; i<nalternatives; i++){
			if(A[i]==1){

				//printf("\nSet C%d = pd: %d, a%d, ",posOrd, posDistillation, i+1);
				posDis_des[i] = posDistillation;
			}


		}


	}

	/*
	printf("\nPos des distillation\n ");
	for (i=0; i<nalternatives; i++){

		printf("%d:%d\n ", i+1, posDis_des[i]);

	}
	*/


	//dellocate memory
	finish();


}

void distillation_Asc(double alpha, double betha){

	double s_cut_k;
	int i, k;
	int posOrd, posAsc, count;
	int count_outrankings;


	//initialize variables
	start();

	cardA = nalternatives;

	/*
	printf("\n\nAscending distillation beginning ");
	printf("\n$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
	*/

	posOrd=1;

	while( cardA > 1  ){


		//Distilation 1
		//k=0

		D_equal_to_A();

		cut_k = find_highest();
		//cut_k = 0.846;

		s_cut_k = alpha * cut_k + betha;

		/*
		printf("\n\ncut_k: %g\n", cut_k);
		printf("s_cut_k: %g\n", s_cut_k);
		printf("cut_k-s_cut_k: %g\n", cut_k-s_cut_k);
		*/

		cut_k = find_distillation_threshold( cut_k-s_cut_k );


		//printf("cut_k: %g\n", cut_k);


		count_outrankings = crisp_outranking_relation(alpha, betha);
		if( count_outrankings  > 0 ){
			calculate_qualification();
			obtain_minimum_qualification();
		}

		if( elements_in_D == 1   ||   cut_k == 0    ){


			//take of elements from A which are in D
			// A = A \ D
			A_minus_C();

			/*
			//printing C
			printf("\nSet C%d = ",posOrd);
			*/
			for (i=0; i<nalternatives; i++){

				if( D[i]==1 ){
					posTem[i]=posOrd;

					//printf("a%d, ", i+1);
				}
			}
			posOrd++;



		}
		else{

			k=0;


			//printf("\nelements_in_D: %d", elements_in_D);
			//printf("\ncut_k: %g", cut_k);

			while(elements_in_D > 1  &&   cut_k != 0 ){


				//printf("\n\nStep k=%d\n-------------------\n", k);

				//printf("\n\ncut_k: %g\n", cut_k);
				s_cut_k = alpha * cut_k + betha;
				cut_k = find_distillation_threshold( cut_k - s_cut_k );

				/*
				printf("cut_k: %g\n", cut_k);
				printf("s_cut_k: %g\n", s_cut_k);
				*/

				count_outrankings = crisp_outranking_relation(alpha, betha);

				if( count_outrankings  > 0 ){
					calculate_qualification();
					obtain_minimum_qualification();
				}

				if( elements_in_D == 1  ||   cut_k == 0   ){
					//take of elements from A which are in D
					// A = A \ D
					A_minus_C();

					//printing C
					//printf("\nSet C%d = ",posOrd);
					for (i=0; i<nalternatives; i++){

						if( D[i]==1 ){
							posTem[i]=posOrd;
							//printf("a%d, ", i+1);
						}
					}
					posOrd++;

				}

				k++;

			}


		}



	}//end while



	if (cardA==1){
		/*
		//imprimir sigma
		int j;
		printf("\n");
		for (i=0; i<nalternatives; i++){
			for (j=0; j<nalternatives; j++){
				printf("%1.2g\t", sigma_matrix[i][j]);
			}
			printf("\n");

		}
		*/

		/*
		//Alternativa faltante, ultima categoría C
		printf("\ncardA:%d\nA[i]: ", cardA);
		for (i=0; i<nalternatives; i++){
			printf("%d, ",A[i]);

		}
		*/

		int valT;
		//find the remaining alternative
		for (i=0; i<nalternatives; i++){
			if(A[i]==1){
				//assign remaining alternative the position of the pre-order
				//printf("\nSet C%d = a%d, ",posOrd, i+1);
				posTem[i]= posOrd;
				valT=posTem[i];
			}


		}
		posOrd++;



	}


	//convert the temporal position to the final position in ascending pre-order
	//as the ascending pre-order is in opposite side (down-up)
	//we change the side to up-down, then best alternatives will be in top position on asc preorder
	posAsc=1;
	for (; posOrd>0; posOrd--){
		count=0;

		for (i=0; i<nalternatives; i++){
			if( posTem[i]== posOrd ){
				posDis_asc[i] = posAsc;
				count++;
			}

		}

		posAsc += count;
	}

	/*
	printf("\nTemp -> Pos des distillation\n ");
	for (i=0; i<nalternatives; i++){

		printf("%d:%d\n ", i+1, posTem[i]);

	}

	printf("\nPos des distillation\n ");
	for (i=0; i<nalternatives; i++){

		printf("%d:%d\n ", i+1, posDis_asc[i]);

	}
	*/


	//dellocate memory
	finish();


}


void median_preOrder(int *tot_preOrder){
	//Intersect des and asc pre-orders (distillation results)
	//count the strict preferences between alternatives from both pre-orders
	int i, j, bucket;
	int mayor, posMayor, vTemp, pTemp;


	//arrays for positions of alternatives for asc and des pre-orders
	// *posDis_asc, *posDis_des, *posTem;
	posDis_asc = (int *) malloc(   nalternatives * sizeof(int)   );
	posDis_des = (int *) malloc(   nalternatives * sizeof(int)   );
	posTem = (int *) malloc(   nalternatives * sizeof(int)   );
	dif_des_asc = (int *) malloc(   nalternatives * sizeof(int)   );


	//array for counting the strict preferences between alternatives from both pre-orders
	sum_medianPreorder = (int *) malloc(   nalternatives * sizeof(int)   );
	sum_incomparability = (int *) malloc(   nalternatives * sizeof(int)   );
	sum_equivalent = (int *) malloc(   nalternatives * sizeof(int)   );
	medianPreorder = (int *) malloc(   nalternatives * sizeof(int)   );


	distillation_Des(-0.15, 0.30);

	distillation_Asc(-0.15, 0.30);


	for (j=0; j<nalternatives; j++){
		if( posDis_des[j] > posDis_asc[j] )
			dif_des_asc[j] = posDis_des[j] - posDis_asc[j];
		else
			dif_des_asc[j] = posDis_asc[j] - posDis_des[j];
	}


	for (j=0; j<nalternatives; j++){
		sum_medianPreorder[j]=0;
		sum_incomparability[j]=0;
		sum_equivalent[j]=0;

		medianPreorder[j]=j;
	}

	//instersection between pre-orders
	for (i=0; i<nalternatives; i++){
		for (j=0; j<nalternatives; j++){
			if(i!=j){

				//find the strict preferences
				//a is better rank in one order and a is at least as good as b
				if( (posDis_des[i] < posDis_des[j] && posDis_asc[i] <= posDis_asc[j]) ||
					(posDis_des[i] <= posDis_des[j] && posDis_asc[i] < posDis_asc[j])
				){
					sum_medianPreorder[i]++;
				}

				//find incomparability
				if( (posDis_des[i] < posDis_des[j] && posDis_asc[i] > posDis_asc[j]) ||
					(posDis_des[i] > posDis_des[j] && posDis_asc[i] < posDis_asc[j])
				){
					sum_incomparability[i]++;
				}

				//find equivalent
				if( posDis_des[i] == posDis_des[j] && posDis_asc[i] == posDis_asc[j] ){
					sum_equivalent[i]++;
				}

			}

		}

	}

	/*
	//print sum
	printf("\nSuma strict preference -> P\n");
	for (j=0; j<nalternatives; j++){
		printf("a%d:%d\n", j+1, sum_medianPreorder[j]);
	}
	*/

	//ordering alternatives


	for (i=0; i<nalternatives-1; i++){

		mayor=sum_medianPreorder[i];
		posMayor=i;

		for (j=i+1; j<nalternatives; j++){
			if( sum_medianPreorder[j] > mayor){
				mayor= sum_medianPreorder[j];
				posMayor=j;
			}
		}

		vTemp=sum_medianPreorder[i];

		sum_medianPreorder[i]= mayor;
		sum_medianPreorder[posMayor]= vTemp;


		pTemp=medianPreorder[i];
		medianPreorder[i] = medianPreorder[posMayor];
		medianPreorder[posMayor]= pTemp;

	}

	/*
	printf("\nPosition des,asc distillation\n");
	for (j=0; j<nalternatives; j++){

		printf("\n%d, %d", posDis_des[ medianPreorder[j] ], posDis_asc[ medianPreorder[j] ] );

	}
	*/

	//printf("\ncomparison\n");
	//distinguishing according to their rank difference
	for (j=0; j<nalternatives-1; j++){

		//they are not in the same bucket
		if( posDis_des[ medianPreorder[j] ] != posDis_des[ medianPreorder[j+1] ] ||
			posDis_asc[ medianPreorder[j] ] != posDis_asc[ medianPreorder[j+1] ]
		) {


			//if both orders have the same rank
			if( sum_medianPreorder[j] == sum_medianPreorder[j+1] ){
				if( dif_des_asc[ medianPreorder[j] ] > dif_des_asc[ medianPreorder[j+1] ] ){
					//printf("\na%d, a%d", medianPreorder[j]+1, medianPreorder[j+1]+1);
					//printf("\t%d,%d", dif_des_asc[medianPreorder[j]], dif_des_asc[medianPreorder[j+1]]);
					pTemp=medianPreorder[j];
					medianPreorder[j] = medianPreorder[j+1];
					medianPreorder[j+1] = pTemp;
				}

			}

		}

	}

	/*
	printf("\nPre-orden:ordenamiento de la suma de P\n");
	for (j=0; j<nalternatives; j++){
		printf("a%d:%d\t%d\n", medianPreorder[j]+1, sum_medianPreorder[j], dif_des_asc[medianPreorder[j]]);
	}
	*/


	bucket=0;
	for (j=0; j<nalternatives; j++){

		tot_preOrder[ medianPreorder[j] ] = bucket;

		if( j+1 < nalternatives ){

			//they are not in the same bucket
			if( posDis_des[ medianPreorder[j] ] != posDis_des[ medianPreorder[j+1] ] ||
				posDis_asc[ medianPreorder[j] ] != posDis_asc[ medianPreorder[j+1] ]
			) {

				bucket++;
			}
		}



	}




	//Move, dellocate after  intersection between des and asc pre-orders
	free(  (void*) posDis_asc  );
	free(  (void*) posDis_des  );
	free(  (void*) posTem  );
	free(  (void*) sum_medianPreorder  );
	free(  (void*) medianPreorder  );
	free(  (void*) sum_incomparability  );
	free(  (void*) sum_equivalent  );
	free(  (void*) dif_des_asc  );



}


//Methods used in both Descending and Ascending procedures
void start(){

	//initialize variables

	A = (int *) malloc(   nalternatives * sizeof(int)   );
	D = (int *) malloc(   nalternatives * sizeof(int)   );

	power = (int *) malloc(   nalternatives * sizeof(int)   );
	weakness = (int *) malloc(   nalternatives * sizeof(int)   );
	qualification = (int *) malloc(   nalternatives * sizeof(int)   );

	crisp_OR = int_allocate_memory_matrix(nalternatives, nalternatives);

	// Initially every element belongs to credibility matrix
	int i;
	for (i=0; i<nalternatives; i++){
		A[i] = 1;
	}





}

void finish(){

	//dellocate memory

	//free(  (void*) remaining_elements  );
	free(  (void*) A  );
	free(  (void*) D  );
	free(  (void*) power  );
	free(  (void*) weakness  );
	free(  (void*) qualification  );



	int_deallocate_memory_matrix(crisp_OR, nalternatives, nalternatives);
}


double find_highest(){

	int i,j;
	double highest;

	highest=0;

	for (i=0; i<nalternatives; i++){

		if( D[i] ){ //if the element a belongs to Dk (credibility matrix)

			for (j=0; j<nalternatives; j++)
			{
				if(j != i)
					if( sigma_matrix[i][j] > highest)
						highest = sigma_matrix[i][j];
			}

		}


	}

	//printf("\nHighest: %g", highest);
	return highest;

}

double find_distillation_threshold(double cut_level){

	//find the richest credibility degree lower than cut_level

	int i,j;
	double highest;

	highest=0;

	for (i=0; i<nalternatives; i++){

		if( D[i] ){ //if the element a belongs to Ak (credibility matrix)

			for (j=0; j<nalternatives; j++)
			{
				if(j != i && D[j])
					if( sigma_matrix[i][j] < cut_level && sigma_matrix[i][j] > highest)
						highest = sigma_matrix[i][j];
			}

		}


	}

	//printf("\nHighest: %g", highest);
	return highest;

}

void D_equal_to_A(){
	int i;

	elements_in_D=0;
	for (i=0; i<nalternatives; i++){
		D[i] = A[i];
		elements_in_D++;
	}


}

void calculate_qualification(){
	int i,j;

	for (i=0; i<nalternatives; i++){

		power[i]=0; weakness[i]=0; qualification[i]=0;

	}


	for (i=0; i<nalternatives; i++){

		if( D[i] ){ //if the element a belongs to Dk (credibility matrix)

			for (j=0; j<nalternatives; j++)
			{
				if(j != i)
					if( crisp_OR[i][j] == 1 ){
						power[i]++;
						weakness[j]++;
					}
			}
		}
	}

	/*
	//print qualification for every alternative
	printf("\n\n");
	for (i=0; i<nalternatives; i++){

		printf("%d\t", i);

	}
	printf("\n");
	*/

	for (i=0; i<nalternatives; i++){

		qualification[i] = power[i] - weakness[i];

		//printf("%d\t", qualification[i]);

	}



}

int crisp_outranking_relation(double alpha, double betha){
	int i,j;
	int No_COR;

	//initialize the matrix
	for (i=0; i<nalternatives; i++){

		for (j=0; j<nalternatives; j++)
		{
			//if(j != i)
				crisp_OR[i][j]=0;
			//else
				//crisp_OR[i][j]=1;

		}

	}

	No_COR=0;

	//find crisp relation
	for (i=0; i<nalternatives; i++){

		if( D[i] ){ //if the element a belongs to D_maximum (credibility matrix)

			for (j=0; j<nalternatives; j++)
			{
				if(j != i && D[j])
					if(  sigma_matrix[i][j] > cut_k  ){
						if(   sigma_matrix[i][j]   >   sigma_matrix[j][i] + ( alpha * sigma_matrix[i][j] + betha )    ){
							crisp_OR[i][j]=1;
							No_COR++;
							/*
							printf("\na,b: %g", sigma_matrix[i][j]);
							printf("\nb,a: %g", sigma_matrix[j][i]);
							printf("\nalpha * a,b + betha: %lf", alpha * sigma_matrix[i][j] + betha );
							*/
						}

					}
			}

		}





	}// for i


	/*
	printf("\n");
	for (i=0; i<nalternatives; i++){

		printf("\n");
		for (j=0; j<nalternatives; j++)
		{
			printf("%d ", crisp_OR[i][j]);

		}

	}
	*/


	return No_COR;

}

void A_minus_C(){
	int i;
	for (i=0; i<nalternatives; i++){
		//take of element from A which are in D
		// A = A \ D
		if(D[i] == 1) {
			A[i]=0;
			cardA--;

		}
	}
}

//Methods for Descending Distillation
//------------------------------------
void obtain_maximum_qualification(){

	int i;
	int maximum;


	maximum = qualification[0];

	for (i=1; i<nalternatives; i++){

		if( D[i]==1 ){

			if( qualification[i] > maximum) maximum = qualification[i];

		}

	}

	//printf("\n\nMaximum: %d\n", maximum);

	//initialize D to be obtained again
	for (i=0; i<nalternatives; i++){
		D[i]=0;

	}

	elements_in_D=0;
	//find the bests qualifications, Next D
	for (i=0; i<nalternatives; i++){

		if( qualification[i] == maximum ){
			D[i]=1;
			elements_in_D++;
		}

	}

	/*
	printf("\nAlternatives in Next D\n");
	for (i=0; i<nalternatives; i++){

		if( D[i]==1 ) printf("%d\t", i);

	}
	*/




}


//Methods for Ascending Distillation
//------------------------------------
void obtain_minimum_qualification(){

	int i;
	int minimum;


	minimum = qualification[0];

	for (i=1; i<nalternatives; i++){

		if( D[i]==1 ){

			if( qualification[i] < minimum) minimum = qualification[i];

		}

	}

	//printf("\n\nMinimum: %d\n", minimum);

	//initialize D to be obtained again
	for (i=0; i<nalternatives; i++){
		D[i]=0;

	}

	elements_in_D=0;
	//find the bests qualifications, Next D
	for (i=0; i<nalternatives; i++){

		if( qualification[i] == minimum ){
			D[i]=1;
			elements_in_D++;
		}

	}

	/*
	printf("\nAlternatives in Next D\n");
	for (i=0; i<nalternatives; i++){

		if( D[i]==1 ) printf("%d\t", i);

	}
	*/




}


