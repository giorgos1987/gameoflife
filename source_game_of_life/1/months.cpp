#include <stdio.h>
//epistrefei to onoma tou niostou mina
char *monthName(int);

main(){
        int k;
        
     scanf("%d", k);
     printf("\n %c \n",monthName(1));

}


char *monthName(int a){
     static char *name[] = {
            "not a month",
            "Jan","Feb","Mar","Apr",
            "May","Jun","Jul"            
            };
     
     return (a < 1 || a > 12)?name[0]:name[a];
     
     
     }
