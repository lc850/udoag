/* This file contains the variable and function declarations */

# ifndef _GLOBAL_H_
# define _GLOBAL_H_

# define INF 1.0e14
# define EPS 1.0e-14
# define E  2.71828182845905
# define PI 3.14159265358979
# define GNUPLOT_COMMAND "/usr/local/bin/gnuplot -persist"

typedef struct
{
    int rank;
    double constr_violation;
    double *xreal;
    int **gene;
    double *xbin;
    double *obj;
    double *constr;
    double crowd_dist;
}
individual;

typedef struct
{
    individual *ind;
}
population;

typedef struct lists
{
    int index;
    struct lists *parent;
    struct lists *child;
}
list;

extern int nreal;
extern int nbin;
extern int nobj;
extern int ncon;
extern int popsize;
extern double pcross_real;
extern double pcross_bin;
extern double pmut_real;
extern double pmut_bin;
extern double eta_c;
extern double eta_m;
extern int ngen;
extern int nbinmut;
extern int nrealmut;
extern int nbincross;
extern int nrealcross;
extern int *nbits;
extern double *min_realvar;
extern double *max_realvar;
extern double *min_binvar;
extern double *max_binvar;
extern int bitlength;
extern int choice;
extern int obj1;
extern int obj2;
extern int obj3;
extern int angle1;
extern int angle2;

//Added variables for intercriteria parameters problem
//----------------------------
extern int use_alfa;	//to say if we use or not alpha values
extern int use_veto;  //yes:1, no:0
extern int num_intercriterion_elements; // number of elements to conform a criterion [4 if fixed w,q,p,v][7 if w, alpha and beta for each q,p,v]
extern int nalternatives;	//number of alternatives
extern int ncriteria;	//number of criteria
extern int genActual;	//numero de generación actual
extern int firstAgrees;
extern int numComparaciones;
extern int tot_u_muting;
extern int *criteria_direccion;  //min:0, max:1
extern int *criteria_order;  //List of criteria in preference order
extern int *criterion_position_array;  //List of criteria in preference order
extern int total_best_pop; //numero de individuos diferentes incluidos en la mejor población
extern int best_ind;
extern int best_gen;
extern int best_fitness;
//caso 2
extern int *total_preOrder;

//Added variables for market_share problem
//----------------------------
extern int **frecuency;
extern double *marketshare;
extern double *b;






extern char str_nalternatives[33];
extern char archivo_VOR[300];

extern double lambda;
extern double rango_epsilon;

extern int *orderG, *orderDM, *orderInd;
extern int **firstRanking_agrees;
extern double **criteria_threshold;
extern int *elements_criThreshold;
extern int *w_muting;
extern int *u_muting;

extern double **performance_matrix;
extern double *intercriteria_parameters;
extern double **sigma_matrix;
extern double *highest_performance;
extern double *minimum_performance;
extern double *threshold_epsilon;

extern population *best_pop;


//Sigma
double *w;
double *q;
double *p;
double *v;
double *t;

FILE *f_sigma; //Apuntador para el archivo de sigma de salida

//----------------------------


void allocate_memory_pop (population *pop, int size);
void allocate_memory_ind (individual *ind);
void deallocate_memory_pop (population *pop, int size);
void deallocate_memory_ind (individual *ind);

double maximum (double a, double b);
double minimum (double a, double b);

void crossover (individual *parent1, individual *parent2, individual *child1, individual *child2);
void realcross (individual *parent1, individual *parent2, individual *child1, individual *child2);
void bincross (individual *parent1, individual *parent2, individual *child1, individual *child2);

void assign_crowding_distance_list (population *pop, list *lst, int front_size);
void assign_crowding_distance_indices (population *pop, int c1, int c2);
void assign_crowding_distance (population *pop, int *dist, int **obj_array, int front_size);

void decode_pop (population *pop);
void decode_ind (individual *ind);

void onthefly_display (population *pop, FILE *gp, int ii);

int check_dominance (individual *a, individual *b);

void evaluate_pop (population *pop);
void evaluate_ind (individual *ind);

void fill_nondominated_sort (population *mixed_pop, population *new_pop);
void crowding_fill (population *mixed_pop, population *new_pop, int count, int front_size, list *cur);

