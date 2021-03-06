#include "header.h"

bool MEM_USED_FLAGS[MEMORY_SPACE]; //dummy define for compilation error
bool BREAK_POINT[MEMORY_SPACE/2];  //dummy define for compilation error
uint16_t REGS[7];
uint8_t MEM[MEMORY_SPACE];
uint16_t& R0 = REGS[0];
uint16_t& R1 = REGS[1];
uint16_t& R2 = REGS[2];
uint16_t& R3 = REGS[3];
uint16_t& R4 = REGS[4];
uint16_t& R5 = REGS[5];
uint16_t& R6 = REGS[6];
uint16_t& R7 = REGS[7];
uint16_t& SP = REGS[6];
uint16_t& PC = REGS[7];
uint16_t starting_pc;
instruction * current_instruction;	//decoded instruction information
int verbosity_level = 0;            //Level of verbosity in print statements
string trace_file;
string data_file;
string branch_trace_file;
PSW_t PSW;

void get_user_octal(string prompt, string error_text, uint16_t &word)
{
//Only here to get rid of a compilation error.
}

void clear_psw(PSW_t & PSW) {
	PSW.PSW_BYTE = 0;
}

void print_psw(PSW_t & PSW) {
	cout << "Global PSW: " << setfill('0') << setw(3) << oct << uint16_t(PSW.PSW_BYTE) << endl;
	cout << "Global PSW.SPL: " << setfill('0') << setw(1) << uint16_t(PSW.SPL) << endl;
	cout << "Global PSW.T: " << setfill('0') << setw(1) << uint16_t(PSW.T) << endl;
	cout << "Global PSW.N: " << setfill('0') << setw(1) << uint16_t(PSW.N) << endl;
	cout << "Global PSW.Z: " << setfill('0') << setw(1) << uint16_t(PSW.Z) << endl;
	cout << "Global PSW.V: " << setfill('0') << setw(1) << uint16_t(PSW.V) << endl;
	cout << "Global PSW.C: " << setfill('0') << setw(1) << uint16_t(PSW.C) << endl;
}


static void printMemReg()
{
  cout << "\n";
  for (int i = 0; i < 20; i+=2)
  {
    printf("address: %u,%u   val: %04x\n", i, i+1, (MEM[i]) | (MEM[i+1] << 8) );
  }

  for (int i = 0; i <= 7; i++)
  {
    printf("Reg: %u   val: %04x\n", i, REGS[i]);
  }
  cout << "\n";
}

static void fillMem(int val)
{
  for (int i = 0; i < MEMORY_SPACE; i++)
  {
    MEM[i] = val;
  }
}

static void clearReg()
{
  for (int i = 0; i <= 7; i++)
    REGS[i] = 0;
}

static void printFlags(instruction * inst)
{
  cout << "OP: " << inst->op_text << endl;
  printf("N: %i\n", inst->N);
  printf("Z: %i\n", inst->Z);
  printf("V: %i\n", inst->V);
  printf("C: %i\n\n", inst->C);
}

