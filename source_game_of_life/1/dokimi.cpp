#include <stdio.h>
int j;
 main(){
     
     int i=0;
     int **p;
     p=&i;
     printf("%d\n",p);
     
     p=&j;
     printf("%d\n",**p);
     scanf("%d",i);
     
     
     }
