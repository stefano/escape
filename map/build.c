#include <stdio.h>
#include <stdlib.h>

#define SZ 1024

int main(int argc, char **argv)
{
  if (argc != 2)
    {
      printf("Usage: build output.map");
      exit(1);
    }
  FILE *f = fopen(argv[1], "w");
  int i,j;
  for (i = 0; i < SZ; i++)
    for (j = 0; j < SZ; j++)
      {
        /* random generator for the moment */
        unsigned char n = rand()%256;
        fwrite(&n, sizeof(unsigned char), 1, f);
      }
  fclose(f);

  return 0;
}
