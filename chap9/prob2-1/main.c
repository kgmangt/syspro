#include <sys/types.h> 
#include <sys/wait.h>
pid_t wait(int *status);
pid_t waitpid(pid_t pid, int *statloc, int options);
int main() 
{
   int pid, child, status;
   printf("[%d] parent process start \n", getpid( ));
   pid = fork();
   if (pid == 0) {
      printf("[%d] process start \n", getpid( ));
      exit(1);
   }
   child = wait(&status);
   printf("[%d] student process %d end \n", getpid(), child);
   printf("\tend code %d\n", status>>8);
}

