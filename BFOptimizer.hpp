#ifndef BFOPTIMIZER_H
#define BFOPTIMIZER_H

#include <map>
#include <string>
#include <stack>
#include "Brackets.hpp"
#include "BFInstruction.hpp"

using namespace std;

// Abstract Factory
class BFOptimizer {
	map<char, BFInstruction*> instructions;

public:
	BFOptimizer(unsigned int* totalBracketsCount, stack<Bracket>* brackets) {
		instructions['>'] = new BFNInstruction();
		instructions['<'] = new BFPInstruction();
		instructions['+'] = new BFIInstruction();
		instructions['-'] = new BFDInstruction();
		instructions['.'] = new BFOInstruction();
		instructions[','] = new BFInInstruction();
		instructions['['] = new BFBOInstruction(totalBracketsCount, brackets);
		instructions[']'] = new BFBCInstruction(totalBracketsCount, brackets);
	}
	~BFOptimizer() {
		;
	}

	BFInstruction* getInstruction(char key) {
		auto it = instructions.find(key);
		if(it != instructions.end()) {
			return it->second;
		}
		string errorInfo = "Instruction not found.";
		throw errorInfo;
	}
};

#endif