/*
Copyright 2017 Igor Santarek

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "BFCompiler.hpp"

using namespace std;

int main(int argc, char* argv[]) {
	if(argc != 3) {
		cout<<"Brainfuck compiler 1.0 by Igor Santarek."<<endl;
		cout<<"(c) 2017 Igor Santarek. All rights reserved."<<endl<<endl;
		cout<<"Usage: <input file path> <output file path>"<<endl;
		return 1;
	}

	ifstream ifile(argv[1]);
	ofstream ofile;

	if(ifile.good()) {
		string line;
		string code = "";
		while(getline(ifile, line)) {
			code += line;
		}
		
		ofile.open(argv[2]);

		if(ofile.is_open()) {
			BFCompiler compiler(code.c_str());
			try {
				auto compiledCode = compiler.compile();
				for(const auto line : compiledCode) {
					ofile<<line<<endl;
				}
			} catch(string errorInfo) {
				cout<<errorInfo<<endl;
				return 1;
			}
			ofile.close();
		} else {
			cout<<"Cannot to save file."<<endl;
		}

		ifile.close();
	} else {
		cout<<"Cannot to load file."<<endl;
		return 1;
	}

	return 0;
}