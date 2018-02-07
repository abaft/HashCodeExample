#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct _pizza
{
  char* pizzaData;
  int R, C, L, H;
}pizza;

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


int main(int argc, char** argv)
{
  pizza p = parseFile("example.in");

}
