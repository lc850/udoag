/*Calcula la nueva matriz de credibilidad con los parametros del invidiuo y la matriz de performance.*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

# include "global.h"

double fitness_borda(double*xreal){

	int i;
	int j;
	for (j=0; j<nalternatives; j++){
		b[j]=0;
	}

	printf("\n\nB\n");
	for (j=0; j<nalternatives; j++){
		for(i=0; i<nalternatives; i++){
			b[j]=b[j]+ frecuency [i][j]*xreal[i];
		}
		printf("%lf, ", b[j] );

	}
	printf("\n");



	double suma=0;
	for (j=0; j<nalternatives; j++){
		suma+=b[j];


	}
	printf ("\n sumatoria de B: %lf", suma);

	for(j=0; j<nalternatives; j++){
			b[j]= b[j]/suma;


			printf("\n participación de Bj: %lf", b[j]);
	}
	double error=0;
	double dif;

	for( j=0; j<nalternatives; j++){
		if(marketshare[j]>=b[j]){
		dif=marketshare [j]-b[j];
		}
		else{
			dif=b[j]-marketshare[j];

		}
				 error+= dif;
	}
		printf("\n error total: %lf", error);


	return error;
}

double eva_objective_kendall (double *xreal){
	int i, j, acuerdos=0, desacuerdos=0;
	//char c[5];
	//int Oa_b;
	double penalty=0, parameter_p=0.5;


	if(genActual <= ngen){
		credibility_matrix(xreal);
		//net_flow();
		//median_preOrder( total_preOrder );
		median_preOrder( orderInd );

		/*
		printf("\nPre-orden total\n");
		for (j=0; j<nalternatives; j++){
			printf("a%d:%d\n", j+1, orderInd[j] );
		}
		*/


	}
	else{

		credibility_matrix_imprime("ranking-sadage/VOR_temp.out", xreal);

		double lambda = xreal[nreal-1];
		char executionCommand[600];
		char str_lambda[30];

		// convierte de flotante a string
		sprintf(str_lambda, "%f", lambda);
		sprintf(str_nalternatives, "%d", nalternatives);

		 //procedimiento para llamar otro programa desde el sistema
		strcpy(executionCommand, "ranking-sadage/ranking-total-sadage <ranking-sadage/sadage-inputAG-dm.in 0.3 ");
		strcat(executionCommand, str_nalternatives);
		strcat(executionCommand, " ");
		strcat(executionCommand, str_lambda);

		//printf("\n%s", executionCommand);
		//printf("\n");
		//exit(0);

		if ( mi_system(executionCommand) != -1 ){
			read_ordering_ind("ranking-sadage/ranking.out");
		}
		else{
			printf("\nError al ejecutar el programa '%s'", executionCommand);
			exit(0);
		}
		//printf("\ncorriendo: %d",genActual);
	}


	//kendall para preOrden total

	for(  i=0;   i < nalternatives-1;   i++  ){
		for(   j=i+1;   j < nalternatives;   j++  ){

			//Caso 1
				//si a no es mejor b en Odm y Oind		 ó		//si a es mejor b en Odm y Oind
			if(  (  orderDM[i] > orderDM[j]  && orderInd[i] > orderInd[j]  )   ||
				 (  orderDM[i] < orderDM[j]  && orderInd[i] < orderInd[j]  )  ||
				 (  orderDM[i] == orderDM[j]  && orderInd[i] == orderInd[j]  )
			){
				acuerdos++;
				//Penalty es 0
			}
			else{

				if( (  orderDM[i] > orderDM[j]  && orderInd[i] < orderInd[j]  ) ||
					(  orderDM[i] < orderDM[j]  && orderInd[i] > orderInd[j]  )
				){
					penalty += 1;
				}
				else{

					if(
						(  orderDM[i] == orderDM[j]  && ( orderInd[i] > orderInd[j] || orderInd[i] < orderInd[j]  ) )  ||
						(  ( orderDM[i] < orderDM[j]  || orderDM[i] > orderDM[j] ) &&  orderInd[i] == orderInd[j] )
					)
					{
						penalty += parameter_p;
					}

				}



			}


			/*
			//Caso 2
			if(  orderDM[i] == orderDM[j]  && orderInd[i] == orderInd[j]  ){
				//Penalty es 0
				acuerdos++;
			}

			//Caso 3
			if( (  orderDM[i] == orderDM[j]  && ( orderInd[i] > orderInd[j] || orderInd[i] < orderInd[j]  ) ) ||
				(  ( orderDM[i] < orderDM[j]  || orderDM[i] > orderDM[j] ) &&  orderInd[i] == orderInd[j] )
			){
				penalty += parameter_p;

			}
			*/

		}

	}

	//printf("\nKp(DM,ind)= %lf", penalty);

	/*
	//imprimir la última generación
	if(genActual == ngen-1){

		printf("\n%d: ", desacuerdos);
		for(   j=0;   j < nalternatives;   j++  ){
			printf("%d, ", orderInd[j] );
		}
	}
	*/





