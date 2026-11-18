#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main( ) 
{
   printf("start of perent process\n");
   if (fork( ) == 0) {
      execl("/bin/echo", "echo", "hello", NULL);
      fprintf(stderr,"frist fail"); 
      exit(1);
   }
   
if (fork( ) == 0) {
      execl("/bin/date", "date", NULL);
      fprintf(stderr,"secend fail"); 
      exit(2);
  }

   if (fork( ) == 0) {
      execl("/bin/ls","ls", "-l", NULL);
      fprintf(stderr,"third fail"); 
      exit(3);
   }
   printf("end of parent process\n");
}

