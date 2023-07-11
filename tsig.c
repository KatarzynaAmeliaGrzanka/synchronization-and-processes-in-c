// Program in C which tests synchronization mechanizms and signals

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include<sys/wait.h>

#define NUM_CHILD 4
volatile sig_atomic_t keyboard_interrupt_occurence = 0; // variable that may change when keyboard interrupt occures

void ignore_all_signals(){  // function for ignoring all signals and restoring the default handler for SIGCHLD
    int i;
    for (i = 1; i < NSIG; i++) {
    signal(i, SIG_IGN); 
    }
    signal(SIGCHLD, SIG_DFL);
}

void my_sigint_handler(int sig) {
    printf("\nReceived SIGINT signal (keyboard interrupt) \n");
    keyboard_interrupt_occurence = 1;

}

void my_sigterm_handler(int sig) {
    printf("child[%d]: Received termination signal (SIGTERM). Process terminated\n", getpid());
   // exit(0);
}

void my_sigchld_handler(int sig) {
    printf("Received child termination signal (SIGCHLD)\n");
}


int main() {
    
    int i, j;
    pid_t pid[NUM_CHILD];

    #ifdef WITH_SIGNALS
        ignore_all_signals();
        signal(SIGINT, my_sigint_handler);
    #endif

    for (i = 0; i < NUM_CHILD; i++) {
        sleep(1); // add a 1 second delay between fork calls

        #ifdef WITH_SIGNALS
            if (keyboard_interrupt_occurence){
                printf("Interrupt of the of the creation process detected. Sending SIGTERM to all created processes.\n");
                for(j = 0; j < i; j++){
                    kill(pid[j], SIGTERM);
                }
                break;
            }
        #endif

        pid[i] = fork();

        if (pid[i] < 0) {
            printf("Error: Fork failed.\n");
            for (j = 0; j < i; j++)
            {
                kill(pid[j], SIGTERM);
            }
            exit(1);
        } 
        else if (pid[i] == 0) {   // child process algorithm

            #ifdef WITH_SIGNALS
                signal(SIGINT, SIG_IGN);
                signal(SIGTERM, my_sigterm_handler);
            #endif

            printf("child[%d] created, parent ID: %d\n", getpid(), getppid());
            sleep(10);
            printf("child[%d] Execution completed\n",getpid() );
            exit(0);
        } 
    }
    sleep(1);

    // printf("\nAll child processes created successfully\n");

    int count = 0;
    while (wait(NULL) > 0) {
               count ++;
    }
        printf("All child processes terminated. Received %d exit codes\n", count);

    signal(SIGINT, SIG_DFL);
    signal(SIGCHLD, SIG_DFL);
    signal(SIGTERM, SIG_DFL);

   return 0;
}