//kendal usando matriz sigma y no ranking para individuo
	/*
	for(  i=0;   i < nalternatives-1;   i++  ){
		for(   j=i+1;   j < nalternatives;   j++  ){

			Oa_b = RO_lambda_better_than(sigma_matrix, i, j, lambda, 0.015);

				//si a no es mejor b en OG y a-Sb		 ó		//si a mejor b en OG y aSb
			if(  (  orderDM[i] > orderDM[j]  && !Oa_b  )   ||   (  orderDM[i] < orderDM[j]  && Oa_b  )  )
				acuerdos++;
			else{
				desacuerdos++;

			}

		}

	}
	*/

	return penalty;
}

void net_flow(){
	int i, j;
	double *nf_plus;
	double *nf_minus;
	double *nf_global;

	nf_plus = (double*) malloc (  sizeof(double) * nalternatives  );
	nf_minus = (double*) malloc (  sizeof(double) * nalternatives  );
	nf_global = (double*) malloc (  sizeof(double) * nalternatives  );

	for(  i=0;   i < nalternatives;   i++  ){
		nf_plus[i]=0;
		nf_minus[i]=0;
		nf_global[i]=0;

	}

	//net flujo positivo
	for(  i=0;   i < nalternatives;   i++  ){
		for(   j=0;   j < nalternatives;   j++  ){
			if(i != j){
				nf_plus[i] += sigma_matrix[i][j];
			}
		}
	}

	//printf("\nPositivo: ");
	for(  i=0;   i < nalternatives;   i++  ){
		nf_plus[i] = (double) 1 / (nalternatives-1) * nf_plus[i];
	}


	//net flujo negativo
	for(  i=0;   i < nalternatives;   i++  ){
		for(   j=0;   j < nalternatives;   j++  ){
			if(i != j){
				nf_minus[i] += sigma_matrix[j][i];
			}
		}
	}

	//printf("\nNegativo: ");
	for(  i=0;   i < nalternatives;   i++  ){
		nf_minus[i] = (double) 1 / (nalternatives-1) * nf_minus[i];
	}

	//printf("\n");
	//net flujo global
	for(  i=0;   i < nalternatives;   i++  ){
		nf_global[i]= nf_plus[i] - nf_minus[i];
		//printf("%lf, ", nf_global[i] );
	}

	for(  i=0;   i < nalternatives;   i++  ){
		orderInd[i]=i;
	}


	/*
	printf("\nOrdenamiento: ");
	for(  i=0;   i < nalternatives;   i++  ) printf("%d, ", orderInd[i] );

	printf("\nGlobal desordenado: ");
	for(  i=0;   i < nalternatives;   i++  ) printf("%f, ", nf_global[i] );
	 */
	ordenar_valores(nf_global, orderInd );


	//printf("\nOrdenamiento: ");
	//printf("\n");
	//for(  i=0;   i < nalternatives;   i++  ) printf("%d, ", orderInd[i]+1 );



	/*
	printf("\nGlobal ordenado: ");
	for(  i=0;   i < nalternatives;   i++  ) printf("%f, ", nf_global[i] );

	printf("\n");
	*/

	free(nf_plus);
	free(nf_minus);
	free(nf_global);



}


//count the number of modified intercriteria parameters
int eva_objective1 (double *xreal){
	int i, dif=0;
	for(i=0; i<nreal; i++){
		if(xreal[i] != intercriteria_parameters[i]) dif++;
	}
	return dif;
}


