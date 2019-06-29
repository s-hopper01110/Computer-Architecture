#include "cpu.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */

//RAM Functions:
  unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address){
    return cpu->ram[address];
  }

  unsigned char cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value){
    return cpu->ram[address] = value;
  }



/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
// TODO: Replace this with something less hard-coded

// void cpu_load(struct cpu *cpu)
// {
//   char data[DATA_LEN] = {
//     // From print8.ls8
//     0b10000010, // LDI R0,8
//     0b00000000,
//     0b00001000,
//     0b01000111, // PRN R0
//     0b00000000,
//     0b00000001  // HLT
//   };

//   int address = 0;

//   for (int i = 0; i < DATA_LEN; i++) {
//     cpu->ram[address++] = data[i];
//   }

  
// }

// Load the binary bytes from a .ls8 source file into a RAM array
// TODO: Replace this with something less hard-coded:

void cpu_load(struct cpu *cpu, int argc, char *argv[])
{

  if (argc < 2)
  {
    printf("Error\n");
    exit(1);
  }
  char *file_name = argv[1];
  
  FILE  *fpr;
    char f_line[1024];
    int address = 0;
  fpr = fopen(file_name, "r");
  if( fpr == NULL) {
    fprintf(stderr, "Error\n");
    exit(1);
  } 
    
   while (fgets(f_line, 1024, fpr) != NULL) {
    char *endptr;
    unsigned char i = strtoul(f_line, &endptr,2);
    if (endptr == f_line) {
      continue;
    }
   cpu->ram[address] = i;
   address++;
  }
  fclose(fpr);
}




// ALU:
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      // TODO
      cpu->registers[regA] = cpu->registers[regA] * cpu->registers[regB];
      break;
    // TODO: implement more ALU ops
    case ALU_CMP:
      if(cpu->registers[regA] == cpu->registers[regB]){
        cpu->FL = 0b00000001;
      }else if(cpu->registers[regA] > cpu->registers[regB]){
        cpu->FL = 0b00000010;
      }else if(cpu->registers[regA] < cpu->registers[regB]){
        cpu->FL = 0b00000100;
      }
      break;
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    unsigned char IR = cpu->ram[cpu->PC];
    // 2. Figure out how many operands this next instruction requires
    unsigned int operands = IR >> 6;
    // 3. Get the appropriate value(s) of the operands following this instruction
    unsigned char operandA = cpu_ram_read(cpu, cpu->PC + 1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->PC + 2);
    // 4. switch() over it to decide on a course of action.
    switch(IR){
       // 5. Do whatever the instruction should do according to the spec.
       case HLT://Halt the CPU
        running = 0;
        break;
       case LDI://LDI register immediate //set value of register to interger
        cpu->registers[operandA] = operandB;
        break;
       case PRN://PRN print numeric value stored in given register
        printf("%d\n", cpu->registers[operandA]);
        break;
        case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        break;
        //Sprint Challenge 
        case CMP:
        alu(cpu, ALU_CMP, operandA, operandB);
         break;

        case JMP:
        cpu->PC = cpu->registers[operandA];
        operands = 0;
        break;
        case JEQ:
        if(cpu->FL == 1){
          cpu->PC = cpu->registers[operandA];
          operands = 0;
        }
        break;
        case JNE:
        if(cpu->FL == 0){
          cpu->PC = cpu->registers[operandA];
          operands = 0;
        }
        break;
       default:
        break;
    }
    // 6. Move the PC to the next instruction.
      cpu->PC += (operands + 1);
  }
} 

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = 0;
  //Sprint Challenge 
  cpu->FL = 0; 
  //memset - fill a block of memory with a particular value
  memset(cpu->registers, 0, sizeof(cpu->registers));
  memset(cpu->ram, 0, sizeof(cpu->ram));


}
