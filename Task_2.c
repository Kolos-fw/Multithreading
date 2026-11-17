#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){
    pid_t pid = fork();
    if (pid == 0) {  
        execvp(argv[1], &argv[1]);                                     
        exit(127);
    }

    int status;
    waitpid(pid, &status, 0);

    if (WEXITSTATUS(status) == 127){
        printf("runner: не удалось запустить %s\n", argv[1]);
    }
    else {
        printf("runner: команда завершилась с кодом %d\n", WEXITSTATUS(status));
    }

    return 0;
}
