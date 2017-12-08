/*
 ============================================================================
 Name        : Caso 2, pre-ordern total
Infer inter-criteria parameters q, p, v, w
input: pre-orden total
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

# include "global.h"
# include "rand.h"

int nreal;
int nbin;
int nobj;
int ncon;
int popsize;
double pcross_real;
double pcross_bin;
double pmut_real;
double pmut_bin;
double eta_c;
double eta_m;
int ngen;
int nbinmut;
int nrealmut;
int nbincross;
int nrealcross;
int *nbits;
double *min_realvar;
double *max_realvar;
double *min_binvar;
double *max_binvar;
int bitlength;
int choice;
int obj1;
int obj2;
int obj3;
int angle1;
int angle2;
// variables para market share parametros iniciales de frecuencia y pesos
int **frecuency;
double *marketshare;
double *b;

//Added variables
//----------------------------
int use_alfa;		//we say the alpha value is not used
int num_intercriterion_elements;	//;[7 if w, alpha and beta for each q,p,v]
int nalternatives;
int ncriteria;
int genActual;
int use_veto;
int firstAgrees;
int numComparaciones;
int tot_u_muting;
int total_best_pop; //numero de individuos diferentes incluidos en la mejor población
int best_ind;
int best_fitness;
int best_gen;
//caso 2
int *total_preOrder;

char str_nalternatives[33];
char archivo_VOR[300];

double lambda;
double rango_epsilon;	//rango de variación que pueden sufrir los criterios

int *criteria_direccion;  //min:0, max:1
int *criteria_order;   //List of criteria in preference order
int *criterion_position_array;
int *orderG, *orderDM, *orderInd;
int **firstRanking_agrees;
int *elements_criThreshold;
int *w_muting;
int *u_muting;  //tiene tot_u_muting elementos

double **performance_matrix;
double *intercriteria_parameters;
double **sigma_matrix;
double **criteria_threshold;
double *highest_performance;
double *minimum_performance;
double *threshold_epsilon;


FILE *f_sigma; //Apuntador para el archivo de sigma de salida


population *best_pop;

//----------------------------

int main (int argc, char **argv)
{
    int i;
    FILE *fpt1;
    FILE *fpt2;
    FILE *fpt3;
    FILE *fpt4;
    FILE *fpt5;
    FILE *gp;

    clock_t start, stop;
    start = clock();


    population *parent_pop;
    population *child_pop;
    population *mixed_pop;



    //initialize variables - intercriteria
    use_alfa=0;
    use_veto=0;
    num_intercriterion_elements=6; //w,x,y,z,t,lamda
    nalternatives=0;
    ncriteria=0;
    lambda = 0;
    genActual=0;
    firstAgrees=0;
    numComparaciones=0;
    total_best_pop=0;

    rango_epsilon = 0.5	;
    best_fitness = 1000;
    best_ind=-1;
    best_gen=-1;

    //system("pwd");

    /*
    printf("\n%s", argv[1]);
    printf("\n%s\n", argv[2]);
    exit(1);
    */


    if (argc<2)
    {
        printf("\n Usage ./nsga2r random_seed directoryPath\n");
        exit(1);
    }
    seed = (double)atof(argv[1]);
    printf("seed=%lf", seed);
    if (seed<=0.0 || seed>=1.0)
    {
        printf("\n Entered seed value is wrong, seed value must be in (0,1) \n");
        exit(1);
    }


    if( argv[2] == NULL)
    {
        printf("\n There is no path for read the problem files \n");
        exit(1);
    }



    //Creación de los archivos de la población
    //----------------------------------------------------
    fpt1 = fopen("initial_pop.out","w");
    fpt2 = fopen("final_pop.out","w");
    fpt3 = fopen("best_pop.out","w");
    fpt4 = fopen("all_pop.out","w");
    fpt5 = fopen("params.out","w");

    fprintf(fpt1,"# This file contains the data of initial population\n");
    fprintf(fpt2,"# This file contains the data of final population\n");
    fprintf(fpt3,"# This file contains the data of final feasible population (if found)\n");
    fprintf(fpt4,"# This file contains the data of all generations\n");
    fprintf(fpt5,"# This file contains information about inputs as read by the program\n");
    //gp: es el archivo que contiene los parametros para plotear en GNU-PLOT
    //----------------------------------------------------

    //Lectura de parámetros para el NSGA-II
    //----------------------------------------------------

    printf("\n Enter the problem relevant and algorithm relevant parameters ... ");
    printf("\n Enter the population size (a multiple of 4) : ");
    scanf("%d",&popsize);
    if (popsize<4 || (popsize%4)!= 0)
    {
        printf("\n population size read is : %d",popsize);
        printf("\n Wrong population size entered, hence exiting \n");
        exit (1);
    }
    printf("\n Enter the number of generations : ");
    scanf("%d",&ngen);
    if (ngen<1)
    {
        printf("\n number of generations read is : %d",ngen);
        printf("\n Wrong nuber of generations entered, hence exiting \n");
        exit (1);
    }
    printf("\n Enter the number of objectives : ");
    scanf("%d",&nobj);
    if (nobj<1)
    {
        printf("\n number of objectives entered is : %d",nobj);
        printf("\n Wrong number of objectives entered, hence exiting \n");
        exit (1);
    }
    printf("\n Enter the number of constraints : ");
    scanf("%d",&ncon);
    if (ncon<0)
    {
        printf("\n number of constraints entered is : %d",ncon);
        printf("\n Wrong number of constraints entered, hence exiting \n");
        exit (1);
    }

    //Creación de variables reales
    //----------------------------------------------------

    printf("\n Enter the number of real variables : ");
    scanf("%d",&nreal);
    if (nreal<0)
    {
        printf("\n number of real variables entered is : %d",nreal);
        printf("\n Wrong number of variables entered, hence exiting \n");
        exit (1);
    }
    if (nreal != 0)
    {
        //Asignación de memoria para los arreglos que contienen los valores mínimos y máximos de las variables reales
        //----------------------------------------------------
        min_realvar = (double *)malloc(nreal*sizeof(double));
        max_realvar = (double *)malloc(nreal*sizeof(double));
        for (i=0; i<nreal; i++)
        {
        	/*
        	 //Quitamos la lectura de los valores max y min de cada uno de las variables
            printf ("\n Enter the lower limit of real variable %d : ",i+1);
            scanf ("%lf",&min_realvar[i]);
            printf ("\n Enter the upper limit of real variable %d : ",i+1);
            scanf ("%lf",&max_realvar[i]);
            */

        	max_realvar[i] =0;
        	min_realvar[i] =0;
            if (max_realvar[i] < min_realvar[i])
            {
                printf("\n Wrong limits entered for the min and max bounds of real variable %d, hence exiting \n", i+1);
                exit(1);
            }
        }
        //----------------------------------------------------

        printf ("\n Enter the probability of crossover of real variable (0.6-1.0) : ");
        scanf ("%lf",&pcross_real);
        if (pcross_real<0.0 || pcross_real>1.0)
        {
            printf("\n Probability of crossover entered is : %e",pcross_real);
            printf("\n Entered value of probability of crossover of real variables is out of bounds, hence exiting \n");
            exit (1);
        }
        printf ("\n Enter the probability of mutation of real variables (1/nreal) : ");
        scanf ("%lf",&pmut_real);
        if (pmut_real<0.0 || pmut_real>1.0)
        {
            printf("\n Probability of mutation entered is : %e",pmut_real);
            printf("\n Entered value of probability of mutation of real variables is out of bounds, hence exiting \n");
            exit (1);
        }
        printf ("\n Enter the value of distribution index for crossover (5-20): ");
        scanf ("%lf",&eta_c);
        if (eta_c<=0)
        {
            printf("\n The value entered is : %e",eta_c);
            printf("\n Wrong value of distribution index for crossover entered, hence exiting \n");
            exit (1);
        }
        printf ("\n Enter the value of distribution index for mutation (5-50): ");
        scanf ("%lf",&eta_m);
        if (eta_m<=0)
        {
            printf("\n The value entered is : %e",eta_m);
            printf("\n Wrong value of distribution index for mutation entered, hence exiting \n");
            exit (1);
        }
    }
    printf("\n Enter the number of binary variables : ");
    scanf("%d",&nbin);
    if (nbin<0)
    {
        printf ("\n number of binary variables entered is : %d",nbin);
        printf ("\n Wrong number of binary variables entered, hence exiting \n");
        exit(1);
    }
    if (nbin != 0)
    {
        nbits = (int *)malloc(nbin*sizeof(int));
        min_binvar = (double *)malloc(nbin*sizeof(double));
        max_binvar = (double *)malloc(nbin*sizeof(double));
        for (i=0; i<nbin; i++)
        {
            printf ("\n Enter the number of bits for binary variable %d : ",i+1);
            scanf ("%d",&nbits[i]);
            if (nbits[i] < 1)
            {
                printf("\n Wrong number of bits for binary variable entered, hence exiting");
                exit(1);
            }
            printf ("\n Enter the lower limit of binary variable %d : ",i+1);
            scanf ("%lf",&min_binvar[i]);
            printf ("\n Enter the upper limit of binary variable %d : ",i+1);
            scanf ("%lf",&max_binvar[i]);
            if (max_binvar[i] <= min_binvar[i])
            {
                printf("\n Wrong limits entered for the min and max bounds of binary variable entered, hence exiting \n");
                exit(1);
            }
        }
        printf ("\n Enter the probability of crossover of binary variable (0.6-1.0): ");
        scanf ("%lf",&pcross_bin);
        if (pcross_bin<0.0 || pcross_bin>1.0)
        {
            printf("\n Probability of crossover entered is : %e",pcross_bin);
            printf("\n Entered value of probability of crossover of binary variables is out of bounds, hence exiting \n");
            exit (1);
        }
        printf ("\n Enter the probability of mutation of binary variables (1/nbits): ");
        scanf ("%lf",&pmut_bin);
        if (pmut_bin<0.0 || pmut_bin>1.0)
        {
            printf("\n Probability of mutation entered is : %e",pmut_bin);
            printf("\n Entered value of probability  of mutation of binary variables is out of bounds, hence exiting \n");
            exit (1);
        }
    }
    if (nreal==0 && nbin==0)
    {
        printf("\n Number of real as well as binary variables, both are zero, hence exiting \n");
        exit(1);
    }
    choice=0;
    printf("\n Do you want to use gnuplot to display the results realtime (0 for NO) (1 for yes) : ");
    scanf("%d",&choice);
    if (choice!=0 && choice!=1)
    {
        printf("\n Entered the wrong choice, hence exiting, choice entered was %d\n",choice);
        exit(1);
    }
    if (choice==1)
    {
        //Abre el flujo para el programa gnuplot - La definición está en global.h
        //----------------------------------------------------
        gp = popen(GNUPLOT_COMMAND,"w");
        if (gp==NULL)
        {
            printf("\n Could not open a pipe to gnuplot, check the definition of GNUPLOT_COMMAND in file global.h\n");
            printf("\n Edit the string to suit your system configuration and rerun the program\n");
            exit(1);
        }
        //----------------------------------------------------

        if (nobj==2)
        {
            printf("\n Enter the objective for X axis display : ");
            scanf("%d",&obj1);
            if (obj1<1 || obj1>nobj)
            {
                printf("\n Wrong value of X objective entered, value entered was %d\n",obj1);
                exit(1);
            }
            printf("\n Enter the objective for Y axis display : ");
            scanf("%d",&obj2);
            if (obj2<1 || obj2>nobj)
            {
                printf("\n Wrong value of Y objective entered, value entered was %d\n",obj2);
                exit(1);
            }
            obj3 = -1;
        }
        else
        {
            printf("\n #obj > 2, 2D display or a 3D display ?, enter 2 for 2D and 3 for 3D :");
            scanf("%d",&choice);
            if (choice!=2 && choice!=3)
            {
                printf("\n Entered the wrong choice, hence exiting, choice entered was %d\n",choice);
                exit(1);
            }
            if (choice==2)
            {
                printf("\n Enter the objective for X axis display : ");
                scanf("%d",&obj1);
                if (obj1<1 || obj1>nobj)
                {
                    printf("\n Wrong value of X objective entered, value entered was %d\n",obj1);
                    exit(1);
                }
                printf("\n Enter the objective for Y axis display : ");
                scanf("%d",&obj2);
                if (obj2<1 || obj2>nobj)
                {
                    printf("\n Wrong value of Y objective entered, value entered was %d\n",obj2);
                    exit(1);
                }
                obj3 = -1;
            }
            else
            {
                printf("\n Enter the objective for X axis display : ");
                scanf("%d",&obj1);
                if (obj1<1 || obj1>nobj)
                {
                    printf("\n Wrong value of X objective entered, value entered was %d\n",obj1);
                    exit(1);
                }
                printf("\n Enter the objective for Y axis display : ");
                scanf("%d",&obj2);
                if (obj2<1 || obj2>nobj)
                {
                    printf("\n Wrong value of Y objective entered, value entered was %d\n",obj2);
                    exit(1);
                }
                printf("\n Enter the objective for Z axis display : ");
                scanf("%d",&obj3);
                if (obj3<1 || obj3>nobj)
                {
                    printf("\n Wrong value of Z objective entered, value entered was %d\n",obj3);
                    exit(1);
                }
                printf("\n You have chosen 3D display, hence location of eye required \n");
                printf("\n Enter the first angle (an integer in the range 0-180) (if not known, enter 60) :");
                scanf("%d",&angle1);
                if (angle1<0 || angle1>180)
                {
                    printf("\n Wrong value for first angle entered, hence exiting \n");
                    exit(1);
                }
                printf("\n Enter the second angle (an integer in the range 0-360) (if not known, enter 30) :");
                scanf("%d",&angle2);
                if (angle2<0 || angle2>360)
                {
                    printf("\n Wrong value for second angle entered, hence exiting \n");
                    exit(1);
                }
            }
        }
    }
    printf("\n Input data successfully entered, now performing initialization \n");
    fprintf(fpt5,"\n Population size = %d",popsize);
    fprintf(fpt5,"\n Number of generations = %d",ngen);
    fprintf(fpt5,"\n Number of objective functions = %d",nobj);
    fprintf(fpt5,"\n Number of constraints = %d",ncon);
    fprintf(fpt5,"\n Number of real variables = %d",nreal);
    if (nreal!=0)
    {
        for (i=0; i<nreal; i++)
        {
            fprintf(fpt5,"\n Lower limit of real variable %d = %e",i+1,min_realvar[i]);
            fprintf(fpt5,"\n Upper limit of real variable %d = %e",i+1,max_realvar[i]);
        }
        fprintf(fpt5,"\n Probability of crossover of real variable = %e",pcross_real);
        fprintf(fpt5,"\n Probability of mutation of real variable = %e",pmut_real);
        fprintf(fpt5,"\n Distribution index for crossover = %e",eta_c);
        fprintf(fpt5,"\n Distribution index for mutation = %e",eta_m);
    }
    fprintf(fpt5,"\n Number of binary variables = %d",nbin);
    if (nbin!=0)
    {
        for (i=0; i<nbin; i++)
        {
            fprintf(fpt5,"\n Number of bits for binary variable %d = %d",i+1,nbits[i]);
            fprintf(fpt5,"\n Lower limit of binary variable %d = %e",i+1,min_binvar[i]);
            fprintf(fpt5,"\n Upper limit of binary variable %d = %e",i+1,max_binvar[i]);
        }
        fprintf(fpt5,"\n Probability of crossover of binary variable = %e",pcross_bin);
        fprintf(fpt5,"\n Probability of mutation of binary variable = %e",pmut_bin);
    }
    fprintf(fpt5,"\n Seed for random number generator = %e",seed);
    bitlength = 0;
    if (nbin!=0)
    {
        for (i=0; i<nbin; i++)
        {
            bitlength += nbits[i];
        }
    }
    fprintf(fpt1,"# of objectives = %d, # of constraints = %d, # of real_var = %d, # of bits of bin_var = %d, constr_violation, rank, crowding_distance\n",nobj,ncon,nreal,bitlength);
    fprintf(fpt2,"# of objectives = %d, # of constraints = %d, # of real_var = %d, # of bits of bin_var = %d, constr_violation, rank, crowding_distance\n",nobj,ncon,nreal,bitlength);
    fprintf(fpt3,"# of objectives = %d, # of constraints = %d, # of real_var = %d, # of bits of bin_var = %d, constr_violation, rank, crowding_distance\n",nobj,ncon,nreal,bitlength);
    fprintf(fpt4,"# of objectives = %d, # of constraints = %d, # of real_var = %d, # of bits of bin_var = %d, constr_violation, rank, crowding_distance\n",nobj,ncon,nreal,bitlength);
    nbinmut = 0;
    nrealmut = 0;
    nbincross = 0;
    nrealcross = 0;

    //Asignación de memoria para población
    //----------------------------------------------------

    //Cuatro tipos de población
    parent_pop = (population *)malloc(sizeof(population));
    child_pop = (population *)malloc(sizeof(population));
    mixed_pop = (population *)malloc(sizeof(population));
    best_pop =  (population *)malloc(sizeof(population));

    //se le asigna memoria a cada individuo en la población
    allocate_memory_pop (parent_pop, popsize);
    allocate_memory_pop (child_pop, popsize);
    allocate_memory_pop (mixed_pop, 2*popsize);
    allocate_memory_pop (best_pop, popsize);
    randomize();
    //----------------------------------------------------



    //Archivo de salida -> Relación de outranking valuada (VOR o sigma)
    //----------------------------------------------------
    //char archivo_VOR[300];
    strncpy(  archivo_VOR,   argv[2], sizeof(archivo_VOR)  );
    strncat(  archivo_VOR, "VOR.out", 50);
    //f_sigma = fopen(archivo_VOR,"w"); //VOR (sigma) resultante del mejor individuo

    //printf("\nSalida de VOR:\n%s\n", archivo_VOR);


    strcpy(str_nalternatives, "");

    //memoria para datos del problema
    //----------------------------------------------------
    read_size(argv[2]); //number of nalternatives, ncriteria

    //printf("string nalternativas: %s", str_nalternatives);


    for(i=nalternatives-1; i>0; i--) numComparaciones += i;
    //printf("\nrango_epsilon: %g ", rango_epsilon);
    //printf("\nlambda: %g ", lambda);
    //printf("\nnumComparaciones: %d ", numComparaciones);


    //printf("\nnalternatives: %d; ncriteria: %d\n",nalternatives, ncriteria);
    performance_matrix = allocate_memory_matrix2 ( nalternatives, ncriteria);
    intercriteria_parameters = allocate_memory_intercriteria(  nreal  );
    sigma_matrix = allocate_memory_matrix2 ( nalternatives, nalternatives);
    orderG = (int *)   malloc (   nalternatives * sizeof(int)   );
    orderDM = (int *)   malloc (   nalternatives * sizeof(int)   );
    orderInd = (int *)   malloc (   nalternatives * sizeof(int)   );
    criteria_direccion= (int*) malloc (sizeof(int) * ncriteria); //min:0, max:1
    criteria_order= (int*) malloc (sizeof(int) * ncriteria);
    criterion_position_array = (int*) malloc (sizeof(int) * ncriteria);
    //criteria_threshold = (double**) malloc ( sizeof(double) * (ncriteria * 3) );
    elements_criThreshold = (int*) malloc (  sizeof(int) * (ncriteria * 3)  );
    highest_performance = (double*) malloc (  sizeof(double) * ncriteria  );
    minimum_performance = (double*) malloc (  sizeof(double) * ncriteria  );
    threshold_epsilon = (double*) malloc (  sizeof(double) * ncriteria  );
    //w_muting = (double*) malloc (  sizeof(double) * ncriteria  );
    w_muting = (int*) malloc (  sizeof(int) * ncriteria  );

    //variables para calcular sigma
	w = (double*) malloc (sizeof(double) * ncriteria);
	q= (double*) malloc (sizeof(double) * ncriteria);
	p= (double*) malloc (sizeof(double) * ncriteria);
	v= (double*) malloc (sizeof(double) * ncriteria);
	t= (double*) malloc (sizeof(double) * ncriteria);

	//Caso 2
	//--------------------------------------------------------------------------------------
	total_preOrder = (int *) malloc(   nalternatives * sizeof(int)   );


	frecuency= int_allocate_memory_matrix ( nalternatives, nalternatives);
	marketshare=(double *)   malloc (   nalternatives * sizeof(double)   );
	b=(double *)   malloc (   nalternatives * sizeof(double)   );



	//old
    //firstRanking_agrees =  int_allocate_memory_matrix ( nalternatives, nalternatives);

    read_data_parameters(argv[2]);

	printf("\n");
	int j;
	     for(i=0; i<nalternatives; i++){
	     	for(j=0; j<nalternatives; j++) {

	     		printf("%d\t",frecuency[i][j]);
	     	}
	     	printf("\n");
	     }

	if(use_veto) tot_u_muting = ncriteria*3;
	else tot_u_muting = ncriteria*2;
	//u_muting = (double*) malloc (  sizeof(double) * tot_u_muting  ); //incluye q y p
	u_muting = (int*) malloc (  sizeof(int) * tot_u_muting  ); //incluye q y p

	//read_ordering_ind("ranking-sadage/ranking.out"); //prueba para lectura de ordenamiento del individuo

	//imprime market share real del individuo
	for(j=0; j<nalternatives; j++) {
		printf("%lf, ", marketshare[j] );

	}
	printf("\n");


    /*
     //old
    firstRanking_agress_matrix(firstRanking_agrees, orderDM, orderG);
    printf("\nFirst order agrees: %d\n", firstAgrees);
    printf("Desacuerdos: %d \n", numComparaciones-firstAgrees);
    */



	//find the highest performance per criteria to know the scale of the criteria
	//create an array of ncriteria size; threshold_epsilon[j] = performance * rango_epsilon
    find_highest_performance_criterion();


    initialize_pop (parent_pop);


    //Mutación
    //-----------------------------------------------------------------
    //mutation_pop (parent_pop,0); //todos mutan para conformar la población inicial




