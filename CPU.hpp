#include <iostream>
#include <string>
#include <vector>
#include <bitset>
using namespace std;

#ifndef CPU_CLASS
#define CPU_CLASS
class Instruction {
    public:
        Instruction() {}
        string opcode;
        string rd;
        string rs1;
        string rs2;
        string funct3;
        string funct7;
        string immediate;
};
class CPU {
    public:
        // constructor for CPU
        CPU() {
        	//operation
            ALUOp = "";
            //ALUsource
            ALUSrc = 0;
            ALUSrc_next=0;
            address_result=0;
            //rs1,rs2,immediate
            rs1 = 0;
            rs2 = 0;
            rd = 0;
            imm = 0;
            rs1_next = 0;
            rs2_next = 0;
            rd_next = 0;
            imm_next = 0;
            //Results of ALU function
            ALU_result = 0;
            ALU_result_next = 0;
            ALU_addr = 0;
            ALU_addr_next = 0;
            // memory
            Mem_Result = 0;
            Mem_Result_next = 0;
            //type of instruction
            type = "";
            for (int i = 0; i < 31; i++) {
                regFile[i] = 0;
            }
        }
        //definition

        int regFile[32];
        string ALUOp;
        int rs1, rs2, rd, imm, address_result,rs1_next,rs2_next,rd_next,imm_next,ALU_addr, ALU_addr_next,
		Mem_Result, Mem_Result_next,ALU_result, ALU_result_next,ALUSrc_next;
        bool ALUSrc;
        string type;
        string Fetch(vector<string> instMem, int myPC);
        void Decode(string instr, Instruction instruction);
        int str_bin_to_decimal(string s);
        void Execute(string instr, Instruction instruction, int myPC);
        void Memory(string instr, int ALU_addr);
        void WriteBack();
        int Addresschange(int myPC);

        // data memory
        // memory read and write
        bitset<8> memory[1024];
        int ReadMem(int addr);
        void WriteMem(int addr);

};

#endif
