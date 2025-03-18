#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char **argv){
        pid_t p;
        int status;
        int x = 17;
        p = fork();
        if (p < 0){ /*error*/
                perror("fork");
                _exit(1);
        }
        else if (p == 0){ /*child*/
                x = 42;
                printf("Hello World I am a child\nMy pid is %d\nMy parent's pid is %d\nThe given x is: %d\n", getpid(), getppid(), x);
                if (execv("char-count", argv) == -1) {
                        perror("execv");
                        _exit(1); // Exit with an error code
                }

                _exit(0); /*terminates the child*/
        }
        else { /*p > 0 and p = child's pid*/
                wait(&status);
                printf("Hello I am the parent\nMy child's pid is %d\nThe given x is: %d\n", p, x);
        }
        return 0;
}

