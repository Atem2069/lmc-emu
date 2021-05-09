#pragma once

#include<iostream>
#include<vector>
#include<fstream>
#include<string>

inline std::vector<std::string> split(std::string in, char split)	//simple split function :)
{
	std::vector<std::string> res;
	std::string temp = "";
	for (int i = 0; i < in.length(); i++)
	{
		if (in[i] == split)
		{
			res.push_back(temp);
			temp = "";
		}
		else
			temp += in[i];
	}
	res.push_back(temp);
	return res;
}

struct Token
{
	std::string name;
	int address;
};

struct InstructionMapping
{
	std::string mnemonic;
	int opcode;
	bool requireOperand;
};

class Assembler
{
public:
	bool assembleFromFile(std::string file, std::vector<int>& memory);
	bool assembleFromStringData(std::vector<std::string> m_strings, std::vector<int>& memory);

private:
	bool m_resolveOpcode(std::string operation, int& opcode, bool& requiresOperand);
	std::vector<Token> m_knownTokens;	//tokens that we collect from DAT instruction
	std::vector<Token> m_unresolvedTokens;	//tokens we will need to resolve later
	InstructionMapping m_instructionSet[11] = {
		{"ADD",100,true},
		{"SUB",200,true},
		{"STA",300,true},
		{"LDA",500,true},
		{"BRA",600,true},
		{"BRZ",700,true},
		{"BRP",800,true},
		{"INP",901,false},
		{"OUT",902,false},
		{"OTC",922,false},
		{"HLT",000,false}
	};
	const int INSTR_COUNT = 11;
};