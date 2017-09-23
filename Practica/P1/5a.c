#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main()
{
    int status; 
    pid_t homer = fork();
    if(homer == -1)
    {
        perror("ERROR fork");
        exit(1);
    }
    if(homer == 0)
    {
        /*Proceso hijo*/
        int HIJOS = 3;
        int childs_status[HIJOS];
        pid_t childs[HIJOS];
        for(int i = 0; i < HIJOS; i++)
        {   
            childs[i] = fork();
            if(childs[i] == -1)
            {
                perror("ERROR fork on child");
            }

            if(childs[i] == 0)
            {   /*Proceso "nieto" ponele*/
                switch(i)
                {
                    case 0:
                        printf("Bart\n");
                        exit(0);       
                        break;
                    case 1:
                        printf("Lisa\n");
                        exit(0);
                        break;
                    default:
                        printf("Maggie\n");
                        exit(0);
                        break;
                }

            } 
        }

        for(int i = 0; i < HIJOS; i++) /* Falta chequear status de waitpid */
            waitpid(childs[i], &childs_status[i], 0);
        printf("Homer\n");
        exit(0);
    } 
    else 
    {
        /* Falta chequear status de waitpid */
        int ret = waitpid(homer, &status, 0);
        printf("Abraham\n");
    }
    return 0;
}