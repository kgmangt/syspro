#include <stdio.h> 
#include <stdlib.h>
struct book {
   int id; 
   char bookname[20];
   char author[20];
   int year;
   int numofborrow;
   char borrow[5];
};
int main(int argc, char* argv[])
{
   struct book rec;
   FILE *fp;

   if (argc != 2) {
      fprintf(stderr, "How to use: %s FileName\n",argv[0]);
      exit(1);
   }

   fp = fopen(argv[1], "wb");
   printf("Id	bookname	author	year	numofborrow		borrow\n");
   while (scanf("%d %s %s %d %d %s ", &rec.id,rec.bookname, rec.author,& rec.year, &rec.numofborrow, rec.borrow ) == 6)
      fwrite(&rec, sizeof(rec), 1, fp);

   fclose(fp);
   exit(0);
}

