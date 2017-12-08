/* Mutation routines */

# include <stdio.h>
# include <stdlib.h>
# include <math.h>

# include "global.h"
# include "rand.h"

/* Function to perform mutation in a population */
void mutation_pop (population *pop, int popInicial)
{
    int i;
    //double muta;
    for (i=0; i<popsize; i++)
    {
    	/*
    	if( popInicial ) muta = -1;  //muta todos para la población inicial
    	else    	muta = rndreal(0,1);	//muta los que sean menor al indice de mutación
    	*/

    	mutation_ind(&(pop->ind[i]));

    	/*
    	if(muta < pmut_real)
    		mutation_ind(&(pop->ind[i]));

    	*/

    	/*
    	if(i==popsize-2)
    		muta=0;
    	 */

    	//printf("\nmuta: %g, pmut_real: %g", muta, pmut_real);


    }

    return;
}

void mutation_pop_ini (population *pop)
{
    int i;
    int inicio=9;
    int numParamMuting=inicio;
    int totParam = tot_u_muting + ncriteria;

    for (i=0; i<popsize; i++)
    {
    	if(numParamMuting==totParam) numParamMuting = inicio;
    	numParamMuting = numParamMuting + 1;

    	//printf("\n%d: %d, ",i, numParamMuting);

    	real_mutate_ind_InitialPop (&(pop->ind[i]), numParamMuting);


/*    	printf("\n");
        for (int j=0; j<nreal; j++)
        {

           	if(j % 7 == 0)  //imprime w
            {
           		printf("%0.3g\t",pop->ind[i].xreal[j]);
           		printf("%g\t",pop->ind[i].xreal[j+2]);
           		printf("%g\t",pop->ind[i].xreal[j+4]);
           		j += 6;
            }

        }*/


    }
    return;
}


/* Function to perform mutation of an individual */
void mutation_ind (individual *ind)
{
    if (nreal!=0)
    {
        //real_mutate_ind(ind);
    	real_mutate_ind_param(ind);
    }
    if (nbin!=0)
    {
        bin_mutate_ind(ind);
    }
    return;
}

/* Routine for binary mutation of an individual */
void bin_mutate_ind (individual *ind)
{
    int j, k;
    double prob;
    for (j=0; j<nbin; j++)
    {
        for (k=0; k<nbits[j]; k++)
        {
            prob = randomperc();
            if (prob <=pmut_bin)
            {
                if (ind->gene[j][k] == 0)
                {
                    ind->gene[j][k] = 1;
                }
                else
                {
                    ind->gene[j][k] = 0;
                }
                nbinmut+=1;
            }
        }
    }
    return;
}

/* Routine for real polynomial mutation of an individual */
void real_mutate_ind (individual *ind)
{

    int j;
    double rnd, delta1, delta2, mut_pow, deltaq;
    double y, yl, yu, val, xy;
    for (j=0; j<nreal; j++)
    {
        if (randomperc() <= pmut_real)
        {
            y = ind->xreal[j];
            yl = min_realvar[j];
            yu = max_realvar[j];
            delta1 = (y-yl)/(yu-yl);
            delta2 = (yu-y)/(yu-yl);
            rnd = randomperc();
            mut_pow = 1.0/(eta_m+1.0);
            if (rnd <= 0.5)
            {
                xy = 1.0-delta1;
                val = 2.0*rnd+(1.0-2.0*rnd)*(pow(xy,(eta_m+1.0)));
                deltaq =  pow(val,mut_pow) - 1.0;
            }
            else
            {
                xy = 1.0-delta2;
                val = 2.0*(1.0-rnd)+2.0*(rnd-0.5)*(pow(xy,(eta_m+1.0)));
                deltaq = 1.0 - (pow(val,mut_pow));
            }
            y = y + deltaq*(yu-yl);
            if (y<yl)
                y = yl;
            if (y>yu)
                y = yu;
            ind->xreal[j] = y;
            nrealmut+=1;
        }
    }
    return;
}


