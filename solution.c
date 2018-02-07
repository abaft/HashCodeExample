#define _GNU_SOURCE
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define DEPTH 10000
int recorder;

typedef struct _pizza
{
  char* pizzaData;
  int R, C, L, H;
}pizza;

typedef struct _slice
{
  int r1,c1,r2,c2;
}slice;

pizza parseFile(char* fileName)
{
  FILE* file;
  file = fopen(fileName, "r");


  char* tmpString = calloc (5, sizeof(char));
  int values[4];
  //Read in 
  for (int i = 0; i < 4; i++){
    char tmp = fgetc(file);
    while (tmp != ' ' && tmp != '\n')
    {
      strcat(tmpString, &tmp);
      tmp = fgetc(file);
    }
    values[i] = atoi(tmpString);
    memset(tmpString, '\0', 5);
  }

  pizza rtn;
  rtn.R = values[0];
  rtn.C = values[1];
  rtn.L = values[2];
  rtn.H = values[3];
  rtn.pizzaData = malloc (sizeof(char) * rtn.R * rtn.C * 10);

  char* line = rtn.pizzaData;
  size_t n = sizeof(char) * rtn.C;
  for (int i = 0; i < rtn.R; ++i)
  {
    getline(&line, &n, file);
    line += rtn.C;
  }
  
  fclose(file);
  free(tmpString);
  return rtn;
}

char* navC(pizza p, int r, int c) { return (p.pizzaData + c + r * p.C); }

char checkSlice (pizza *p, slice s)
{
  int m = 0;
  int t = 0;

  for (int i = 0; i <= abs(s.r2-s.r1); ++i)
    for (int j = 0; j <= abs(s.c2-s.c1); ++j)
    {
      if (*navC(*p, s.r1+i, s.c1+j) == 'M')
        ++m;
      else if (*navC(*p, s.r1+i, s.c1+j) == 'T')
        ++t;
      else if (*navC(*p, s.r1+i, s.c1+j) == 'X')
        return 0;
    }

  return m >= p->L && t >= p->L && (m + t) <= p->H;
}

char nullSlice (pizza *p, slice s)
{

  for (int i = 0; i <= abs(s.r2-s.r1); ++i)
    for (int j = 0; j <= abs(s.c2-s.c1); ++j)
      *navC(*p, s.r1+i, s.c1+j) = 'X';

  return 0;
}

slice randomSlicer(pizza p)
{
  slice rtn;
  int r1 = rand()%p.R;
  int c1 = rand()%p.C;
  int r2 = rand()%p.R;
  int c2 = rand()%p.C;

  rtn.r1 = r1 <= r2 ? r1:r2;
  rtn.r2 = r1 > r2 ? r1:r2;
  rtn.c1 = c1 <= c2 ? c1:c2;
  rtn.c2 = c1 > c2 ? c1:c2;
  return rtn;
}

int sliceArrayScore (slice* s, int n)
{
  int score = 0;
  for (int i = 0; i < n; ++i)
    score += ((s[i].r2-s[i].r1) + 1) + ((s[i].c2-s[i].r1) + 1);
}

slice* sliceArrayGen (pizza p)
{
  slice* 
    rtn = malloc(sizeof(slice) * p.R * p.C); //Make it big enough for anything
  int n = 0;
  for (int i = 0; i < DEPTH; ++i)
  {
    slice tmp;
    if (checkSlice(&p, (tmp = randomSlicer(p))))
    {
      nullSlice (&p, tmp);
      printf("%i, %i, %i, %i\n", tmp.r1, tmp.c1, tmp.r2, tmp.c2);
      rtn[n++] = tmp; 
    }
  }
  recorder = n;
  return rtn;
}

int main(int argc, char** argv)
{
  pizza p = parseFile("example.in");
  slice* s;
  do
  {
    srand(time(NULL));
    s = sliceArrayGen(p);
    p = parseFile("example.in");
    printf("%i\n", sliceArrayScore(s, recorder));
    free(s);
  }while(1);
}