int eva_objective2 (){
	int i, j, acuerdos=0, desacuerdos=0;
	int Oa_b;

	for(  i=0;   i < nalternatives-1;   i++  ){
		for(   j=i+1;   j < nalternatives;   j++  ){

			Oa_b = RO_lambda_better_than(sigma_matrix, i, j, lambda, 0.15);

				//si a no es mejor b en OG y a-Sb		 ó		//si a mejor b en OG y aSb
			if(  (  orderG[i] > orderG[j]  && !Oa_b  )   ||   (  orderG[i] < orderG[j]  && Oa_b  )  )
				acuerdos++;
			else{
				desacuerdos++;

			}

		}

	}

	return desacuerdos;
}


int eva_objective3 (){
	int i, j, acuerdos=0;
	int Oa_b;
	//int Ob_a;

	for(  i=0;   i < nalternatives-1;   i++  ){
		for(   j=i+1;   j < nalternatives;   j++  ){

			if(  firstRanking_agrees[i][j] >=0  ){  //tienen acuerdo

				Oa_b = RO_lambda_better_than(sigma_matrix, i, j, lambda, 0.15);
				//Ob_a = RO_lambda_better_than(sigma_matrix, j, i, lambda, 0.15);

					//si a mejor b									ó		//si b mejor a
				if(  (  Oa_b  &&  firstRanking_agrees[i][j] == 1  )   ||   (  !Oa_b  &&  firstRanking_agrees[i][j] == 0  ) 	)
					acuerdos++;

				/*
				if(   orderG[i] > orderG[j]  && firstRanking_agrees[i][j] == 0       ) //se mantiene el acuerdo b es mejor que a
					acuerdos++;
				else if  (  orderG[i] < orderG[j]  && firstRanking_agrees[i][j] == 1  )//se mantiene el acuerdo a es mejor que b
					acuerdos++;
				*/

			}



		}

	}

	return acuerdos;
}

int eva_constr_violation(double *xreal, double *obj)
{
	int i, pos, violation=0;
	double sum_w=0;
	//double q, p;
	double epsilon_w, limit_epsilon_Wpos, limit_epsilon_Wneg;

	for( i=0; i < ncriteria; i++){
		pos = i*7;

		sum_w += xreal[pos];

		//pesos negativos
		if(xreal[pos] < 0) violation--;

		//consistencia en umbrales v>p>q > 0,
		if( xreal[pos+2] >  xreal[pos+4] )
			violation--;

		if( xreal[pos+2] < 0 ||  xreal[pos+4] < 0   ||   xreal[pos+6] < 0)
			violation--;

		if( use_veto  &&  xreal[pos+4] >  xreal[pos+6]    )
			violation--;

		//q = xreal[pos+2]; p= xreal[pos+4];
		//Umbrales estén en el rango deseado < performance
		if( xreal[pos+2] >  highest_performance[i] ) violation--;
		if( xreal[pos+4] >  highest_performance[i] ) violation--;
		if( use_veto  &&  xreal[pos+6]  >  highest_performance[i]    )	violation--;


		//Evaluar el rango de epsilon mutado
		epsilon_w =  intercriteria_parameters[ pos ] * rango_epsilon;


		//mantener los valores de los parametros dentro de una vecindad
		limit_epsilon_Wpos = intercriteria_parameters[ pos ] + epsilon_w;
		limit_epsilon_Wneg = intercriteria_parameters[ pos ] - epsilon_w;

		if( xreal[pos] > limit_epsilon_Wpos || xreal[pos] < limit_epsilon_Wneg){
			violation--;
		}



	}

	// si la suma de los pesos no da 1, 10 ó 100
	if(sum_w != 1    &&    sum_w !=10    &&    sum_w != 100)
		violation--;


	// si el número de parametros modificados es cero
	if(obj[0] < 1)
		violation--;







	return violation;
}

