#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
  char *ptr = malloc(33);
  strcpy(ptr, "Hello from My Database project!");
  puts(ptr);
  free(ptr);
  return 0;
}
