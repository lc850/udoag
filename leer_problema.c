/*Calcula la nueva matriz de credibilidad con los parametros del invidiuo y la matriz de performance.*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

# include "global.h"


FILE *fp1_altCri;
FILE *fp1_performance;
FILE *fp2_intercriteria;
FILE *fp3_ordering;
FILE *fp4_ordering;
FILE *fp5_threholds;
FILE *fp6_direction;
FILE *fp7_order_ind;
FILE *fp1_frecuency;
FILE *fp1_marketshare;


FILE *fp8_order_criteria;


void read_ordering_criteria(char *ruta);


void read_data_parameters(char *ruta){

	read_performance(ruta);
	read_criteria_direction(ruta);
	read_ordering_DM(ruta);
	read_frecuency(ruta);
	read_marketshare(ruta);

	read_ordering_criteria(ruta);

	read_ordering_Group(ruta); //old
	//read_thresholMutation(ruta); //old
	//read_intercriteria(ruta); // old


}

//Read the number of alternatives and criteria
void read_size(char *ruta){
	int x, elements=0;
	char archivo[300];

    strncpy(  archivo,   ruta, sizeof(archivo)  );
    strncat(  archivo, "problem.dat", 50);
    fp1_altCri = fopen(archivo, "r");

	//fp1_altCri = fopen("src/model_data/problem.dat", "r");

	if( !fp1_altCri ){
        printf("\nThe problem file was not found\n");
        exit(1);
	}
	//else         printf("\nThe problem file is open\n");

	while(	!feof (fp1_altCri)   ){

		if(elements==3)
			fscanf(fp1_altCri,"%lf\t\n", &lambda);
		else
			fscanf(fp1_altCri,"%d\t\n", &x);

		switch(elements){
			case 0:
				nalternatives = x;
				//itoa (x,str_nalternatives,10);
				sprintf(str_nalternatives, "%d", x);
				break;
			case 1: ncriteria=x; break;
			case 2: use_veto = x; break;
			//case 3: lambda = x; break;
		}
		elements++;
	}
	fclose(fp1_altCri);

}

//Read the original performance matrix by DM
void read_performance(char *ruta){
	int i, j;
	double x;
	char archivo[300];

    strncpy(  archivo,   ruta, sizeof(archivo)  );
    strncat(  archivo, "performance.dat", 50);
    fp1_performance = fopen(archivo, "r");

	//fp1_performance = fopen("src/model_data/performance.dat", "r");

	if( !fp1_performance ){
        printf("\nThe performance matrix file was not found\n%s.", archivo);
        exit(1);
	}
	//else         printf("\nThe performance matrix file is open\n");

	//printf("Alternatives: %d\nCriteria: %d\n",nalternatives, ncriteria);
	i=0; j=0;
	while(	!feof (fp1_performance)   ){

		if(  i == nalternatives   &&   j == ncriteria  ){
	        printf("\nThe performance matrix has more elements that required\n");
	        exit(1);
		}

		fscanf(fp1_performance,"%lf\t\n", &x);
		performance_matrix[i][j]=x;
		//printf("%g\n",x);
		//printf("%d-%d, ",i,j);
		j++;
		if(j==ncriteria) {
			i++;
			if( i != nalternatives) j=0;
			//printf("\n");
		}
	}

	fclose(fp1_performance);

}

//Read the original intercriteria parameters by DM
void read_intercriteria(char *ruta){
	int i ;
	double x;
	char archivo[300];

    strncpy(  archivo,   ruta, sizeof(archivo)  );
    strncat(  archivo, "intercriteria.dat", 50);
    fp2_intercriteria = fopen(archivo, "r");

	//fp2_intercriteria = fopen("src/model_data/intercriteria.dat", "r");

	if( !fp2_intercriteria ){
        printf("\nThe parameters intercriteria file was not found\n");
        exit(1);
	}
	//else         printf("\nThe parameters intercriteria file is open\n");

	i=0;
	while(	!feof (fp2_intercriteria)   ){
		if(  i == nreal  )
		{
	        printf("\nThe parameters intercriteria file has more elements that required\n");
	        exit(1);
		}
		fscanf(fp2_intercriteria,"%lf\t\n", &x);
		//printf("%g\n",x);
		intercriteria_parameters[i] = x;
		i++;

	}
	fclose(fp2_intercriteria);
}

void read_ordering_Group(char *ruta){
	int i ;
	int x;
	char archivo[300];

    strncpy(  archivo,   ruta, sizeof(archivo)  );
    strncat(  archivo, "groupOrder.dat", 50);
    fp3_ordering = fopen(archivo, "r");

	//fp3_ordering = fopen("src/model_data/groupOrder.dat", "r");

	if( !fp3_ordering ){
        printf("\nThe group order file was not found\n");
        exit(1);
	}
	//else         printf("\nThe parameters intercriteria file is open\n");

	i=0;
	while(	!feof (fp3_ordering) && i<nalternatives  ){

		fscanf(fp3_ordering,"%d\t\n", &x);
		//printf("%g\n",x);
		orderG[i] = x;
		i++;

	}
	fclose(fp3_ordering);
}

void read_ordering_DM(char *ruta){
	int i ;
	int x;
	char archivo[300];

    strncpy(  archivo,   ruta, sizeof(archivo)  );
    strncat(  archivo, "DMOrder.dat", 50);
    fp4_ordering = fopen(archivo, "r");

	//fp4_ordering = fopen("src/model_data/DMOrder.dat", "r");

	if( !fp4_ordering ){
        printf("\nThe individual order file was not found\n");
        exit(1);
	}
	//else         printf("\nThe parameters intercriteria file is open\n");

	i=0;
	while(	!feof (fp4_ordering) && i<nalternatives  ){

		fscanf(fp4_ordering,"%d\t\n", &x);
		//printf("%g\n",x);
		orderDM[i] = x;
		i++;

	}
	fclose(fp4_ordering);
}

void read_ordering_criteria(char *ruta){
	int i ;
	int x;
	char archivo[300];

    strncpy(  archivo,   ruta, sizeof(archivo)  );
    strncat(  archivo, "criteria_order.dat", 50);
    fp8_order_criteria = fopen(archivo, "r");

	if( !fp8_order_criteria ){
        printf("\nThe 'criteria_order.dat' file was not found\n");
        exit(1);
	}

	i=0;
	while(	!feof (fp8_order_criteria) && i<ncriteria  ){

		fscanf(fp8_order_criteria,"%d\t\n", &x);
		//printf("%d, ",x);
		criteria_order[i] = x;
		criterion_position_array[ x-1 ] = (i+1) ;
		i++;

	}
	fclose(fp8_order_criteria);

	/*
	printf("\nCriteria order: ");
	for(i=0; i<ncriteria; i++){
		printf("%d, ", criterion_position_array[i]);
	}
	*/


}