int eva_constr_violation_kendall(double *xreal)
{

    int j, pos_x, pos_y, pos_z, pos_t;
    int violation=0, posCri, posCriAnt;

    double W, max_valor_Pj;

    W=0;
    for (j=0; j<ncriteria; j++) //parametro w
    {
		W += xreal[j];
		//if( xreal[j] < 0 ) violation--;

		posCri= criterion_position_array[j]-1;

		if( j>0 ){
			posCriAnt = criterion_position_array[ j-1 ]-1;
			if( xreal[ posCri  ] >= xreal[ posCriAnt  ])
				violation--;

		}

		if (  xreal[ j  ] > 1.0 ){
			violation--;
		}

    }

    if(  W < 0.9999999  ||  W  > 1.0000001 ){
    	violation--;
    }


    /*
    //corregimos la suma de los pesos por el problema de la precision

    int posCriMayor, posCriMenor;
    double newW, may;

    posCriMayor = criterion_position_array[0]-1;
    posCriMenor = criterion_position_array[ ncriteria-1 ]-1;

    may = xreal[ posCriMayor  ];
    if(W != 1) {
    	if(W > 1){
    		newW = W-1;
    		xreal[ posCriMayor  ] -= newW;
    	}
    	if(W < 1){
    		newW = 1-W;
    		xreal[ posCriMayor  ] += newW;
    	}
    	may = xreal[ posCriMayor  ];

    }
    */



    pos_x = ncriteria * 1;
    pos_y = ncriteria * 2;
    pos_z = ncriteria * 3;
    pos_t = ncriteria * 4;
    j=0;

    double vy, vz;

    while (j < ncriteria)  //parametros y, z
    {
    	max_valor_Pj = highest_performance[j] - minimum_performance[j];
    	if (  (xreal[ pos_y ] + xreal[ pos_z ])  >  (2*max_valor_Pj)   )
    	{
    		violation--;
    	}

    	if ( xreal[ pos_y ] >= xreal[ pos_z ]  ){
    		violation--;
    	}

    	if( xreal[ pos_x ] < 0 ){
    		violation--;
    	}

    	if( xreal[ pos_y ] < 0 ){
    		violation--;
    	}

    	if( xreal[ pos_z ] < 0 ){
    		violation--;
    	}

    	if( xreal[ pos_t ] < 0 ){
    		violation--;
    	}

    	if( xreal[ pos_t ] > 1 ){
    		violation--;
    	}

    	pos_x++;
		pos_y++;
		pos_z++;
		pos_t++;
		j++;

    }

    /*
    if(violation<0)
    	printf( "\nViolations: %d", violation);
    */


    return violation;
}

int RO_lambda_better_than(double **sigma, int a, int b, double lambda, double B){
	int RO_lambda=-1;
	//int preferencia[]={1, 1, 1, 0, 1, 1, 0, 0, 0};
	int preferencia[]={1, 1, 1, 1, 1, 1, 0, 0, 0};
	double Sab, Sba;

	Sab = sigma[a][b];
	Sba = sigma[b][a];

	if( sigma[a][b] >= lambda){

		if(  sigma[b][a] <= lambda-B )
			RO_lambda = preferencia[0];
		else if( lambda-B < sigma[b][a] && sigma[b][a] < lambda )
			RO_lambda = preferencia[1];
		else
			RO_lambda = preferencia[2];
	}
	else{
		RO_lambda = 0;
	}

	return RO_lambda;

}

