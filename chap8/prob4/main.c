 #include <stdio.h>
  #include <stdlib.h>
  static void exit_handler1(void), 
                 exit_handler2(void);
 
  int main(void) 
  {
   if (atexit(exit_handler1) != 0)	
      perror("exit_handler1 cant register");
   if (atexit(exit_handler2) != 0) 	
     perror("exit_handler2 can't register");
  printf("main end \n");
 exit(0);
 }
 static void exit_handler1(void) 
 {
    printf("at First exit handling\n");
 }

 static void exit_handler2(void) 
 {
   printf("second exit handling\n");
 }

