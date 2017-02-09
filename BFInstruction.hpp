#ifndef BFINSTRUCTION_H
#define BFINSTRUCTION_H

#include <vector>
#include <string>
#include <stack>
#include "Brackets.hpp"

using namespace std;

// Strategy
class BFInstruction {
protected:
	unsigned countToRepeat;
	char tag;
	bool optimizable;

public:

	BFInstruction() {
		this->countToRepeat = 1;
		this->tag = 0;
		this->optimizable = true;
	}

	void optimalize(int count) {
		this->countToRepeat = count;
	}

	virtual vector<string> getInstructions() {
		;
	}

	char getTag() {
		return this->tag;
	}

	bool isOptimizable() {
		return this->optimizable;
	}
};

class BFNInstruction
	: public BFInstruction
{
public:
	BFNInstruction()
		: BFInstruction()
	{
		this->tag = '>';
	}

	virtual vector<string> getInstructions() {
		vector<string> instructions;
		if(countToRepeat == 1) {
			instructions.push_back("inc ebp");
		} else {
			instructions.push_back(("add ebp, " + to_string(countToRepeat)));
		}
		return instructions;
	}
};

class BFPInstruction
	: public BFInstruction
{
public:
	BFPInstruction()
		: BFInstruction()
	{
		this->tag = '<';
	}

	virtual vector<string> getInstructions() {
		vector<string> instructions;
		if(countToRepeat == 1) {
			instructions.push_back("dec ebp");
		} else {
			instructions.push_back(("sub ebp, " + to_string(countToRepeat)));
		}
		return instructions;
	}
};

class BFIInstruction
	: public BFInstruction
{
public:
	BFIInstruction()
		: BFInstruction()
	{
		this->tag = '+';
	}

	virtual vector<string> getInstructions() {
		vector<string> instructions;
		if(countToRepeat == 1) {
			instructions.push_back("inc byte [ebx + ebp*1]");
		} else {
			instructions.push_back(("add byte [ebx + ebp*1], " + to_string(countToRepeat)));
		}
		return instructions;
	}
};

class BFDInstruction
	: public BFInstruction
{
public:
	BFDInstruction()
		: BFInstruction()
	{
		this->tag = '-';
	}

	virtual vector<string> getInstructions() {
		vector<string> instructions;
		if(countToRepeat == 1) {
			instructions.push_back("dec byte [ebx + ebp*1]");
		} else {
			instructions.push_back(("sub byte [ebx + ebp*1], " + to_string(countToRepeat)));
		}
		return instructions;
	}
};

class BFOInstruction
	: public BFInstruction
{
public:
	BFOInstruction()
		: BFInstruction()
	{
		this->tag = '.';
	}

	virtual vector<string> getInstructions() {
		vector<string> instructions;
		if(countToRepeat == 1) {
			instructions.push_back("mov al, [ebx + ebp*1]");
			instructions.push_back("push eax");
			instructions.push_back("call _putchar");
			instructions.push_back("add esp, 4");
		} else {
			instructions.push_back("mov al, [ebx + ebp*1]");
			instructions.push_back("push eax");
			for(int i=0; i<countToRepeat; i++) {
				instructions.push_back("call _putchar");
			}
			instructions.push_back("add esp, 4");
		}
		return instructions;
	}
};

class BFInInstruction
	: public BFInstruction
{
public:
	BFInInstruction()
		: BFInstruction()
	{
		this->tag = ',';
		this->optimizable = false;
	}

	virtual vector<string> getInstructions() {
		vector<string> instructions;
		instructions.push_back("call _getchar");
		instructions.push_back("mov byte [ebx + ebp*1], al");
		instructions.push_back("mov eax, 0");
		return instructions;
	}
};

class BFBOInstruction
	: public BFInstruction
{
	unsigned int* totalBracketsCount;
	stack<Bracket>* brackets;

public:
	BFBOInstruction(unsigned int* totalBracketsCount, stack<Bracket>* brackets)
		: BFInstruction()
	{
		this->tag = '[';
		this->optimizable = false;
		this->totalBracketsCount = totalBracketsCount;
		this->brackets = brackets;
	}

	virtual vector<string> getInstructions() {
		vector<string> instructions;
		auto startBracketName = "SB" + to_string(*totalBracketsCount);
		auto endBracketName = "EB" + to_string(*totalBracketsCount);
		auto bracket = Bracket{
			startBracketName,
			endBracketName
		};
		brackets->push(bracket);
		instructions.push_back((startBracketName + ":"));
		instructions.push_back("cmp byte [ebx + ebp*1], 0");
		instructions.push_back(("je " + endBracketName));
		*totalBracketsCount = *totalBracketsCount + 1;
		return instructions;
	}
};

class BFBCInstruction
	: public BFInstruction
{
	stack<Bracket>* brackets;

public:
	BFBCInstruction(stack<Bracket>* brackets)
		: BFInstruction()
	{
		this->tag = ']';
		this->optimizable = false;
		this->brackets = brackets;
	}

	virtual vector<string> getInstructions() {
		vector<string> instructions;
		auto bracket = brackets->top();

		instructions.push_back(("jmp " + bracket.startLabel));
		instructions.push_back((bracket.endLabel + ":"));
		brackets->pop();
		return instructions;
	}
};

#endif