//calcula el sigma (matriz de credibilidad)
void credibility_matrix(double *xreal){
	//genera la matriz de credibilidad a partir de xreal, umbrales y dirección de criterios
	//______________________________________________________________________________________

	int i, j;

	//int *direccion= (int*) malloc (sizeof(int) * ncriteria); //min:0, max:1

	/*
	double *w = (double*) malloc (sizeof(double) * ncriteria);
	double *q= (double*) malloc (sizeof(double) * ncriteria);
	double *p= (double*) malloc (sizeof(double) * ncriteria);
	double *v= (double*) malloc (sizeof(double) * ncriteria);
	double *t= (double*) malloc (sizeof(double) * ncriteria);
	*/


	//direccion[0] =0; direccion[1]=1;

	//toma los valores del individul xreal y los convierte en parámetros intercriterios q, p, v
	organiza_parametros_intercriterios(xreal, w, q, p, v, t);


	/*
	w[0]=0.4; w[1]=0.25; w[2]=0.15; w[3]=0.1; w[4]=0.1;
	q[0]=0.2; q[1]=0.2; q[2]=4.0; q[3]=1.0; q[4]=0.5;
	p[0]=0.5; p[1]=0.5; p[2]=9.0; p[3]=1.5; p[4]=1.0;
	v[0]=1.0; v[1]=1.0; v[2]=40.0; v[3]=6.0; v[4]=7.0;
	t[0]=1.0; t[1]=1.0; t[2]=1.0; t[3]=1.0; t[4]=1.0;



    //imprime performance e intercriterios
    printf( "\n\nDesempeños de alternativas\n" );
    //int j;
     for(i=0; i<nalternatives; i++){
     	for(j=0; j<ncriteria; j++) {
     		printf("%lf, ",performance_matrix[i][j]);
     	}
     	printf("\n");
     }


     printf( "\nW\n" );
  	for(j=0; j<ncriteria; j++) {
  		printf("%g, ",w[j]);
  	}
  	printf("\n");

    printf( "\nQ\n" );
 	for(j=0; j<ncriteria; j++) {
 		printf("%g, ",q[j]);
 	}
 	printf("\n");

    printf( "\nP\n" );
 	for(j=0; j<ncriteria; j++) {
 		printf("%g, ",p[j]);
 	}
 	printf("\n");



    printf( "\nV\n" );
 	for(j=0; j<ncriteria; j++) {
 		printf("%g, ",v[j]);
 	}
 	printf("\n");

    printf( "\nT\n" );
 	for(j=0; j<ncriteria; j++) {
 		printf("%g, ",t[j]);
 	}
 	*/




	//Se construye la matriz de credibilidad VOR
	for(i=0; i<nalternatives; i++){
		for(j=0; j<nalternatives; j++){

			if(j==i){
				sigma_matrix[i][j] = 1;
				//printf("\nsigma: 1");
			}
			else	sigma_matrix[i][j] = sigma2(performance_matrix[i], performance_matrix[j], w, q, p, v, t, criteria_direccion );


			/*
			if(  genActual == 1  ){
				printf("%1.2g\t",sigma_matrix[i][j] );

			}
			*/


		}
		//if(  genActual == 1  )	printf("\n");
	}


}


void credibility_matrix_final(){

	//Genera la matriz de credibilidad y la imprime en el archivo f_sigma definido en main()
	//---------------------------------------------------------------------------------------

	//Antes de llamar esta función se debe llamar a:

	//toma los valores del individul xreal y los convierte en parámetros intercriterios q, p, v
	//organiza_parametros_intercriterios(xreal, w, q, p, v, t);

	int i, j;


	/*

	w[0]=0.4; w[1]=0.25; w[2]=0.15; w[3]=0.1; w[4]=0.1;
	q[0]=0.2; q[1]=0.2; q[2]=4.0; q[3]=1.0; q[4]=0.5;
	p[0]=0.5; p[1]=0.5; p[2]=9.0; p[3]=1.5; p[4]=1.0;
	v[0]=1.0; v[1]=1.0; v[2]=40.0; v[3]=6.0; v[4]=7.0;
	t[0]=1.0; t[1]=1.0; t[2]=1.0; t[3]=1.0; t[4]=1.0;



    //imprime performance e intercriterios
    printf( "\n\nDesempeños de alternativas\n" );
    //int j;
     for(i=0; i<nalternatives; i++){
     	for(j=0; j<ncriteria; j++) {
     		printf("%lf, ",performance_matrix[i][j]);
     	}
     	printf("\n");
     }



     printf( "\nW\n" );
  	for(j=0; j<ncriteria; j++) {
  		printf("%g, ",w[j]);
  	}
  	printf("\n");

    printf( "\nQ\n" );
 	for(j=0; j<ncriteria; j++) {
 		printf("%g, ",q[j]);
 	}
 	printf("\n");

    printf( "\nP\n" );
 	for(j=0; j<ncriteria; j++) {
 		printf("%g, ",p[j]);
 	}
 	printf("\n");

    printf( "\nV\n" );
 	for(j=0; j<ncriteria; j++) {
 		printf("%g, ",v[j]);
 	}
 	printf("\n");

    printf( "\nT\n" );
 	for(j=0; j<ncriteria; j++) {
 		printf("%g, ",t[j]);
 	}

 	*/

	printf( "\nMatriz outranking valuada\n" );






	//Se construye la matriz de credibilidad VOR
	for(i=0; i<nalternatives; i++){
		for(j=0; j<nalternatives; j++){

			if(j==i){
				sigma_matrix[i][j] = 1;
			}
			else	sigma_matrix[i][j] = sigma2(performance_matrix[i], performance_matrix[j], w, q, p, v, t, criteria_direccion );

			//printf("%1.2g\t",sigma_matrix[i][j] );
			fprintf(f_sigma, "%1.2g\t", sigma_matrix[i][j]);

		}
		//printf("\n");
		fprintf(f_sigma, "\n");
	}


}