/* Rutina para mutación de parametros */
void real_mutate_ind_param (individual *ind)
{

	int tipoParam, variableMutar;
    //punto = rnd(0, nreal-1);

	int mutaPesos, mutaVD, mutat, mutaLambda;
	//int epsilon_w;
	double epsilon_w, epsilon_VD, epsilon_lambda;

	epsilon_w = 0.1; //[0, 1]
	epsilon_VD = 0.3; //[0, 1]
	epsilon_lambda = 0.2; //[0, 1]

	mutaPesos = rndreal(0,1);
	mutaVD = rndreal(0,1);
	mutat = rndreal(0,1);
	mutaLambda = rndreal(0,1);


	//------------------------------------
	//Nota: corregir esto
	//no esta mutando pesos para probar
	//------------------------------------

	tipoParam = rnd(0,2); //0 -> w;  1 y 2 -> x,y,z,t,lambda

	if(mutaPesos < pmut_real) //w
	{

		//Mutar pesos
		//considera el ordenamiento de los criterios del experto
		//Este método se implemento para conservar un orden en los pesos de los criterios
		//------------------------------------------------------------------------------------

		int criMutar, posCri, posCriMayor, posCriMenor;
		int dif, precision, tipoModif, valor, valMayor, valMenor;
		double newW, cambio;

		precision =1000;


		criMutar = rnd(1, ncriteria-2);
		posCri = criterion_position_array[criMutar];

		tipoModif = rnd(0,1); //0: reducir, 1 aumentar

		if(tipoModif == 0){ //reducir

			//posicion del criterio inferior
			posCriMenor = criterion_position_array[criMutar+1];
			valMayor = (int) roundf(  ind->xreal[   posCri   ]* precision  );
			valMenor = (int) roundf(  ind->xreal[   posCriMenor   ]*precision  );
			//dif= ((int) ind->xreal[   posCri   ]*precision - (int)ind->xreal[   posCriMenor   ]*precision ) / 2  - 1;
			dif= (valMayor - valMenor) / 2  - 1;

			//if(dif > epsilon_w) dif= epsilon_w;
			dif = dif * epsilon_w;

			if(dif>1){
				valor = rnd(1, dif);
				cambio = (double) valor / (double) precision;

				newW = ind->xreal[   posCri   ] - cambio;
				ind->xreal[   posCri   ] = newW;

				newW = ind->xreal[   posCriMenor   ] + cambio;
				ind->xreal[   posCriMenor   ] = newW;
			}


		}
		else{ //aumenta

			//posicion del criterio mayor
			posCriMayor = criterion_position_array[criMutar-1];

			valMayor = (int) roundf(  ind->xreal[   posCriMayor   ]* precision  );
			newW = ind->xreal[   posCriMayor   ];

			valMenor = (int) roundf(  ind->xreal[   posCri   ]*precision  );
			newW = ind->xreal[   posCri   ];


			//dif= ((int) ind->xreal[   posCriMayor   ]*precision - (int)ind->xreal[   posCri   ]*precision ) / 2  - 1;
			dif= (valMayor - valMenor) / 2  - 1;
			//if(dif > epsilon_w) dif= epsilon_w;
			dif = dif * epsilon_w;

			if(dif>1 && dif<precision){
				valor = rnd(1, dif);
				cambio = (double) valor / (double) precision;

				newW = ind->xreal[   posCriMayor   ] - cambio;
				ind->xreal[   posCriMayor   ] = newW;

				newW = ind->xreal[   posCri   ] + cambio;
				ind->xreal[   posCri   ] = newW;
			}

		}


	    double W=0.0;
	    posCriMayor = criterion_position_array[0]-1;

	    int j;
	    for (j=0; j<ncriteria; j++){
	    	W += ind->xreal[j];
	    }

	    if(W > 1.0){
	    	ind->xreal[posCriMayor] = ind->xreal[posCriMayor] - (W-1.0);
	    }
	    if(W < 1.0){
	    	ind->xreal[posCriMayor] = ind->xreal[posCriMayor] + (1.0-W);
	    }






		/*
		//Método de mutar pesos
		int i, random, temp;
		int nW_mutar;	//número de pesos a mutar
		double maximo_mutar;	//el valor máximo permito para mutar entre los pesos seleccionados
		double valorMutado;
		double newW, totalMutado;


		nW_mutar = rnd(2,ncriteria);

		//ordenamos los criterios de 0 a ncriteria-1
		for(i=0;  i<ncriteria;  i++)	w_muting[i]=i;

		//seleccionamos aleatoriamente los criterios a mutar
		//Es un orden aleatorio del arreglo, los primero nW_mutar pesos mutarán
		for(i=0;  i < nW_mutar;  i++){
			random = rnd(i, ncriteria-1);
			temp = w_muting[random];
			w_muting[ random ] = w_muting[ i ];
			w_muting[ i ] = temp;
		}

		//Sumamos el valor de los pesos que mutarán para saber el valor máximo que podemos utilizar
		maximo_mutar = 0;
		for(i=0;  i < nW_mutar;  i++){
			maximo_mutar += ind->xreal[ w_muting[i] ];
		}

		//printf("\n");
		totalMutado=0;
		for(i=0;  i < nW_mutar;  i++){

			if( i == nW_mutar-1 ) //si es el último peso a mutar
				valorMutado = maximo_mutar; //se le asigna el restante
			else
				valorMutado = rndreal(  0, maximo_mutar  ); //se asigna aleatorio

			newW =  valorMutado;
			ind->xreal[ w_muting[i] ] = newW;

			//printf(  "\n%g\t%g",  maximo_mutar, valorMutado );

			maximo_mutar -= valorMutado;
			totalMutado += valorMutado;

		}
		//printf("\nTotal mutado: %g", totalMutado);

		*/


	}

	//else{ //x,y,z,t,lambda
	//Variables de decision
	//----------------------------------------



	int j, criMutar, violation;
	int pos_x, pos_y, pos_z, pos_t;
	double max_valor_Pj, qj, pj, vj, x,y,z;
	double max_VD, lambda_cambio, lambda_max;


	criMutar = rnd (0,  ncriteria-1);
	variableMutar = rnd(1, 3); //x, y, z

	j= criMutar;

	pos_x = ncriteria * 1 +j;
	pos_y = ncriteria * 2 +j;
	pos_z = ncriteria * 3 +j;

    if(mutaVD < pmut_real)//x, y, z
    {


		max_valor_Pj = highest_performance[j] - minimum_performance[j];
		violation = -1;

		while(violation != 0)
		{
				violation=0;


			if( variableMutar == 1) // x
			{
				max_VD = max_valor_Pj * epsilon_VD;
				ind->xreal[pos_x] = rndreal (  0,  max_VD  );
				//ind->xreal[pos_x] = rndreal (  0,  max_valor_Pj  );

			}

			if( variableMutar == 2) // y
			{
				max_VD = ind->xreal[ pos_z ] * epsilon_VD;
				ind->xreal[ pos_y ] = rndreal (0, max_VD);
				//ind->xreal[ pos_y ] = rndreal (0, ind->xreal[ pos_z ]);


			}
			if( variableMutar == 3) // z
			{
				//generamos zj >= yj
				max_VD = max_valor_Pj * epsilon_VD;
				ind->xreal[ pos_z ] = rndreal (ind->xreal[ pos_y ], max_VD);  //z, z>=y
				//ind->xreal[ pos_z ] = rndreal (ind->xreal[ pos_y ], max_valor_Pj);  //z, z>=y


			}

			//calculamos pj
			pj = ( ind->xreal[ pos_z ] + ind->xreal[ pos_y ] ) / 2 ;
			printf("\npj=%lf <= %lf", pj,max_valor_Pj); exit(1);
			if( pj <=  max_valor_Pj){
				qj = ( ind->xreal[ pos_z ] - ind->xreal[ pos_y ] ) / 2 ;
				vj = ind->xreal[pos_x] + (0.5*ind->xreal[ pos_y ]) + (0.5 * ind->xreal[ pos_z ]) ;




				//printf( "\nC%i: max_valor_Pj:%g\t\t qj:%g \t pj:%g \t vj:%g", j,max_valor_Pj, qj, pj, vj );


			}
			else{
				printf("\nviolación: P > diferencia maxima desempeños");
			}

			x = ind->xreal[ pos_x ];
			y = ind->xreal[ pos_y ];
			z = ind->xreal[ pos_z ];


			if (  (ind->xreal[ pos_y ] + ind->xreal[ pos_z ])  >  (2*max_valor_Pj)   ) violation--;

			if ( ind->xreal[ pos_y ] > ind->xreal[ pos_z ] ) {
				violation--;
				variableMutar=2;
			}

			if( ind->xreal[ pos_y ] < 0 ) violation--;

			if( ind->xreal[ pos_z ] < 0 ) violation--;



			/*
			if(violation < 0)
				printf("\nviolación: %d", violation);
			*/

		}//while

    }


    if(mutat < pmut_real){
    //if(0){//no se considera el veto en esta prueba
		//if( variableMutar == 4) // t
		//{
    		j = rnd (0,  ncriteria-1);
    		pos_t = ncriteria * 4 +j;

			if( ind->xreal[ pos_t ] == 0 )  ind->xreal[ pos_t ]=1;
			else ind->xreal[ pos_t ] = 0;

			if( ind->xreal[ pos_t ] < 0 ) violation--;
			if( ind->xreal[ pos_t ] > 1 ) violation--;

		//}
    }

    if(mutaLambda < pmut_real){
    	//if( variableMutar == 5) // lambda
		//{
    	//ind->xreal[nreal-1] = rndreal (0.6, 0.8);
    	lambda_cambio = rnd(0,1); //0: decrementa, 1: incrementa
    	lambda_max = ind->xreal[nreal-1] * epsilon_lambda;

    	if( lambda_cambio == 1 ){
    		ind->xreal[nreal-1] += rndreal (0, lambda_max);
    		//Se corrige si pasa límites superior
    		if(ind->xreal[nreal-1] > 0.8) ind->xreal[nreal-1] = 0.8;
    	}

    	if( lambda_cambio == 0 ){
    		ind->xreal[nreal-1] -= rndreal (0, lambda_max);
    		//Se corrige si pasa límite inferior
    		if(ind->xreal[nreal-1] < 0.6) ind->xreal[nreal-1] = 0.6;
    	}




		//}
    }






	//}


    return;
}



