#include "CPU.hpp"
#include<cmath>


// Fetch the instruction
string CPU::Fetch(vector<string> instMem, int myPC) {

    // get the 4 bytes that make up an instruction based on the PC
    string str_B1 = instMem[myPC];
    string str_B2 = instMem[myPC+1];
    string str_B3 = instMem[myPC+2];
    string str_B4 = instMem[myPC+3];

    // convert string to int
    int int_B1 = stoi(str_B1);
    int int_B2 = stoi(str_B2);
    int int_B3 = stoi(str_B3);
    int int_B4 = stoi(str_B4);

    // int to binary
    bitset<8> binary_B1(int_B1);
    bitset<8> binary_B2(int_B2);
    bitset<8> binary_B3(int_B3);
    bitset<8> binary_B4(int_B4);


    // concatenate the strings to create the instruction
    string instr = binary_B4.to_string() + binary_B3.to_string() + binary_B2.to_string() + binary_B1.to_string();

    return instr;
}


// helper function to convert a binary string to a decimal
int CPU::str_bin_to_decimal(string s) {
    int decimal = 0;
    int len = s.length();
    for (int i = 0; i < len; i++) {
        if (s[i] == '1') {
            decimal += pow(2, len-i-1);
        }
    }
    return decimal;
}

//Decoding
void CPU::Decode(string instr, Instruction instruction) {
	Instruction type_instr;
	// extract opcode to decide the type of instruction
	type_instr.opcode = instr.substr(25,7);


	    // R-type
	    if (type_instr.opcode == "0110011") {

	        type = "R";
	    }

	    // I-type
	    if (type_instr.opcode== "0010011") {

	        type = "I";
	    }

	    //B-type
	    if (type_instr.opcode == "1100011") {

	            type = "B";
	        }

	    // LW
	    if (type_instr.opcode == "0000011") {
	        type = "LW";
	    }

	    // SW
	    if (type_instr.opcode == "0100011") {

	        type = "SW";
	    }

	    //JALR
	    if (type_instr.opcode == "1100111") {

	            type = "JALR";
	        }

    instruction.opcode = instr.substr(25,7);

    // extract funct3
    instruction.funct3 = instr.substr(17,3);

    // extract rs1
    instruction.rs1 = instr.substr(12,5);

    // extract rs2
    instruction.rs2 = instr.substr(7,5);

    // extract funct7
    instruction.funct7 = instr.substr(0,7);

    // extract rd
    instruction.rd = instr.substr(20,5);

    // R-type
    if (type == "R") {
        // ADD
        if (instruction.funct3 == "000" && instruction.funct7 == "0000000") {
        	ALUOp = "ADD";
        }
        // SUB
        else if (instruction.funct3 == "000" && instruction.funct7 == "0100000") {
        	ALUOp = "SUB";
        }
        else if (instruction.funct3 == "100") {
        	ALUOp = "XOR";
        }
        else if (instruction.funct3 == "101") {
        	ALUOp = "SRA";
        }
    }

    // I-type
    if (type == "I") {

        instruction.immediate = instr.substr(0, 12);
        // I-type instructions do not use rs2
        instruction.rs2 = "00000";
        // ADDI
        if (instruction.funct3 == "000") {
        	ALUOp = "ADDI";
        }
        // ORI
        if (instruction.funct3 == "110") {
        	ALUOp = "ORI";
        }
        // ANDI
        if (instruction.funct3 == "111") {
        	ALUOp = "ANDI";
        }
    }
    if (type == "B"){
    	ALUOp="BLT";
    	// left shirt one and plau a "0" in the end
    	instruction.immediate = instr.substr(24,1) +
    			instr.substr(1,6) + instr.substr(20,4)+"0";
    	//test

    	//int test=str_bin_to_decimal(instruction.immediate);
    	//cout<<"!!!!!!!!!!!!!!!"<<instr<<endl;
    	//cout<<"IMM"<<instruction.immediate<<"!!!!!!!BLT IMM  "<< test<<endl;


    }
    if (type =="JALR"){
    	// extract imm for JALR
    	instruction.immediate = instr.substr(0,12);
    	ALUOp="JALR";

    }
    // LW
    if (type == "LW") {
        // immediate values
    	//rs2=0 in lw instruction
        instruction.immediate = instr.substr(0, 12);
        instruction.rs2 = "00000";
        ALUOp = "LW";
    }

    // SW
    //rd=0 in sw instruction
    if (type == "SW") {
        // immediate values
        instruction.immediate = instr.substr(0, 7) + instr.substr(20,5);
        instruction.rd = "00000";
        ALUOp = "SW";
    }

    // read rs1 rs2 rd
    // actually _next represents the current value in this stage
    rs1_next = str_bin_to_decimal(instruction.rs1);
    rs2_next = str_bin_to_decimal(instruction.rs2);
    rd_next = str_bin_to_decimal(instruction.rd);

    // read positive immediate
    if (instruction.immediate[0] != '1') {
        imm_next = str_bin_to_decimal(instruction.immediate);
    }
    // read negative immediate
    else {
        int l = instruction.immediate.length();
        for (int i = 0; i < l; i++) {
            if (i == 0)
                imm_next = pow(2, l-1)*(-1);
            if (instruction.immediate[i] == '1' && i != 0) {
                imm_next += pow(2, l-i-1);
            }
        }
    }



}