void credibility_matrix_imprime(char *ruta, double *xreal){
	//genera la matriz de credibilidad e imprime la matris en el archivo *ruta
	//______________________________________________________________________________________


	int i, j;

    FILE *f_sigmaTemp = fopen(ruta,"w"); //VOR (sigma) resultante del  individuo actual
    //fflush(f_sigmaTemp);


	//printf( "\nMatriz outranking valuada\n" );

    organiza_parametros_intercriterios(xreal, w, q, p, v, t);

	//Se construye la matriz de credibilidad VOR
	for(i=0; i<nalternatives; i++){
		for(j=0; j<nalternatives; j++){

			if(j==i){
				sigma_matrix[i][j] = 1;
			}
			else	sigma_matrix[i][j] = sigma2(performance_matrix[i], performance_matrix[j], w, q, p, v, t, criteria_direccion );

			//printf("%1.2g\t",sigma_matrix[i][j] );
			//2 decimales
			//fprintf(f_sigmaTemp, "%0.2g\t", sigma_matrix[i][j]);

			//precision completa
			fprintf(f_sigmaTemp, "%lf\t", sigma_matrix[i][j]);


		}
		//printf("\n");
		fprintf(f_sigmaTemp, "\n");
	}

	fflush(f_sigmaTemp);
	fclose(f_sigmaTemp);

	/*

	f_sigmaTemp = fopen("VOR_respaldo.out","w");

	//Se construye respaldo de la matriz de credibilidad VOR
	for(i=0; i<nalternatives; i++){
		for(j=0; j<nalternatives; j++){

			fprintf(f_sigmaTemp, "%0.2g\t", sigma_matrix[i][j]);

		}

		fprintf(f_sigmaTemp, "\n");

	}

	fflush(f_sigmaTemp);
	fclose(f_sigmaTemp);
	*/


}



