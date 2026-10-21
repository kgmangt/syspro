#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
struct book {
   int id;
   char bookname[20]; 
   char author[20];
   int year;
   int numofborrow;
   char borrow[5];
};

int main(int argc, char *argv[])
{
   struct book rec;
   int a;
   FILE *fp;
   if (argc != 2) {
      fprintf(stderr, "How to use: %s FileName\n", argv[0]);
      exit(1);
   }

   if ((fp = fopen(argv[1], "rb")) == NULL ) {
      fprintf(stderr, "Error Open File\n");
      exit(2);
   }
   
   printf("0: list of all books, 1: list of available books)");
   scanf("%d",&a);
	if(a == 0){
	   while (fread(&rec, sizeof(rec), 1, fp) > 0) 
      if (rec.id != 0)
         printf("%d	%s	 %s	 %d	 %d	 %s \n", rec.id,  rec.bookname,  rec.author,rec.year,rec.numofborrow,rec.borrow);
   
   printf("-----------------------------------\n");
   fclose(fp);
   return 0;
} 
if(a==1){
if(strcmp(rec.borrow,"True")){
		while (fread(&rec, sizeof(rec), 1, fp) > 0) 
      if (rec.id != 0)
         printf("%d	%s	%s	%d	%d	%s \n", rec.id,  rec.bookname,  rec.author,rec.year,rec.numofborrow,rec.borrow);
   
   printf("-----------------------------------\n");
   fclose(fp);
   return 0;
} 

		}

		fclose(fp);
	  exit(0);
}
