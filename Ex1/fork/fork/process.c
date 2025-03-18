//File for Exercise 1.2.2v
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc , char **argv){
    pid_t p;
    int status;
    int x = 17;

    p = fork(); //creating a child process/
    if (p < 0){  //error/
        perror("Fork");
        _exit(0);
    }

    else if (p == 0){ //child/
        x = 42;
        printf("Hello World I am a child\nMy pid is %d\nMy parent's pid is %d\nGiven x is %d\n", getpid(), getppid(), x);
        _exit(0); //terminates the child/
    }

    else { //p > 0 and p = child's pid/
        wait(&status); //parent waits for the child to terminate/
        //x = 17;
        printf("Hello I am the parent\nMy child's pid is %d\nGiven x is %d\n", p, x);
    }
    return 0;
}
