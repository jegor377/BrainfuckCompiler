/*
Copyright 2017 Igor Santarek

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef BFCOMPILER_H
#define BFCOMPILER_H

#include <vector>
#include <string>
#include <stack>
#include "Brackets.hpp"
#include "BFOptimizer.hpp"
#include "BFInstruction.hpp"

using namespace std;

class BFCompiler {
	string code;

public:
	BFCompiler(const string code) {
		this->code = code;
	}

	vector<string> compile() {
		vector<string> resultCode;
		vector<string> entryCode;
		vector<string> programCode;

		entryCode = createEntry();
		programCode = createProgram();
		resultCode.insert(end(resultCode), begin(entryCode), end(entryCode));
		resultCode.insert(end(resultCode), begin(programCode), end(programCode));

		return resultCode;
	}
private:
	vector<string> createEntry() {
		vector<string> entry;
		entry.push_back("[bits 32]\n");
		entry.push_back("extern _putchar, _getchar, _exit\n");
		entry.push_back("section .bss");
		entry.push_back("memory:");
		entry.push_back(" resb 30000\n");
		entry.push_back("section .text");
		entry.push_back("_WinMain@16:");
		return entry;
	}

	vector<string> createProgram() {
		vector<BFInstruction*> program;
		vector<string> retCode;
		stack<Bracket> brackets;
		unsigned int totalBracketsCount = 0;
		BFOptimizer optimizer(&totalBracketsCount, &brackets);

		retCode.push_back("mov ebx, memory");
		retCode.push_back("mov ebp, 0");
		for(int i = 0; i < this->code.length(); i++) {
			try {
				program.push_back( optimizer.getInstruction(this->code[i]) );
			} catch(string errInfo) {
				;
			}
		}

		auto optimalizedCode = optimalizeCode(program);
		retCode.insert(end(retCode), begin(optimalizedCode), end(optimalizedCode));

		if(!brackets.empty()) {
			string errorInfo = "Non paired brackets.";
			throw errorInfo;
		}

		return retCode;
	}

	vector<string> optimalizeCode(vector<BFInstruction*> instructions) {
		vector<string> code;
		vector<BFInstruction*> optimalizedInstructions;

		int i=0;
		while(i<instructions.size()) {
			auto instruction = instructions[i];
			auto offset = getInstructionCount(instructions, i);
			instruction->optimalize(offset);
			auto instructionCode = instruction->getInstructions();
			code.insert(end(code), begin(instructionCode), end(instructionCode));
			i += offset;
		}

		return code;
	}

	int getInstructionCount(vector<BFInstruction*> instructions, int id) {
		auto instruction = instructions[id];
		if((id + 1) < instructions.size()) {
			if(instruction->isOptimizable()) {
				if(instructions[id+1]->getTag() == instruction->getTag()) {
					return getInstructionCount(instructions, id + 1) + 1;
				} else {
					return 1;
				}
			} else {
				return 1;
			}
		} else {
			return 1;
		}
	}
};

#endif