#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(void) {
    int p1[2], p2[2], p3[2];  
    pipe(p1);                  
    pipe(p2);                  
    pipe(p3);                  
    char buf[256];

    if (fork() == 0) {
        close(p1[0]);          
        close(p2[0]); close(p2[1]); 
        close(p3[0]); close(p3[1]); 

        write(p1[1], "Hello", strlen("Hello"));
        close(p1[1]);           
        exit(0);
    }

    if (fork() == 0) {
        close(p1[1]);       
        close(p2[0]);
        close(p3[0]); close(p3[1]); 

        ssize_t n = read(p1[0], buf, sizeof(buf)); 
        printf("Процесс 2 получил: %.*s\n", (int)n, buf);
        close(p1[0]); 
        
        write(p2[1], buf, n); 
        close(p2[1]);
        exit(0);
    }

    if (fork() == 0) {
        close(p1[0]); close(p1[1]); 
        close(p2[1]);         
        close(p3[0]);       

        ssize_t n = read(p2[0], buf, sizeof(buf)); 
        printf("Процесс 3 получил: %.*s\n", (int)n, buf);
        close(p2[0]);

        write(p3[1], buf, n);  
        close(p3[1]);
        exit(0);
    }

    close(p1[0]); close(p1[1]); 
    close(p2[0]); close(p2[1]); 
    close(p3[1]);              

    ssize_t n = read(p3[0], buf, sizeof(buf)); 
    printf("Родитель получил: %.*s\n", (int)n, buf);
    close(p3[0]);

    wait(NULL); wait(NULL); wait(NULL); 
    return 0;
}
