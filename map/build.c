#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SZ 1024
#define MAX_DIFF 10

int main(int argc, char **argv)
{
  if (argc != 2)
    {
      printf("Usage: build output.map");
      exit(1);
    }
  FILE *f = fopen(argv[1], "w");
  int i,j;
  unsigned char res[SZ][SZ];
  time_t t;
  srand(time(&t));
  for (i = 0; i < SZ; i++)
    for (j = 0; j < SZ; j++)
      {
        /* random generator */
        int n = -1;
        unsigned char prev = 0;
        if (j == 0)
          {
            if (i == 0) 
              n = rand()%256;
            else
              prev = res[i-1][j];
          }
        else
          prev = res[i][j-1];
        if (n!=-1)
          res[i][j] = n;
        else
          {
            int sign = rand()%2;
            unsigned char lat = prev;
            /*            if (sign == 3) 
              {
                res[i][j] = 0;
                continue;
                }*/
            if (i > 0) 
              lat = res[i-1][j];
            /* more chances to go down */
            if (sign != 1)
              sign = -1;
            n = (prev+lat)/2 + sign*rand()%MAX_DIFF;
            if (n < 0)
              n = 0;
            if (n > 255)
              n = 255;
            res[i][j] = n;
          }
      }
  fwrite(&res[0][0], sizeof(unsigned char), SZ*SZ, f);
  fclose(f);

  return 0;
}