void read_ordering_ind(char *ruta){
	int i ;
	int x;
	//char archivo[300];

    //strncpy(  archivo,   ruta, sizeof(archivo)  );
    //strncat(  archivo, "DMOrder.dat", 50);
    fp7_order_ind = fopen(ruta, "r");


	if( !fp7_order_ind ){
        printf("\nThe order from individual file was not found\n");
        exit(1);
	}
	//else         printf("\nThe parameters intercriteria file is open\n");

	i=0;
	while(	!feof (fp7_order_ind) && i<nalternatives  ){

		fscanf(fp7_order_ind,"%d\t\n", &x);
		//printf("%g\n",x);
		orderInd[i] = x;
		i++;

	}
	fclose(fp7_order_ind);
}

void read_thresholMutation(char *ruta)
{
	int i, j, elementos ;
	double x;
	char archivo[300];

    strncpy(  archivo,   ruta, sizeof(archivo)  );
    strncat(  archivo, "thresholdsMutation.dat", 50);
    fp5_threholds = fopen(archivo, "r");


	if( !fp5_threholds ){
        printf("\nThe threholds mutation file was not found\n");
        exit(1);
	}
	//else         printf("\nThe parameters intercriteria file is open\n");

	i=0;
	while(	!feof (fp5_threholds) && i<(ncriteria*3)  ){

		fscanf(fp5_threholds,"%d:", &elementos);

		elements_criThreshold[i] = elementos;
		if(elementos==0) elementos=1;

		criteria_threshold[i] = (double*) malloc ( sizeof(double) * elementos );

		for(  j=0;     j < elements_criThreshold[i];    j++   ){

			if( feof (fp5_threholds) ){
		        printf("\nThe threholds mutation file is not in the correct format\n");
		        exit(1);
			}

			fscanf(fp5_threholds,"%lf\t\n", &x);

			criteria_threshold[i][j]  =  x;

			//printf("%g, ",x);
		}
		//printf("\n");

		i++;

	}
	fclose(fp5_threholds);
}

