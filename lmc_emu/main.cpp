#include<iostream>
#include<vector>

#include"CPU.h"
#include"Assembler.h"


int main(int argc, char** argv)
{
	Assembler m_assembler;
	std::vector<int> m_cpuMemory;
	std::cout << "LMC Emulator." << std::endl;
	if (argc==2)
	{
		std::cout << "Loading from file " << argv[1] << ".. " << std::endl;
		if (!m_assembler.assembleFromFile(argv[1], m_cpuMemory))
			return -1;

	}
	else
	{
		std::cout << "Press ENTER to exit prompt. " << std::endl;
		std::vector<std::string> src;
		bool stop = false;
		while (!stop)
		{
			std::cout << ">";
			std::string line;
			std::getline(std::cin, line);
			if (line.empty())
				stop = true;
			else
				src.push_back(line);
		}
		if (!m_assembler.assembleFromStringData(src, m_cpuMemory))
			return -1;
	}

	CPU m_cpu(m_cpuMemory,true);
	m_cpu.run();
	return 0;
}