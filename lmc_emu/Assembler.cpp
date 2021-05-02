#include"Assembler.h"

//first read each file line by line
//then split each line
//if the resulting vector is 3 items, then the first must be a label
//if so, add to list of known tokens
//then, we work on next two items
//first is instruction. we map it to known opcodes (100,200,etc.)
//then we decode OPERAND
//if it is not known then we add to unknown tokens
//once we have finished reading over we have to fix up unknown tokens
//we go through each token one by one and resolve the token, i.e. if it is found we just add whatever is in memory at the real token's location to the location specified
//then we keep a count of all the tokens we have fixed
//if any still remain then obviously there is an issue
//also obviously same for instruction - if instruction name is some complete bs lol (like 'hello') then instantly throw an error and exit
bool Assembler::assembleFromFile(std::string file, std::vector<int>& memory)
{
	std::vector<int> m_mem;
	std::vector<std::string> strList;
	std::ifstream inFile(file);
	if (!inFile)
	{
		std::cout << "[FATAL][IO] Failed to open file for assembly.. " << std::endl;
		return false;
	}
	for (std::string line; std::getline(inFile, line);)
	{
		strList.push_back(line);
	}

	inFile.close();

	if (assembleFromStringData(strList, m_mem))
	{
		memory = m_mem;
		return true;
	}
	else
	{
		std::cout << "[FATAL][ASM] Assembly from file failed. " << std::endl;
		return false;
	}
}

bool Assembler::assembleFromStringData(std::vector<std::string> m_strings, std::vector<int>& memory)
{
	std::vector<int> m_tempMemory;
	for (int i = 0; i < 100; i++)
		m_tempMemory.push_back(0);
	for (int i = 0; i < m_strings.size(); i++)
	{
		int opcodeIndex = 0, operandIndex = 1;
		std::string currentLine = m_strings[i];
		std::vector<std::string> splitData = split(currentLine, ' ');
		if (splitData.size() == 3 || (splitData.size()==2 && ( splitData[1]=="INP" || splitData[1]=="OUT" || splitData[1]=="HLT" ) ))	//TERRIBLE HACK if instruction has no operand
		{
			Token m_newToken = {};
			m_newToken.name = splitData[0];
			m_newToken.address = i;	//each line is a single instruction, so this should work?
			m_knownTokens.push_back(m_newToken);
			opcodeIndex++; operandIndex++;
		}

		std::string operation = splitData[opcodeIndex];
		if (operation == "DAT")
		{
			int val = std::stoi(splitData[operandIndex]);
			m_tempMemory[i] = val;
		}
		else
		{
			int opcode = 0;
			bool resolveOperand = false;
			if (m_resolveOpcode(operation, opcode,resolveOperand))
			{
				m_tempMemory[i] = opcode;
				if (resolveOperand)
				{
					std::string operand = splitData[operandIndex];
					if (operand.find_first_not_of("-0123456789")==std::string::npos)
					{
						int addr = std::stoi(operand);
						if (addr >= 100 || addr < 0)
						{
							std::cout << "[FATAL][ASM] Operand out of range (" << addr << "), line " << i + 1 << std::endl;
							return false;
						}
						m_tempMemory[i] += addr;
					}
					else
					{
						Token m_newUnresolvedToken = {};	//token which we must resolve later
						m_newUnresolvedToken.name = operand;
						m_newUnresolvedToken.address = i;
						m_unresolvedTokens.push_back(m_newUnresolvedToken);
					}
				}
			}
			else
			{
				std::cout << "[FATAL][ASM] Error assembling line " << i + 1 << std::endl;
				return false;
			}
		}
	}

	//now we must resolve all tokens
	for (int i = 0; i < m_unresolvedTokens.size(); i++)
	{
		Token currentToken = m_unresolvedTokens[i];
		bool found = false;
		for (int j = 0; j < m_knownTokens.size(); j++)
		{
			Token knownToken = m_knownTokens[j];
			if (currentToken.name == knownToken.name)
			{
				found = true;
				int addr = currentToken.address;
				m_tempMemory[addr] += knownToken.address;	//unresolved token address points to instruction, known token address points to where its stored. so just add together to form completed instruction
			}
		}
		if (!found)
		{
			std::cout << "[FATAL][TOKEN] Invalid label name '" << currentToken.name << "' was used on line " << currentToken.address + 1 << std::endl;
			return false;
		}
	}

	memory = m_tempMemory;
	std::cout << "[INFO][ASM] Assembled with no errors. " << std::endl;
	return true;
}

bool Assembler::m_resolveOpcode(std::string operation, int& opcode, bool& resolveOperand)
{
	int m_tempOpcode = 0;
	resolveOperand = true;
	if (operation == "ADD")
		m_tempOpcode = 100;
	else if (operation == "SUB")
		m_tempOpcode = 200;
	else if (operation == "STA")
		m_tempOpcode = 300;
	else if (operation == "LDA")
		m_tempOpcode = 500;
	else if (operation == "BRA")
		m_tempOpcode = 600;
	else if (operation == "BRZ")
		m_tempOpcode = 700;
	else if (operation == "BRP")
		m_tempOpcode = 800;
	else if (operation == "INP")
	{
		m_tempOpcode = 901;
		resolveOperand = false;
	}
	else if (operation == "OUT")
	{
		m_tempOpcode = 902;
		resolveOperand = false;
	}
	else if (operation == "HLT")
	{
		m_tempOpcode = 0;
		resolveOperand = false;
	}
	else
	{
		std::cout << "[FATAL][ASM] Invalid opcode '" << operation << "' was passed. " << std::endl;
		return false;
	}
	opcode = m_tempOpcode;
	return true;
}