//Execute
void CPU::Execute(string instr, Instruction instruction, int myPC) {
   //execute alu result for each instruction
   if (ALUOp == "ADD") {
        ALU_result_next = regFile[rs1_next] + regFile[rs2_next];


    }
    if (ALUOp == "SUB") {
        ALU_result_next = regFile[rs1_next] - regFile[rs2_next];

    }
    if (ALUOp == "XOR") {
        ALU_result_next = regFile[rs1_next] ^ regFile[rs2_next];
        }
    if (ALUOp == "SRA") {
          ALU_result_next = regFile[rs1_next] >> regFile[rs2_next];
          }
    if (ALUOp == "ADDI") {
        ALU_result_next = regFile[rs1_next] + imm_next;
        /*
        cout<<"rs1"<<regFile[rs1]<<endl;
        cout<<"imm"<<imm<<endl;
        cout<<"rd"<<ALU_result_next<<endl;
        */
    }
    if (ALUOp == "ORI") {
        ALU_result_next = regFile[rs1_next] | imm_next;
    }
    if (ALUOp == "ANDI") {
        ALU_result_next = regFile[rs1_next] & imm_next;
    }
    // compute address
    if (ALUOp == "LW") {
        ALU_addr_next = regFile[rs1_next] + imm_next;
    }
    if (ALUOp == "SW") {
        ALU_addr_next = regFile[rs1_next] + imm_next;
    }
    if (ALUOp == "JALR") {
    	ALU_result_next = myPC + 4;

    }

}

//memory
void CPU::Memory(string instr, int ALU_addr) {
    // load instruction
    if (ALUOp == "LW") {
        Mem_Result_next = ReadMem(ALU_addr_next);
    }
    // store instruction
    if (ALUOp == "SW") {
        WriteMem(ALU_addr_next);
    }
}

int CPU::ReadMem(int addr) {
    string binary_memory = memory[addr+3].to_string()+ memory[addr+2].to_string()+ memory[addr+1].to_string() + memory[addr].to_string();
    //convert to decimal
    return str_bin_to_decimal(binary_memory);
}

//writememory
void CPU::WriteMem(int addr) {
    // obtain value to be written
    int w_val = regFile[rs2_next];

    bitset<32> binary_w_val(w_val);

    string str_w_val = binary_w_val.to_string();

    // string write value
    string str_B1 = str_w_val.substr(24,8);
    string str_B2 = str_w_val.substr(16,8);
    string str_B3 = str_w_val.substr(8,8);
    string str_B4 = str_w_val.substr(0,8);

    // convert the string to int
    int int_B1 = str_bin_to_decimal(str_B1);
    int int_B2 = str_bin_to_decimal(str_B2);
    int int_B3 = str_bin_to_decimal(str_B3);
    int int_B4 = str_bin_to_decimal(str_B4);

    // convert the int values to binary
    bitset<8> binary_B1(int_B1);
    bitset<8> binary_B2(int_B2);
    bitset<8> binary_B3(int_B3);
    bitset<8> binary_B4(int_B4);

    // store the appropriate bytes into memory
    memory[addr] = binary_B1;
    memory[addr+1] = binary_B2;
    memory[addr+2] = binary_B3;
    memory[addr+3] = binary_B4;
}

//writeback stage
void CPU::WriteBack() {
    // write to rd
    if (ALUOp == "ADD" || ALUOp == "SUB"
        || ALUOp == "ADDI" || ALUOp == "ORI" || ALUOp == "ANDI" || ALUOp
		== "XOR" || ALUOp == "SRA"|| ALUOp == "JALR") {
        regFile[rd_next] = ALU_result_next;
    }
    // LW
    if (ALUOp == "LW") {
        regFile[rd_next] = Mem_Result_next;
    }
}

//  return PC
int CPU::Addresschange(int myPC){
	int new_PC;
    bool branch = regFile[rs1_next] < regFile[rs2_next];
	if(ALUOp=="BLT" && branch){

		new_PC = myPC + imm_next;
		//cout<<"enter blt "<< "imm_next = " << imm_next
		//		<<" !!!next pc fpr blt is " << new_PC<<endl;
	}
	else if (ALUOp == "JALR"){
		//cout<<"enter JAL***^^^^^^" <<" , rs1= "<<regFile[rs1_next]<<" imm= " << imm_next<<endl;
		new_PC = regFile[rs1_next] + imm_next ;

	}
	// test purpose
	//else if(ALUOp == "ADDI"){
	//	new_PC=myPC+4;
	//}

	else{
		new_PC = myPC + 4;
	}
	return new_PC;
}