void read_criteria_direction(char *ruta)
{
	int i ;
	int x;
	char archivo[300];

    strncpy(  archivo,   ruta, sizeof(archivo)  );
    strncat(  archivo, "direction.dat", 50);
    fp6_direction = fopen(archivo, "r");

	//fp4_ordering = fopen("src/model_data/DMOrder.dat", "r");

	if( !fp6_direction ){
        printf("\nThe criteria direction file was not found\n");
        exit(1);
	}
	//else         printf("\nThe parameters intercriteria file is open\n");

	i=0;
	while(	!feof (fp6_direction) && i<ncriteria  ){

		fscanf(fp6_direction,"%d\t\n", &x);
		//printf("%g\n",x);
		criteria_direccion[i] = x;
		i++;

	}
	fclose(fp6_direction);
}

void find_highest_performance_criterion()
{
	//find the highest performance per criteria to know the scale of the criteria
	//create an array of ncriteria size; threshold_epsilon[j] = performance * rango_epsilon
	int i, j;

	for(j=0; j<ncriteria; j++)
	{
		highest_performance[j]  =  performance_matrix[0][j];
		minimum_performance[j]  =  performance_matrix[0][j];
		for(i=1;  i<nalternatives; i++)
		{
			if(   performance_matrix[i][j] > highest_performance[j] )
				highest_performance[j]  =  performance_matrix[i][j];

			if(   performance_matrix[i][j] < minimum_performance[j] )
				minimum_performance[j]  =  performance_matrix[i][j];


		}

		threshold_epsilon[j] = 	highest_performance[j] * rango_epsilon;
		threshold_epsilon[j] =  round(threshold_epsilon[j]);
		if(   threshold_epsilon[j] < 1 )  threshold_epsilon[j] =1;

		//printf("\nC%i: max:%g\t\t min:%g", (j+1),highest_performance[j], minimum_performance[j]);

	}



}
//leer matriz de frecuency y marketsharereal
void read_frecuency(char *ruta){
	int i, j;
	int x;
	char archivo[300];

    strncpy(  archivo,   ruta, sizeof(archivo)  );
    strncat(  archivo, "frecuency.txt", 50);
    fp1_frecuency = fopen(archivo, "r");


	if( !fp1_frecuency ){
        printf("\nThe frecuency matrix file was not found\n%s.", archivo);
        exit(1);
	}
	//else         printf("\nThe performance matrix file is open\n");

	//printf("Alternatives: %d\nCriteria: %d\n",nalternatives, ncriteria);
	i=0; j=0;
	while(	!feof (fp1_frecuency )   ){

		if(  i == nalternatives   &&   j == nalternatives  ){
	        printf("\nThe frecuency matrix has more elements that required\n");
	        exit(1);
		}

		fscanf(fp1_frecuency ,"%d\t\n", &x);
		frecuency[i][j]=x;
		//printf("%d, ",frecuency[i][j]);
		//printf("%d-%d, ",i,j);
		j++;
		if(j==nalternatives) {
			i++;
			if( i != nalternatives) j=0;
			//printf("\n");
		}
	}

	fclose(fp1_frecuency );

}

void read_marketshare(char *ruta){
	int i ;
	double x;
	char archivo[300];

    strncpy(  archivo,   ruta, sizeof(archivo)  );
    strncat(  archivo, "Marketshare.txt", 30);
    fp1_marketshare = fopen(archivo, "r");



	//fp4_market share = fopen("src/model_data/Market_Share_real.txt", "r");

	if( !fp1_marketshare ){
        printf("\nThe  marketshare file was not found\n");
        exit(1);
	}
	//else         printf("\nThe parameters intercriteria file is open\n");

	i=0;
	while(	!feof (fp1_marketshare) && i<nalternatives  ){

		fscanf(fp1_marketshare,"%lf\t\n", &x);
		//printf("%g\n",x);
		marketshare[i] = x;
		i++;

	}
	fclose(fp1_marketshare);
}
