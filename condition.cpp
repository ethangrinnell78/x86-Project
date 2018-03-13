#include "header.h"

/*
int CCC(instruction *inst)
{
  // format of instruction:
  // 0 000 000 010 10N ZVC

  uint16_t op = inst->opcode;
  string code_string = "CL";

  // clear flags based on last 4 bits of opcode
  if(op & 0000010) {
    inst->N = 0;
    code_string += "N";
  }
  if(op & 0000004) {
    inst->Z = 0;
    code_string += "Z";
  }
  if(op & 0000002) {
    inst->V = 0;
    code_string += "V";
  }
  if(op & 0000001) {
    inst->C = 0;
    code_string += "C";
  }
  
  // special case for all bits set
  if(op & 0000017) {
    code_string = "CCC";
  }

  // special case for no bits set (NOP)
  if(!(op & 0000017)) {
    code_string = "NOP";
  }

  inst->op_text = code_string;

  return 0;
}

int SCC(instruction *inst)
{
  // format of instruction:
  // 0 000 000 010 11N ZVC

  uint16_t op = inst->opcode;
  string code_string = "SE";

  // clear flags based on last 4 bits of opcode
  if(op & 0000010) {
    inst->N = 1;
    code_string += "N";
  }
  if(op & 0000004) {
    inst->Z = 1;
    code_string += "Z";
  }
  if(op & 0000002) {
    inst->V = 1;
    code_string += "V";
  }
  if(op & 0000001) {
    inst->C = 1;
    code_string += "C";
  }
  
  // special case for all bits set
  if(op & 0000017) {
    code_string = "SCC";
  }

  // special case for no bits set (NOP)
  if(!(op & 0000017)) {
    code_string = "NOP";
  }

  inst->op_text = code_string;

  return 0;
}
*/

int CLC(instruction *inst) // Clear carry
{
  inst->op_text = "CLC";
  inst->C = 0;
  return 0;
}


int CLV(instruction *inst) // Clear overflow
{
  inst->op_text = "CLV";
  inst->V = 0;
  return 0;
}

int CLZ(instruction *inst) // Clear zero
{
  inst->op_text = "CLZ";
  inst->Z = 0;
  return 0;
}

int CLN(instruction *inst) // Clear negative
{
  inst->op_text = "CLN";
  inst->N = 0;
  return 0;
}

int CCC(instruction *inst) // Clear Condition Codes
{
  inst->op_text = "CCC";
  inst->C = 0;
  inst->V = 0;
  inst->Z = 0;
  inst->N = 0;
  return 0;
}

int SEC(instruction *inst) // Set carry
{
  inst->op_text = "SEC";
  inst->C = 1;
  return 0;
}

int SEV(instruction *inst) // Set overflow
{
  inst->op_text = "SEV";
  inst->V = 1;
  return 0;
}

int SEZ(instruction *inst) // Set zero
{
  inst->op_text = "SEZ";
  inst->Z = 1;
  return 0;
}

int SEN(instruction *inst) // Set negative
{
  inst->op_text = "SEN";
  inst->N = 1;
  return 0;
}

int SCC(instruction *inst) // Set Condition Codes
{
  inst->op_text = "SCC";
  inst->C = 1;
  inst->V = 1;
  inst->Z = 1;
  inst->N = 1;
  return 0;
}

