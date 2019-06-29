#include <stdio.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, char *argv[])
{
  if(argc != 2){
    fprintf(stderr, "Error");
  }
  struct cpu cpu;

  cpu_init(&cpu);
  cpu_load(&cpu, argc, argv);
  cpu_run(&cpu);

  return 0;
}