/*
    //imprime ordenamiento del individuo
 	for(j=0; j<nalternatives; j++) {
 		printf("%d ", orderInd[j] );
 	}


 //old
    //imprime acuerdos Oi, OG
    printf("\nAcuerdos entre ordenamientos\n1:Acuerdo que a>b; 0:Acuerdo que b>a; -1: No hay acuerdo" );

    printf("\n");
     for(i=0; i<nalternatives; i++){
     	for(j=0; j<nalternatives; j++) {
     		if(j<=i) printf("-\t");
     		else printf("%d\t",firstRanking_agrees[i][j]);
     	}
     	printf("\n");
     }



    printf( "\nOrdenamiento colectivo OG\n" );
    for(i=0; i<nalternatives; i++){
    		printf( "%d, ", orderG[i]  );
    }

    printf( "\nOrdenamiento individual Oi\n" );
    for(i=0; i<nalternatives; i++){
    		printf( "%d, ", orderDM[i]  );
    }

*/

    /*
    //imprime performance e intercriterios
    printf( "\n\nDesempeños de alternativas\n" );
    //int j;
     for(i=0; i<nalternatives; i++){
     	for(j=0; j<ncriteria; j++) {
     		printf("%lf, ",performance_matrix[i][j]);
     	}
     	printf("\n");
     }


     printf( "\nParámetros intercriterios" );
  	for(j=0; j < nreal; j++) {
  		if(j % num_intercriterion_elements == 0) printf("\n");
  		printf("%f, ",intercriteria_parameters[j]);
  	}
  	printf("\n");
  	*/