//poblacion inicial
/* Rutina para mutación de parametros */
void real_mutate_ind_InitialPop (individual *ind, int numParam)
{

    double newVal;
	int tipoParam;
	int paramRestantes = numParam;
	int i, totOrder;
	int random, temp;
	int iniciaCri, posUmuting;
	int wMutados=0;


	//Preparación para Pesos
	//------------------------------------------------------------------

	if(numParam < ncriteria) totOrder = numParam;
	else totOrder = ncriteria;

	//ordenamos los criterios de 0 a ncriteria-1
	for(i=0;  i<ncriteria;  i++)	w_muting[i]=i;

	//seleccionamos aleatoriamente los criterios a mutar
	for(i=0;  i < totOrder;  i++){
		random = rnd(i, ncriteria-1);
		temp = w_muting[random];
		w_muting[ random ] = w_muting[ i ];
		w_muting[ i ] = temp;
	}

	/*
	printf("\nPocisión aleatorea del peso\n");
	for(i=0;  i<totOrder; i++ ){
		printf("%d, ",w_muting[i]);
	}
	printf("\n");
	*/


	//Preparación para los umbrales
	//------------------------------------------------------------------

	if(  numParam < tot_u_muting  ) totOrder = numParam;
	else totOrder = tot_u_muting;

	//ordenamos los umbrales de 0 a tot_u_muting
	for(i=0;  i<ncriteria; i++ ){
		iniciaCri = i * 7;
		if(use_veto) posUmuting= i*3;
		else posUmuting= i*2;

		u_muting[posUmuting]= iniciaCri+2;	//qi
		u_muting[posUmuting+1]= iniciaCri+4;	//pi
		if(use_veto) u_muting[posUmuting+2]= iniciaCri+6;	//vi
	}
	/*
	printf("\nPocisión del umbral\n");
	for(i=0;  i<tot_u_muting; i++ ){
		printf("%d, ",u_muting[i]);
	}
	*/

	//seleccionamos aleatoriamente los umbrales a mutar
	for(i=0;  i < totOrder;  i++){
		random = rnd(i, tot_u_muting-1);
		temp = u_muting[random];
		u_muting[ random ] = u_muting[ i ];
		u_muting[ i ] = temp;
	}

	/*
	printf("\n\nPocisión aleatoria del umbral\n");
	for(i=0;  i<tot_u_muting; i++ ){
		printf("%d, ",u_muting[i]);
	}


	printf("\n");
	*/


	//
	//------------------------------------------------------------------

	while( paramRestantes > 0 )
	{



		tipoParam = rnd(0,1); //w:0, umbral:1

		//tipoParam=2; //eliminar

		if(tipoParam == 0 && wMutados < ncriteria){  // w


			//Nuevo método de mutar pesos
			int posW;
			int nW_mutar;	//número de pesos a mutar
			int iniciaPeso, parada;
			double epsilon_w;	//el valor mínimo o máximo que será añadido al peso principal que muta
			double valorMutado;
			double compensar;  //valor que será compensado en el resto de criterios por la mutación del principal
			double limit_epsilon_Wpos;
			double limit_epsilon_Wneg;


			nW_mutar = rnd(2,ncriteria);

			if(  wMutados+nW_mutar  >  ncriteria   )
			{
				nW_mutar = ncriteria-wMutados;  //la cantidad de pesos que falta mutar
			}

			if(  nW_mutar  ==  1  )
			{
				nW_mutar = 2;
				iniciaPeso = 0; parada=1;
			}
			else{
				iniciaPeso = wMutados+1;
				parada = wMutados + nW_mutar;
			}


			//el elemento de la posición 0 indica el gen principal para mutar
			posW = w_muting[ wMutados ] * num_intercriterion_elements; // 7

			epsilon_w =  intercriteria_parameters[ posW ] * rango_epsilon;

			valorMutado = rndreal(  (epsilon_w * -1), epsilon_w  );


			//mantener los valores de los parametros dentro de una vecindad
			limit_epsilon_Wpos = intercriteria_parameters[ posW ] + epsilon_w;
			limit_epsilon_Wneg = intercriteria_parameters[ posW ] - epsilon_w;

			if( valorMutado > limit_epsilon_Wpos  )
				valorMutado  =   limit_epsilon_Wpos;
			else
				if( valorMutado < limit_epsilon_Wneg  )
					valorMutado  =   limit_epsilon_Wneg;



			//printf("\nMutar: %d, mutados: %d", nW_mutar, wMutados );

			compensar = valorMutado / (double) (nW_mutar-1);

			ind->xreal[ posW ] += valorMutado;
			wMutados = wMutados + 1;
			//printf("\nw%d: %g, ", posW, ind->xreal[ posW ]);


			for(i=iniciaPeso;  i < parada;  i++){
				posW = w_muting[ i ] * num_intercriterion_elements;
				ind->xreal[ posW ] = ind->xreal[ posW ] - compensar;
				//printf("w%d: %g, ", posW, ind->xreal[ posW ]);
			}

			if(iniciaPeso > 0){
				wMutados = wMutados + nW_mutar-1;
				paramRestantes = paramRestantes - nW_mutar;
			}
			else
				paramRestantes = paramRestantes - 1;
			//printf("\nwMutados: %d\n", wMutados);


		}



		//Si son umbrales
		if(tipoParam != 0){

			double epsilon_umb;
			double rndTemp;
			double limit_epsilon_pos;
			double limit_epsilon_neg;

			int nU_mutar;

			if(  tot_u_muting - paramRestantes >= 0) nU_mutar= paramRestantes;
			else nU_mutar = tot_u_muting;

			for(i=0; i<nU_mutar; i++){
				epsilon_umb =  intercriteria_parameters[ u_muting[i] ] * rango_epsilon;
				rndTemp  = rndreal(   ( epsilon_umb * -1 )   ,    epsilon_umb  )   ;

				//en función del valor del individuo
				//newVal = ind->xreal[ u_muting[i] ] + round( rndTemp );
				newVal = ind->xreal[ u_muting[i] ] +  rndTemp ;

				//mantener los valores de los parametros dentro de una vecindad
				limit_epsilon_pos = intercriteria_parameters[ u_muting[i] ] + epsilon_umb;
				limit_epsilon_neg = intercriteria_parameters[ u_muting[i] ] - epsilon_umb;

				if( newVal > limit_epsilon_pos  )
					newVal  =   limit_epsilon_pos;
				else
					if( newVal < limit_epsilon_neg  )
						newVal  =   limit_epsilon_neg;

				//printf("\nparam:%d, OldVal: %g, newVal: %g", u_muting[i], ind->xreal[ u_muting[i] ], newVal);
				ind->xreal[ u_muting[i] ] = newVal;

			}

			//printf("\n");
			paramRestantes = paramRestantes - nU_mutar;


		}


	}//while( paramRestantes > 0 )




    return;
}



int available_mutation_parameter(int criMutar)
{
	int posParam;
	int tipoMuta=-1;
	int maxElement;
	int mutaParam;


	int paramModificable [4][7] = { {0,1,2,3,4,5,6}, {0,1,2,3,4,-1,-1}, {0,2,4,6,-1,-1,-1}, {0,2,4,-1,-1,-1,-1}  };

	if( use_alfa  && use_veto ){
		tipoMuta=0;
		maxElement=7;
	}
	else if( use_alfa  && !use_veto ){
		tipoMuta=1;
		maxElement=5;
	}
	else if( !use_alfa  && use_veto ){
		tipoMuta=2;
		maxElement=4;
	}
	else if( !use_alfa  && !use_veto ){
		tipoMuta=3;
		maxElement=3;
	}

	mutaParam = rnd(0, maxElement-1);


	posParam = criMutar * 7  +  paramModificable[tipoMuta][mutaParam];


	return posParam;
}














