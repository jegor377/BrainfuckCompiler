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

using namespace std;

struct Bracket {
	string startLabel;
	string endLabel;
};

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
		vector<string> program;
		stack<Bracket> brackets;
		auto totalBracketsCount = 0;

		program.push_back("mov ebx, memory");
		program.push_back("mov ebp, 0");
		for(int i = 0; i < this->code.length(); i++) {
			switch(code[i]) {
			case '>':
				program.push_back("inc ebp");
				break;
			case '<':
				program.push_back("dec ebp");
				break;
			case '+':
				program.push_back("inc byte [ebx + ebp*1]");
				break;
			case '-':
				program.push_back("dec byte [ebx + ebp*1]");
				break;
			case '.':
				program.push_back("mov al, [ebx + ebp*1]");
				program.push_back("push eax");
				program.push_back("call _putchar");
				program.push_back("add esp, 4");
				break;
			case ',':
				program.push_back("call _getchar");
				program.push_back("mov byte [ebx + ebp*1], al");
				program.push_back("mov eax, 0");
				break;
			case '[':
				{
					auto startBracketName = "SB" + to_string(totalBracketsCount);
					auto endBracketName = "EB" + to_string(totalBracketsCount);
					auto bracket = Bracket{
						startBracketName,
						endBracketName
					};
					brackets.push(bracket);
					program.push_back((startBracketName + ":"));
					program.push_back("cmp byte [ebx + ebp*1], 0");
					program.push_back(("je " + endBracketName));
					totalBracketsCount++;
				}
				break;
			case ']':
				{
					auto bracket = brackets.top();
					program.push_back(("jmp " + bracket.startLabel));
					program.push_back((bracket.endLabel + ":"));
					brackets.pop();
				}
				break;
			}
		}

		if(!brackets.empty()) {
			string errorInfo = "Brackets error.";
			throw errorInfo;
		}

		return program;
	}
};

#endif