void initialize_pop (population *pop);
void initialize_ind (individual *ind);

void insert (list *node, int x);
list* del (list *node);

void merge(population *pop1, population *pop2, population *pop3);
void copy_ind (individual *ind1, individual *ind2);

//void mutation_pop (population *pop);
void mutation_pop (population *pop, int popInicial);
void mutation_ind (individual *ind);
void bin_mutate_ind (individual *ind);
void real_mutate_ind (individual *ind);

void test_problem (double *xreal, double *xbin, int **gene, double *obj, double *constr);

void assign_rank_and_crowding_distance (population *new_pop);

void report_pop (population *pop, FILE *fpt);
void report_feasible (population *pop, FILE *fpt);
void report_ind (individual *ind, FILE *fpt);

void quicksort_front_obj(population *pop, int objcount, int obj_array[], int obj_array_size);
void q_sort_front_obj(population *pop, int objcount, int obj_array[], int left, int right);
void quicksort_dist(population *pop, int *dist, int front_size);
void q_sort_dist(population *pop, int *dist, int left, int right);

void selection (population *old_pop, population *new_pop);
individual* tournament (individual *ind1, individual *ind2);


//Added methods
//--------------------------------------------------------------
void allocate_memory_matrix (double ***matrix, int row, int col);
double** allocate_memory_matrix2 (int row, int col);
double* allocate_memory_intercriteria (int size);
int** int_allocate_memory_matrix (int row, int col);

void deallocate_memory_matrix (double **matrix, int row, int col);
void deallocate_memory_intercriteria (double *intercriteria);
void int_deallocate_memory_matrix (int **matrix, int row, int col);

//leer_problema.c
void read_data_parameters(char *ruta);
void read_size(char *ruta);
void read_performance(char *ruta);
void read_intercriteria(char *ruta);
void read_ordering_Group(char *ruta);

void read_ordering_DM(char *ruta);
void read_thresholMutation(char *ruta);
void read_criteria_direction(char *ruta);
void find_highest_performance_criterion();
void read_ordering_ind(char *ruta);
int mi_system (const char *comando);
void read_frecuency(char *ruta);
void read_marketshare(char *ruta);

//objetvies.c
double eva_objective_kendall (double *xreal);
int eva_objective1 (double *xreal);
int eva_objective2 ();
int eva_objective3 ();
int eva_constr_violation_kendall(double *xreal);
int eva_constr_violation(double *xreal, double *obj);
void credibility_matrix(double *xreal);
void credibility_matrix_final();
void credibility_matrix_imprime(char *ruta, double *xreal);
void organiza_parametros_intercriterios(double *xreal, double *pw, double *pq, double *pp, double *pv, double *pt);
double sigma (  double A[], double B[], double *W, double *Q, double *P, double *V, double *T, int *dir   );
double sigma2 (  double A[], double B[], double *W, double *Q, double *P, double *V, double *T, int *dir   );
int RO_lambda_better_than(double **sigma, int a, int b, double lambda, double B);
void firstRanking_agress_matrix(int **agree, int *Oi, int *OG);
void net_flow();
void ordenar_valores(double *valor, int *O);
double fitness_borda(double*xreal);


//crossover.c
void realcrossParam (individual *parent1, individual *parent2, individual *child1, individual *child2);
void realcrossParamMS (individual *parent1, individual *parent2, individual *child1, individual *child2);
int available_mutation_parameter();
individual* mono_tournament (individual *ind1, individual *ind2);

//mutation.c
void real_mutate_ind_param (individual *ind);
void real_mutate_ind_InitialPop (individual *ind, int numParam);

//report.c
void print_ind (individual *ind, int printAlpha);
int busca_ind_repetido (individual *ind);
void agregaIndividuo_best_pop(individual ind);
void quickSort( double[], int, int);
int partition( double[], int, int);

//rand.c
double roundingNdig(double num, int nDec);

//fillnds.c
void mono_fill_nondominated_sort (population *mixed_pop, population *new_pop);

//dominance.c
int check_optimality (individual *a, individual *b);

//distillation.c
void median_preOrder(int *tot_preOrder);


# endif