//    system("pwd");
//    exit(0);

    //printf("Alternatives: %d\nCriteria: %d\n",nalternatives, ncriteria);



    printf("\n Initialization done, now performing first generation\n");
    genActual=1;
    decode_pop(parent_pop);
    evaluate_pop (parent_pop);

    assign_rank_and_crowding_distance (parent_pop);
    report_pop (parent_pop, fpt1);





    fprintf(fpt4,"# gen = 1\n");
    report_pop(parent_pop,fpt4);
    //printf("\n gen = 1");
    fflush(stdout);


    //grafica
    //if (choice!=0)    onthefly_display (parent_pop,gp,1);

    fflush(fpt1);
    fflush(fpt2);
    fflush(fpt3);
    fflush(fpt4);
    fflush(fpt5);
    fflush(f_sigma);


    //sleep(1);
    for (i=2; i<=ngen; i++)
    {
    	genActual++;



    	//printf("\n--------------\ngen = %d",i);


    	//if(i==ngen-1) printf("Begining selection\n");
        selection (parent_pop, child_pop);

        //if(i==ngen-1) printf("Begining mutation_pop\n");
        mutation_pop (child_pop, 0);
        //if(i==ngen-1) printf("Begining decode_pop\n");
        decode_pop(child_pop);


        //if(i==ngen-1) printf("Begining evaluate_pop\n");
        evaluate_pop(child_pop);
        //if(i==ngen-1) printf("Begining merge\n");
        merge (parent_pop, child_pop, mixed_pop);
        //if(i==ngen-1) printf("Begining mono_fill_nondominated_sort\n");
        mono_fill_nondominated_sort (mixed_pop, parent_pop);

        //fill_nondominated_sort (mixed_pop, parent_pop);

        /* Comment following four lines if information for all
        generations is not desired, it will speed up the execution */
        /*
        fprintf(fpt4,"# gen = %d\n",i);
        report_pop(parent_pop,fpt4);
        fflush(fpt4);
        */

        //grafica
        //if (choice!=0)    onthefly_display (parent_pop,gp,i);

        //sleep(1);


    }

    //printf("\n gen = %d",i-1);
    printf("\n Generations finished, now reporting solutions");
    report_pop(parent_pop,fpt2);
    report_feasible(parent_pop,fpt3);

    if (nreal!=0)
    {
        fprintf(fpt5,"\n Number of crossover of real variable = %d",nrealcross);
        fprintf(fpt5,"\n Number of mutation of real variable = %d",nrealmut);
    }
    if (nbin!=0)
    {
        fprintf(fpt5,"\n Number of crossover of binary variable = %d",nbincross);
        fprintf(fpt5,"\n Number of mutation of binary variable = %d",nbinmut);
    }

    fflush(stdout);
    fflush(fpt1);
    fflush(fpt2);
    fflush(fpt3);
    fflush(fpt4);
    fflush(fpt5);
    fflush(f_sigma);

    fclose(fpt1);
    fclose(fpt2);
    fclose(fpt3);
    fclose(fpt4);
    fclose(fpt5);
    fclose(f_sigma);

    if (choice!=0)
    {
        pclose(gp);
    }
    if (nreal!=0)
    {
        free (min_realvar);
        free (max_realvar);
    }
    if (nbin!=0)
    {
        free (min_binvar);
        free (max_binvar);
        free (nbits);
    }


    deallocate_memory_pop (parent_pop, popsize);
    deallocate_memory_pop (child_pop, popsize);
    deallocate_memory_pop (mixed_pop, 2*popsize);

    //----------------------------------------------------------------------
    deallocate_memory_matrix(performance_matrix, nalternatives, ncriteria);
    deallocate_memory_matrix(frecuency, nalternatives, nalternatives);
    free(marketshare);
    free(b);



    deallocate_memory_intercriteria(intercriteria_parameters);
    deallocate_memory_matrix (sigma_matrix, nalternatives, nalternatives);
    free(orderG);
    free(orderDM);
    free(orderInd);
    //int_deallocate_memory_matrix(firstRanking_agrees, nalternatives, nalternatives);
    free(highest_performance);
    free(threshold_epsilon);
    free(w_muting);
    free(u_muting);
    free(elements_criThreshold);
    free(minimum_performance);


    free(criteria_order);
    free(criteria_direccion);
    free(criterion_position_array);

    free(total_preOrder);


    //sigma
    free(w);
    free(q);
    free(p);
    free(v);
    free(t);

    //----------------------------------------------------------------------

    free (parent_pop);
    free (child_pop);
    free (mixed_pop);


    stop = clock();
    //printf("%6.3lu", start);
    //printf("\n\n%6.3lu", stop);
    double tiempo = (double)(stop) / CLOCKS_PER_SEC * 10;
    tiempo = tiempo / 60; //minutos
    //tiempo = tiempo / 60; //horas

    printf("\n Routine successfully exited \n");

    /*
    printf("\n Best_ind (Obj1): %d", best_fitness);
    printf("\n Generation Num: %d", best_gen);
    printf("\n Individual_i: %d", best_ind);

    printf("\nTotal time taken by CPU: %f minutes\n", tiempo );
	*/

    return (0);
}
