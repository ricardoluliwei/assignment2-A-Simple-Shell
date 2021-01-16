#include "stdio.h"
#include "stdlib.h"
#include "string.h"


int main(){
    char input[80], *buffer, full[80];
    while (1)
    {
        fgets(input, (sizeof input / sizeof input[0]), stdin);
        if(strcmp(input, "quit")){break;}
    }
    
   
    return 0;
}