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
	int npipes[2][2];
	pipes = npipes;
	
	for(i = 0; i < count - 1; i++){
		if(pipe(pipes[i])){
			perror("bad pipe");
			exit(1);
		}
	}

	for (i = 0; i < count; i++) {
		
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
			if(i == 0){
				close(pipes[0][0]);
				dup2(pipes[0][1], 1); //redirecciona stdout
			} else if(i == count - 1) {
				close(pipes[i-1][1]);
				dup2(pipes[i-1][0], 0);
			} else {
				close(pipes[i-1][1]);
				close(pipes[i][0]);

				dup2(pipes[i-1][0], 0);
				dup2(pipes[i][1], 1);
			}

			if (execvp(progs[i][0], progs[i]) == -1) {
				perror("execvp");
				return -1;
			}
			exit(1); /* no alcanzable */
		} else {
			children[i] = cur;
			
			if(i == 0){
				close(pipes[0][1]);
			} else if(i == count - 1){
				close(pipes[i-1][0]);
			} else {
				close(pipes[i-1][0]);
				close(pipes[i][1]);
			}
		}
	}

	for (i = 0; i < count; i++) {
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
	char *lscmd[] = { "ls", "-al", NULL };
	char *wccmd[] = { "wc", NULL };
	char *awkcmd[] = { "awk", "{ print $2 }", NULL };
	char **progs[] = { lscmd, wccmd, awkcmd };

	printf("status: %d\n", run(progs, NELEMS(progs)));
	fflush(stdout);
	fflush(stderr);

	return 0;
}
