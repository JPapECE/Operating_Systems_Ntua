#define _GNU_SOURCE
#define _POSIX_SOURCE
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

int process_counter = 0;
pid_t ppid;
int flag = 0; /*a flag so we make sure the sigusr1 message is printing only one time*/

void control_c_handler(int signum) {
        flag = 1;
        kill(ppid, SIGUSR1); /*send a signal ONLY to the parent process*/
}

void sigusr1_handler(int signum) {
        if (flag)
                printf("\n%d children are searching\n", process_counter);
        flag = 0;
}

int main(int argc, char **argv) {
        ppid = getpid();
        signal(SIGINT, control_c_handler);
        signal(SIGUSR1, sigusr1_handler);
        int fd[2];
        const int P = 4; /*as we increase P, the total searching time decreases*/
        char buff[10];
        int pipefd[P][2];

        /* Create P pipes */
        for (int i = 0; i < P; i++) {
                if (pipe(pipefd[i]) == -1) {
                        perror("pipes");
                        _exit(1);
                }
        }

        printf("Here is the input file %s\n", argv[1]);
        fd[0] = open(argv[1], O_RDONLY); /*open file to read*/
        fd[1] = open(argv[2], O_WRONLY); /*open file to write*/
        char *c = argv[1]; /*read character to search*/

        if (fd[0] == -1) { /*error reading file*/
                perror("open1");
                _exit(1);
        }
        if (fd[1] == -1) { /*error writing file*/
                perror("open2");
                _exit(1);
        }

        pid_t p[P];
        int status;

        /* Create P child processes, each reading from the file */
        for (int i = 0; i < P; i++) {
                p[i] = fork();

                if (p[i] < 0) { /*error*/
                        perror("fork");
                } else if (p[i] == 0) { /*child*/
                        /* Write to the parent */
                        close(pipefd[i][0]); /*close read end*/
                        ssize_t rcnt;
                        int cnt = 0;
                        for (;;) {
                                rcnt = read(fd[0], buff, sizeof(buff) - 1); /* Read from file */

                                if (rcnt == -1) { /* Error */
                                        perror("child's read");
                                        _exit(1);
                                } else if (rcnt == 0) { /*End of file*/
                                        buff[rcnt] = '\0';
                                        break;
                                }
                                int i = 0;
                                while (buff[i] != '\0') {
                                        if ((buff[i] == *c) || (buff[i] == *c - 'a' + 'A'))
                                                cnt++;
                                        i++;
                                }
                                usleep(100000);
                        }
                        ssize_t wcnt;
                        wcnt = write(pipefd[i][1], &cnt, sizeof(&cnt));

                        if (wcnt == -1) { /*error*/
                                perror("child's write");
                                _exit(1);
                        }

                        close(pipefd[i][1]); /*close the write end*/
                        process_counter++;
                        _exit(0); /*child terminates*/
                } else {
                        process_counter++; /*parent increments child count*/
                        close(pipefd[i][1]); /*close write end*/
                }
        }

        /* Parent receives from its children */
        int sum = 0;

        for (int i = 0; i < P; i++) {
                waitpid(p[i], &status, 0); /*parent waits for i-th child*/
                process_counter--;
                int child_cnt;
                ssize_t rcnt;
                rcnt = read(pipefd[i][0], &child_cnt, sizeof(child_cnt));

                if (rcnt == -1) {
                        perror("Parent's read");
                        _exit(1);
                }
                sum += child_cnt; /*add to total sum*/
                printf("checkpoint: The %d-th child found %d characters, total: %d\n", i + 1, child_cnt, sum);
                usleep(100000);
        }
        ssize_t wcnt;
        char tmpbuf[10];
        int n = sprintf(tmpbuf, "%d\n", sum); /*convert int to string*/
        int idx = 0;
        do {
                wcnt = write(fd[1], tmpbuf + idx, n - idx);
                if (wcnt == -1) {/*error*/
                        perror("write");
                        return 1;
                }
                idx += wcnt;
        } while( idx < n);

        if(close(fd[0]) == -1){
                perror("Bad close read");
                _exit(1);
        }

        if(close(fd[1]) == -1){
                perror("Bad close write");
                _exit(1);
        }
}
