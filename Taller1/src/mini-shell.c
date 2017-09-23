#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define NELEMS(a) (sizeof(a) / sizeof((a)[0]))

static int run(const char ***progs, size_t count)
{
	int (*pipes)[2];
	pid_t *children; //pid_t : process id 
	size_t i, j;
	pid_t cur;
	int r, status;

	if (!(children = malloc(sizeof(*children) * count))) {
		fputs("out of memory\n", stderr);
		goto end;
	}

	/* ----------------- */
	/* TODO: crear pipes */
	/* ----------------- */
    if(!(pipes = malloc(sizeof(*pipes) * (count-1)))) {
        fputs("out of memory for pipes\n", stderr);
        goto end;
    }

	for (i = 0; i < count; i++) {
		
        if(i < count-1) pipe(pipes[i]);

		if ((cur = fork()) == -1) {
			fprintf(stderr, "fork [%zu]", i);
			perror("");
			r = -1;
			goto end;
		}

		if (cur == 0) {
			
			/* ------------------------------------ */
			/* TODO: redireccionar los fd adecuados */
			/* ------------------------------------ */
            printf("Entre %d\n", i); fflush(stdout);
			if (i == 0) {
                dup2(pipes[i][1], 1);
                close(pipes[i][0]);
            } else if (i == count - 1){
                dup2(pipes[i-1][0], 0);
                close(pipes[i-1][1]);
            } else {
                dup2(pipes[i-1][0], 0);
                dup2(pipes[i][1], 1);
                close(pipes[i-1][1]);
                close(pipes[i][0]);
            }

			if (execvp(progs[i][0], progs[i]) == -1) {
				perror("execvp");
				return -1;
			}
			exit(1); /* no alcanzable */
		} else {
			children[i] = cur;
			
		}
	}

	for (i = 0; i < count; i++) {
        //printf("Esperando thread %d", i); fflush(stdout);
		if (waitpid(children[i], &status, 0) == -1) {
			perror("waitpid");
			return -1;
		}
		if (!WIFEXITED(status)) {
			fprintf(stderr, "proceso %d no terminó correctamente [%d]: ",
			    (int)children[i], WIFSIGNALED(status));
			perror("");
			return -1;
		}
	}
	r = 0;

end:
	free(children);
	free(pipes);

	return r;
}

int
main(int argc, char **argv)
{
    printf("Llege");
	fflush(stdout);
    char *lscmd[] = { "ls", "-al", NULL };
	char *wccmd[] = { "wc", NULL };
	char *awkcmd[] = { "awk", "{ print $2 }", NULL };
	char **progs[] = { lscmd, wccmd, awkcmd };
	printf("status: %d\n", run(progs, NELEMS(progs)));
	fflush(stdout);
	fflush(stderr);

	return 0;
}
