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

class Assembler
{
public:
	bool assembleFromFile(std::string file, std::vector<int>& memory);
	bool assembleFromStringData(std::vector<std::string> m_strings, std::vector<int>& memory);

private:
	bool m_resolveOpcode(std::string operation, int& opcode, bool& requiresOperand);
	std::vector<Token> m_knownTokens;	//tokens that we collect from DAT instruction
	std::vector<Token> m_unresolvedTokens;	//tokens we will need to resolve later
};