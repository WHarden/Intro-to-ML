#include "CPU.hpp"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <vector>
using namespace std;


int main (int argc, char* argv[])
{
	//openfile
	ifstream fin;
	fin.open(argv[1]);
    vector<string> instMem;
	string line;
	while (getline(fin, line)) {
		instMem.push_back(line);
	}

	int maxPC = instMem.size();
	//cout<<maxPC<<endl;


	Instruction instr_curr;

	CPU myCPU;
	string instruction;

	int myClock = 0;
	int myPC = 0;

	bool GO= true;
	while (GO) {
		//hardwire to 0
		myCPU.regFile[0]=0;
		// fetch
		//cout<<"my x3:  "<<myCPU.regFile[3]<<" PC: "<<myPC<<endl;
		//cout<<" my x1: "<<myCPU.regFile[1]<<endl;
		//" next PC "<<new_myPC<<endl;
		instruction = myCPU.Fetch(instMem, myPC);

		// decode
		myCPU.Decode(instruction, instr_curr);

		// execute
		myCPU.Execute(instruction,instr_curr, myPC);

		// memory
		myCPU.Memory(instruction, myCPU.ALU_addr);

		// write-back
		myCPU.WriteBack();
		//cout<< "PC:"<<myPC<<" Clock: "<< myClock<<endl;

		//return new PC
		int new_myPC=myCPU.Addresschange(myPC);

		//myCPU.nowPC=myCPU.PC_next;
        myClock += 1;

        myPC=new_myPC;
		//store

		myCPU.ALUSrc = myCPU.ALUSrc_next;
		myCPU.ALU_result = myCPU.ALU_result_next;
		myCPU.ALU_addr = myCPU.ALU_addr_next;
		myCPU.Mem_Result = myCPU.Mem_Result_next;
        //test
        //cout<<"my x3:  "<<myCPU.regFile[3]<<" "<< "next pc: "<< myPC<<endl;
        //cout<<" my x1: "<<myCPU.regFile[1]<<endl;
        //cout<<"-----------------"<<endl;
		// break out of the loop if there are 5 consecutive nop in the pipeline
        if (new_myPC +4 >maxPC)
        break;

	}
    //cout<<"cycle: " << myClock*5<<endl;
	cout << "(" << myCPU.regFile[10] << "," << myCPU.regFile[11] << ")" << endl;

	return 0;
}
