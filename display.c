/* Routines to display the population information using gnuplot */

# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <string.h>
# include <unistd.h>

# include "global.h"
# include "rand.h"

/* Function to display the current population for the subsequent generation */
void onthefly_display (population *pop, FILE *gp, int ii)
{
    int i;
    int flag;
    FILE *fpt;
    /*
    //printf("\n%s",num[ii]);
    char num[][20]={"plot1.out", "plot2.out", "plot3.out", "plot4.out", "plot5.out", "plot6.out", "plot7.out", "plot8.out", "plot9.out", "plot10.out" };
    fpt = fopen(num[ii-1],"w");
    */

    fpt = fopen("plot.out","w");
    flag = 0;

    double o1, o2, o3;

    for (i=0; i<popsize; i++)
    {
        if (pop->ind[i].constr_violation==0)
        {

        	o1 = pop->ind[i].obj[obj1-1];
        	o2 = pop->ind[i].obj[obj2-1];

            if (choice!=3)
                fprintf(fpt,"%e\t%e\n",pop->ind[i].obj[obj1-1],pop->ind[i].obj[obj2-1]);
            else{
            	o3 = -1 * pop->ind[i].obj[obj3-1]; //Max obj3
                fprintf(fpt,"%e\t%e\t%e\n",pop->ind[i].obj[obj1-1],pop->ind[i].obj[obj2-1], o3);

            }
            fflush(fpt);
            flag = 1;
        }
    }
    if (flag==0)
    {
        printf("\n No feasible soln in this pop, hence no display");
    }
    else
    {
    	/*
    	if(ii==1)  system("cat plot.out > todos.out");
    	else   	system("cat plot.out >> todos.out");
		*/
        if (choice!=3){
            fprintf(gp,"set title 'Generation #%d'\n unset key\n plot 'plot.out' w points pointtype 6 pointsize 1\n",ii);

        	//se agregaron rangos de los puntos
            //fprintf(gp,"set title 'Generation #%d'\n unset key\n plot [-0.2:8] [-0.2:15] 'plot.out' w points pointtype 6 pointsize 1\n",ii);
        }
        else
            fprintf(gp,"set title 'Generation #%d'\n set view %d,%d\n unset key\n splot 'plot.out' w points pointtype 6 pointsize 1\n",ii,angle1,angle2);
        fflush(gp);
    }
    fclose(fpt);

    return;
}