static int operationTest()
{
  current_instruction = new instruction;

  // Variables for changing Byte/Word instruction,
  // Addressing mode, and register
  // Single is for single ops, double is for double ops
  // Change the spots with "x" to desired value

  // Byte mode                 x00000
  uint16_t byte_mode   =      0100000;

  // Addr mode                 0000x0
  uint16_t single_mode =      0000010;

  // Register                  00000x
  uint16_t single_reg  =      0000001;

  // Addr mode 1               00x000
  uint16_t double_mode_src =  0001000;

  // Register 1                000x00
  uint16_t double_src =       0000400;

  // Addr mode 2               0000x0
  uint16_t double_mode_dest = 0000010;

  // Register 2                00000x
  uint16_t double_dest =      0000005;

  // Branch offset             0000xx
  uint16_t branch_offset =    0000001;

  //SINGLE OPERAND

  //General
	uint16_t CLR  =  0005000 + byte_mode + single_mode + single_reg; // Clear
	uint16_t COM  =  0005100 + byte_mode + single_mode + single_reg; // 1's Compliment
	uint16_t INC  =  0005200 + byte_mode + single_mode + single_reg; // Increment
	uint16_t DEC  =  0005300 + byte_mode + single_mode + single_reg; // Decrement
	uint16_t NEG  =  0005400 + byte_mode + single_mode + single_reg; // 2's Compliment negate
	uint16_t TST  =  0005700 + byte_mode + single_mode + single_reg; // Test

  //Shift & Rotate
	uint16_t ASR  =  0006200 + byte_mode + single_mode + single_reg; // Arithmetic shift right
	uint16_t ASL  =  0006300 + byte_mode + single_mode + single_reg; // Arithmetic shift left
	uint16_t ROR  =  0006000 + byte_mode + single_mode + single_reg; // Rotate right
	uint16_t ROL  =  0006100 + byte_mode + single_mode + single_reg; // Rotate left
	uint16_t SWAB =  0000300 + single_mode + single_reg; // Swap bytes

  //Multiple Precision
	uint16_t ADC  =  0005500 + byte_mode + single_mode + single_reg; // Add carry
	uint16_t SBC  =  0005600 + byte_mode + single_mode + single_reg; // Subtract carry
	uint16_t SXT  =  0006700 + single_mode + single_reg; // Sign extend

  //DOUBLE OPERAND

  //Logical
	uint16_t BIT  =  0030000 + byte_mode + double_mode_src + double_src + double_mode_dest + double_dest; // Bit test
	uint16_t BIC  =  0040000 + byte_mode + double_mode_src + double_src + double_mode_dest + double_dest; // Bit clear
	uint16_t BIS  =  0050000 + byte_mode + double_mode_src + double_src + double_mode_dest + double_dest; // Bit set
	uint16_t MOV  =  0010000 + byte_mode + double_mode_src + double_src + double_mode_dest + double_dest; // Move source to destination
  uint16_t CMP  =  0020000 + byte_mode + double_mode_src + double_src + double_mode_dest + double_dest; // Compare source to destination
	uint16_t ADD  =  0060000 + double_mode_src + double_src + double_mode_dest + double_dest; // Add source to destination
	uint16_t SUB  =  0160000 + double_mode_src + double_src + double_mode_dest + double_dest; // Subtract source from destination

// Branch
  uint16_t BR   =  0000400 + branch_offset; // Branch unconditional
  uint16_t BNE  =  0001000 + branch_offset; // Branch if not equal (to zero)
  uint16_t BEQ  =  0001400 + branch_offset; // Branch if equal (to zero)
  uint16_t BPL  =  0100000 + branch_offset; // Branch if plus
  uint16_t BMI  =  0100400 + branch_offset; // Branch if minus
  uint16_t BVC  =  0102000 + branch_offset; // Branch if overflow is clear
  uint16_t BVS  =  0102400 + branch_offset; // Branch if overflow is set
  uint16_t BCC  =  0103000 + branch_offset; // Branch if carry is clear
  uint16_t BCS  =  0103400 + branch_offset; // Branch if carry is set

// Signed Conditional Branch
  uint16_t BGE  =  0002000 + branch_offset; // Branch if greater than or equal (to zero)
  uint16_t BLT  =  0002400 + branch_offset; // Branch if less than (zero)
  uint16_t BGT  =  0003000 + branch_offset; // Branch if greater than (zero)
  uint16_t BLE  =  0003400 + branch_offset; // Branch if less than or equal (to zero)
  uint16_t SOB  =  0077000 + branch_offset; // Subtract one and branch (if not = 0)

// Unsigned Conditional Branch
  uint16_t BHI  =  0101000 + branch_offset; // Branch if higher
  uint16_t BLOS =  0101400 + branch_offset; // Branch if lower or same
  uint16_t BHIS =  0103000 + branch_offset; // Branch if higher or same (same as BCC)
  uint16_t BLO  =  0103400 + branch_offset; // Branch if lower (same as BCS)

  uint16_t JMP  =  0000100 + 022; // Jump
  uint16_t JSR  =  0004000 + 0311; // Jump to subroutine
  uint16_t RTS  =  0000200 + 03; // Return from subroutine

  uint16_t single_ops_all[14] = {CLR, COM, INC, DEC, NEG, TST, ASR, ASL, ROR, ROL, SWAB, ADC, SBC, SXT};
  uint16_t single_ops_general[6] = {CLR, COM, INC, DEC, NEG, TST};
  uint16_t single_ops_snr[5] = {ASR, ASL, ROR, ROL, SWAB};
  uint16_t single_ops_mp[3] = {ADC, SBC, SXT};

  string single_all[14] = {"CLR", "COM", "INC", "DEC", "NEG", "TST", "ASR", "ASL", "ROR", "ROL", "SWAB", "ADC", "SBC", "SXT"};
  string single_general[6] = {"CLR", "COM", "INC", "DEC", "NEG", "TST"};
  string single_snr[5] = {"ASR", "ASL", "ROR", "ROL", "SWAB"};
  string single_mp[3] = {"ADC", "SBC", "SXT"};

  uint16_t double_ops_all[7] = {BIT, BIC, BIS, MOV, CMP, ADD, SUB};

  string double_all[7] = {"BIT", "BIC", "BIS", "MOV", "CMP", "ADD", "SUB"};

  uint16_t branch_ops_all[18] = {BR, BNE, BEQ, BPL, BMI, BVC, BVS, BCC, BCS, BGE, BLT, BGT, BLE, SOB, BHI, BLOS, BHIS, BLO};

  uint16_t test_branch[3] = {JMP, JSR, RTS};

  int runs = 7; // Number of runs for the loop

    int res;
    int val;
    int test;

    fillMem(0);

    val = 0;
    res = clearInstruction(current_instruction);

    // Change the number of runs(above), the string output, and value outputs for the loop
///////////////////////////////////////////////////////////
  /*for (int i = 0; i < runs; i++)
  {
    cout << "_________________\n";
    cout << endl; // CHANGE HERE
    //printf("code: %u\n", double_ops_all[i]);
    fillMem(0);
    res = clearInstruction(current_instruction);
    res = parseInstruction(double_ops_all[i], current_instruction); // CHANGE HERE
    res = printInstruction(current_instruction);

    R0 = 0;
    R1 = 0;
    R2 = 0;
    R3 = 0;
    R4 = 2;
    R5 = 4;
    R6 = 0;
    R7 = 0;

    MEM[0] = 0x00;
    MEM[1] = 0x00;
    MEM[2] = 0x80;
    MEM[3] = 0x00;
    MEM[4] = 0x00;
    MEM[5] = 0x00;
    MEM[6] = 0x00;
    MEM[7] = 0x00;
    MEM[8] = 0x00;
    MEM[9] = 0x00;
    MEM[10] = 0x00;
    MEM[11] = 0x00;
    MEM[12] = 0x00;
    MEM[13] = 0x00;
    MEM[14] = 0x00;
    MEM[15] = 0x00;
    MEM[16] = 0x00;
    MEM[17] = 0x00;
    MEM[18] = 0x00;
    MEM[19] = 0x00;

    printMemReg();
    printFlags(current_instruction);

    test = dispatch(current_instruction);

    printMemReg();
    printFlags(current_instruction);
    //clearReg();
  }*/

    cout << "_________________\n";
    cout << "i"  << endl; // CHANGE HERE
    //printf("code: %d\n", i);

    fillMem(0);


    res = clearInstruction(current_instruction);
    //current_instruction->N = 1;
    //current_instruction->Z = 1;
    //current_instruction->V = 1;
    //current_instruction->C = 1;
    res = parseInstruction(JSR, current_instruction); // CHANGE HERE
    res = printInstruction(current_instruction);

    R1 = 10;
    R2 = 0;
    R3 = 6;
    R6 = 10;
    R7 = 0;

    MEM[6] = 0xF0;
    MEM[10] = 0xCF;
    MEM[11] = 0x00;
    MEM[12] = 0x14;
    MEM[13] = 0x00;
    MEM[14] = 0xFF;
    MEM[15] = 0xFF;

    printMemReg();
    printFlags(current_instruction);

    test = dispatch(current_instruction);

    printMemReg();
    printFlags(current_instruction);
    //clearReg();

    res = clearInstruction(current_instruction);
    //current_instruction->N = 1;
    //current_instruction->Z = 1;
    //current_instruction->V = 1;
    //current_instruction->C = 1;
    res = parseInstruction(RTS, current_instruction); // CHANGE HERE
    res = printInstruction(current_instruction);
    test = dispatch(current_instruction);

    printMemReg();
    printFlags(current_instruction);

///////////////////////////////////////////////////////////
}

int main()
{
  int result = 0;

  result = operationTest();
  return 1;
}