//calcula el sigma a partir de los parametros de un individuo
double sigma2 (  double A[], double B[], double *W, double *Q, double *P, double *V,  double *T, int *dir   ){
	int k;
	double sig;
	//int usarVeto = 0;

	double d, discordancia, c, concordancia, valDis;
	double a ,b, q, p, w;

	sig=0;

	//printf("\n--c--");

	//concordancia
	//------------------------------------

	concordancia =0;
	for(k=0;k<ncriteria;k++){

		a = A[k]; b  =B[k]; q  =Q[k]; p  =P[k], w =W[k];

		if(dir[k]==1) //maximizar
		{
			if(   B[k] <= ( A[k] + Q[k] )    ){ //aSb
			  c = 1;
			}
			else{

				if(   B[k] > ( A[k] + P[k] )   ){
					c =0;
				}
				else{
					if(  ( A[k] + Q[k] ) < B[k]    &&    B[k] <= ( A[k] + P[k] )      ){

						c =   ( P[k] - ( B[k] - A[k] ) )  /   (  P[k] -  Q[k] )  ;
					}
				}
			}
		}
		else //minimizar
		{
			if(  (  ( A[k] - B[k] )  <=  Q[k] )  ||   ( A[k] <= B[k]   )     ){ //aSb
			  c = 1;
			}
			else{
				if( ( A[k] - B[k] )  >  P[k]    )
				{
					c =0 ;
				}
				else
				{
					if(  Q[k] < ( A[k] - B[k] )   &&    ( A[k] - B[k] ) <= P[k]    ){
						c =  ( P[k] - ( A[k] - B[k] ) )  /   ( P[k] - Q[k] )  ;
					}
				}
			}

		}

		//printf("c%i: %1.2g, ",k, (c * W[k]) );

		concordancia += c * W[k];


	}

	//printf("\nConcordancia: %1.2g",concordancia);





	//discordancia
	//-----------------------------

	//printf("\n-- discordancia -- \n" );

	discordancia=0;
	for(k=0;k<ncriteria;k++){

		a = A[k]; b  =B[k]; q  =Q[k]; p  =P[k], w =W[k];

		d = 0;

		if( T[k]==1 ){ //Si el criterio va usar veto

			d = -1; //para problar si esta entrando a las condiciones

			if(dir[k]==1) //maximizar
			{
				if(  ( B[k]-A[k] ) <= P[k]  ){
					d=0;
				}
				else{
					if(  P[k] < ( B[k]-A[k] )  &&  ( B[k]-A[k] ) < V[k]  ){
						d = (  ( B[k]-A[k] ) - P[k]  )  /   (  V[k] - P[k]  );
					}
					else{
						if(  ( B[k]-A[k] ) >= V[k]   ){
							d = 1;
						}

					}

				}
			}
			else{ //minimizar

				if( (A[k]-B[k])  <= P[k] ){
					d=0;
				}
				else{
					if(  P[k]  < (A[k]-B[k])   &&  ( A[k]-B[k] < V[k] )   ){
						d = ( A[k] - P[k] - B[k] )  /   (V[k]- P[k]);
					}
					else{
						if(  A[k]-B[k]  >=  V[k] ){
							d = 1;
						}
					}

				}

			}

		}

		if(d==1){
			valDis = 1;
			discordancia = 1; //esto veta al resto de criterios
		}
		else{
			if( d > concordancia && discordancia != 1  ){
				valDis = ( 1-d ) / ( 1 - concordancia);
				if(discordancia == 0) discordancia = valDis; //asigna el valor, si se multiplica por cero resulta cero.
				else discordancia *= valDis;
			}
			else{
				valDis =0;
			}
		}



		//printf("c%i: %1.2g, ",k,  d );


	}

	//printf("\nDiscordancia: %1.2g",discordancia);

	if(discordancia > 0){
		if(discordancia == 1)
			sig = 0;
		else
			sig = concordancia * discordancia;
	}
	else
		sig = concordancia;


  return sig;

}


//calcula el sigma a partir de los parametros de un individuo
double sigma (  double A[], double B[], double *W, double *Q, double *P, double *V,  double *T, int *dir   ){
	int k, hubo_veto;
	double sig=0,disc,tempdisc;
	//int usarVeto = 0;

	//Primero se checa veto
	hubo_veto=0;
	sig=0;

	if(use_veto){
		for(k=0;k<ncriteria;k++){

			if( T[k]==1 ){ //Si el criterio va usar veto

				if(  (  B[k]-A[k]  ) >= V[k]  ){
					hubo_veto = 1; break;
				}
			}
		}
	}
	//Fin de chequeo de veto


	//Si no existe veto calcular sig
	if(  hubo_veto == 0  )
	{

		disc=1; tempdisc=1;

		for(k=0;k<ncriteria;k++){

			if(dir[k]==1) //maximizar
			{
				if(   B[k] <= ( A[k] + Q[k] )    ){ //aSb
				  sig=sig+W[k];
				}
				else{

					if(   B[k] > ( A[k] + P[k] )   ){  //


						//sig = sig +  0  ; //bSa

						//tempdisc    =    1  -    (   ( (double)B[k]  -  (double)A[k] )  -  P[k]   )   /   (   (double)V[k] - (double)P[k]  );

						if(   tempdisc < disc   ){
							disc=tempdisc;
						}
					}
					else{

						if(  ( A[k] + Q[k] ) < B[k]    &&    B[k] <= ( A[k] + P[k] )      ){

							sig = sig +      W[k]  *  ( P[k] - ( B[k] - A[k] ) )  /   (  P[k] -  Q[k] )  ;

						}

					}

				}
			}
			else //minimizar
			{
				if(    ( A[k] - B[k] )  <=  Q[k]   ){ //aSb
				  sig=sig+W[k];
				}
				else{
					if( ( A[k] - B[k] )  >  P[k]    )
					{
						if(   tempdisc < disc   ){
							disc=tempdisc;
						}
					}
					else
					{
						if(  Q[k] < ( A[k] - B[k] )   &&    ( A[k] - B[k] ) <= P[k]    ){
							sig = sig +      W[k]  *  ( P[k] - ( A[k] - B[k] ) )  /   ( P[k] - Q[k] )  ;
						}
					}
				}

			}

		}

		sig=sig*disc;

	}//if( hubo_veto==0 ){

  return sig;

}

