/*
 * char-count.c
 *
 * A simple C program that counts the occurrences of a character in a file
 * and writes the result to another file.
 *
 * Input is provided from the command line without further validation:
 * argv[1]: File to read from
 * argv[2]: File to write to
 * argv[3]: Character to search for
 *
 * Author: Dimitris Papaderakis
 * Electrical and Computer Engineering (ECE) Student
 *
 * Operating Systems Course, CSLab, ECE, NTUA
 *
 */

 #include <stdio.h>
 #include <errno.h>
 #include <fcntl.h>
 #include <unistd.h>
 
 #define BUFFER_SIZE 1024
 
 int main(int argc, char **argv) {
         if (argc != 4){
                 fprintf(stderr, "Error: Invalid number of arguments.\n");
                 _exit(1);
         }
 
         char buff[BUFFER_SIZE];
         ssize_t rcnt;
         int fd[2];
         int cnt = 0; /* an integer to count */
 
         char *input_file = argv[1];
         char *output_file = argv[2];
         char c = argv[3][0]; /* read character to search */
 
         fd[0] = open(input_file, O_RDONLY); /* open file to read */
 
         if (fd[0] == -1) { // error in reading file
                 perror("Error opening input file");
                 _exit(1);
         }
 
         fd[1] = open(output_file, O_WRONLY | O_TRUNC | O_CREAT); /* open file to write */
 
         if (fd[1] == -1) { // error in writing file
                 perror("Error opening output file");
                 _exit(1);
         }
 
 
         for (;;) {
                 rcnt = read(fd[0], buff, sizeof(buff) - 1); /* reading the file and copy its content to a buffer */
                 if (rcnt == 0) /* end of file */ break;
                 if (rcnt == -1) { /* error */
                         perror("read");
                         _exit(1);
                 }
 
 
                 buff[rcnt] = '\0'; /* make the last character a string terminator */
                 int i = 0;
                 while (buff[i] != '\0') {
                         if ((buff[i] == c) || (buff[i] == c - 'a' + 'A')) cnt++;
                         /* count how many chars match the desired char, case insensitive */
                         i++;
                 }
         }
         ssize_t wcnt;
         char tmpbuf[16];
         int n = sprintf(tmpbuf, "%d\n", cnt); /* make the int cnt a string */
         int idx = 0;
         do {
                 wcnt = write(fd[1], tmpbuf + idx, n - idx);
                 if (wcnt == -1) { /* error */
                         perror("write");
                         _exit(1);
                 }
                 idx += wcnt;
         } while (idx < n);
 
         if (close(fd[0]) == -1) { /* close the reading file */
                 perror("Error closing the input file");
                 _exit(1);
         }
 
         if (close(fd[1]) == -1) { /* close the writing file */
                 perror("Error closing the output file");
                 _exit(1);
         }
 
         printf("%d\n", cnt); /* for checking the result */
         return 0;
 }
 