#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char* argv[]) {
	int status;
	pid_t child;
	struct user_regs_struct regs;
	if (argc <= 1) {
		fprintf(stderr, "Uso: %s commando [argumentos ...]\n", argv[0]);
		exit(1);
	}

	/* Fork en dos procesos */
	child = fork();
	if (child == -1) { perror("ERROR fork"); return 1; }
	if (child == 0) {
		/* Solo se ejecuta en el Hijo */
		if(ptrace(PTRACE_TRACEME, 0, NULL, NULL)){
			perror("ERROR child ptrace(PTRACE_TRACEME, ...)");
			exit(1);
		}

		execvp(argv[1], argv+1);
		/* Si vuelve de exec() hubo un error */
		perror("ERROR child exec(...)"); exit(1);
	} else {
		/* Solo se ejecuta en el Padre */
		while(1) {
			ptrace(PTRACE_SYSCALL, child, NULL, NULL);
			
			if (wait(&status) < 0) { perror("waitpid"); break; }
			if (WIFEXITED(status)) break; /* Proceso terminado */
			
			if(WSTOPSIG(status) == SIGTRAP){ //Se freno por una syscall
				
				long ret = ptrace(PTRACE_GETREGS, child, NULL, &regs);
				int sysno = regs.orig_rax; //sysno = nro de syscall que intento hacer
				
				if (sysno == SIGKILL){  //si intento enviar señal SIGKILL
					fprintf(stdout, "Se ha hecho justicia!\n");
					ptrace(PTRACE_KILL, child, NULL, NULL);
				}
			}
		}
	}
	return 0;
}