void organiza_parametros_intercriterios(double *xreal, double *pw, double *pq, double *pp, double *pv, double *pt){
	int i, pos_x, pos_y, pos_z, pos_t;

    pos_x = ncriteria * 1;
    pos_y = ncriteria * 2;
    pos_z = ncriteria * 3;
    pos_t = ncriteria * 4;

	for(i=0; i<ncriteria; i++){

		pw[i]= xreal[i];
		pq[i]=  ( xreal[ pos_z ] -  xreal[ pos_y ] ) / 2;
		pp[i]=  ( xreal[ pos_z ] +  xreal[ pos_y ] ) / 2;
		pv[i]=  xreal[pos_x] + (0.5*xreal[ pos_y ]) + (0.5 * xreal[ pos_z ]) ;
		pt[i]= xreal[ pos_t ];

		pos_x++;
		pos_y++;
		pos_z++;
		pos_t++;

		//printf( "\nC%i: wj:%g \t qj:%g \t pj:%g \t vj:%g", i,pw[i], pq[i], pp[i], pv[i] );

	}
	//printf( "\n");


}

//identifica los pares en acuerdo entre el DM y el grupo
void firstRanking_agress_matrix(int **agree, int *Oi, int *OG){
	int i, j;
	int agrees=0;

	for( i=0;   i < nalternatives-1;   i++  ){
		for(  j=i+1;  j < nalternatives;  j++  ){

			if(  OG[i] > OG[j]   &&   Oi[i] > Oi[j]   ){  //acuerdo en que b es mejor que a
				firstRanking_agrees[i][j] = 0;
				agrees++;
			}
			else{
				if(  OG[i] < OG[j]   &&   Oi[i] < Oi[j]   ){ //acuerdo en que a es mejor que b
					firstRanking_agrees[i][j] = 1;
					agrees++;
				}
				else{									//no hay acuerdo
					firstRanking_agrees[i][j] = -1;
				}
			}
		}

	}

	firstAgrees = agrees;


}

//ejecuta desde el shell otro programa
int mi_system (const char *comando) {
	/*
    int pid, status;

    if (comando == 0)
        return 1;   // En UNIX/Linux siempre hay un shell
    pid = fork();
    if (pid == -1)
        return -1;
    if (pid == 0) {
        char *argva[4];
        argva[0] = "sh";
        argva[1] = "-c";
        argva[2] = (char *)comando;
        argva[3] = 0;
     //   execve("/bin/sh", argva, environ);
        execv("/bin/sh", argva);
        exit(127);
    }
    do {
        if (waitpid(pid, &status, 0) == -1) {
            if (errno != EINTR)
                return -1;
        } else
            return status;
    } while(1);

    return -100;
    */
	return 1;
}

void ordenar_valores(double *valor, int *O){

	int i, j, posM, Rtemp;
	double mayor, valTemp;

	int *R = (int*) malloc (sizeof(int) * nalternatives);


	for(  i=0;   i < nalternatives;   i++  ){
		R[i]=i;
	}

	for(i=0; i<nalternatives-1; i++){
		mayor = valor[i]; posM=i;

		for(j=i+1; j<nalternatives; j++){
			if( valor[j] > mayor ){
				mayor = valor[j];
				posM=j;
			}

		}

		valTemp = valor[posM];
		valor[posM]= valor[i];
		valor[i]= valTemp;

		O[ R[posM] ] = i;
		Rtemp = R[i];
		R[i] = R[posM];
		R[posM] = Rtemp;


	}

	//El último valor queda en la última posición del arreglo
	O[ R[i] ] = i;

	free(R);





}





