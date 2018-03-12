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
int verbosity_level;            //Level of verbosity in print statements
string trace_file;
string data_file;

void get_user_octal(string prompt, string error_text, uint16_t &word)
{
//Only here to get rid of a compilation error.
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
    printf("Reg: %u   val: %u\n", i, REGS[i]);
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

static int operationTest()
{
  current_instruction = new instruction;

  // Variables for changing Byte/Word instruction,
  // Addressing mode, and register
  // Single is for single ops, double is for double ops
  // Change the spots with "x" to desired value

  // Byte mode                 x00000
  uint16_t byte_mode   =      0000000;

  // Addr mode                 0000x0
  uint16_t single_mode =      0000000;

  // Register                  00000x
  uint16_t single_reg  =      0000001;

  // Addr mode 1               00x000
  uint16_t double_mode_src =  0000000;

  // Register 1                000x00
  uint16_t double_src =       0000100;

  // Addr mode 2               0000x0
  uint16_t double_mode_dest = 0000000;

  // Register 2                00000x
  uint16_t double_dest =      0000002;

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

  int runs = 1; // Number of runs for the loop

// Change the number of runs(above), the string output, and value outputs for the loop
///////////////////////////////////////////////////////////
  for (int i = 0; i < runs; i++)
  {
    cout << "_________________\n";
    cout << "MOV"  << " R1,R2\n"; // CHANGE HERE
    int res;
    int val;
    int test;

    fillMem(0);

    val = 0;
    res = clearInstruction(current_instruction);
    res = parseInstruction(MOV, current_instruction); // CHANGE HERE
    res = printInstruction(current_instruction);

    R1 = 10;
    R2 = 14;

    MEM[10] = 4;
    MEM[14] = 3;

    printMemReg();

    test = dispatch(current_instruction);

    printMemReg();
    clearReg();
  }
///////////////////////////////////////////////////////////
}

int main()
{
  int result = 0;

  result = operationTest();
  return 1;
}
