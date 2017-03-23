#include <stdio.h>
#include <stdlib.h>
#include "sqmacro.h"

int main(int argc, char *argv[])
{
  srand((unsigned int)time(NULL));

  float a = 5.0;
  for (int i=0;i<20;i++)
    printf("%f\n", RAND_F(a));
  return 0;
}
