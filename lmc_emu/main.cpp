#include<iostream>
#include<vector>

#include"CPU.h"
#include"Assembler.h"

std::vector<std::string> src = {
	"INP",
	"STA NUM1",
	"INP",
	"STA NUM2",
	"LOOP LDA TOTAL",
	"ADD NUM1",
	"STA TOTAL",
	"LDA NUM2",
	"SUB ONE",
	"STA NUM2",
	"BRP LOOP",
	"LDA TOTAL",
	"SUB NUM1",
	"STA TOTAL",
	"OUT",
	"HLT",
	"NUM1 DAT 0",
	"NUM2 DAT 0",
	"ONE DAT 1",
	"TOTAL DAT 0"
};

int main()
{
	Assembler m_assembler;
	std::vector<int> m_cpuMemory;
	if (m_assembler.assembleFromStringData(src, m_cpuMemory))
	{
		for (int i = 0; i < m_cpuMemory.size(); i++)
			std::cout << m_cpuMemory[i] << " ";
		std::cout << std::endl;
	}
	else
		return -1;

	CPU m_cpu(m_cpuMemory);
	m_cpu.run();
	return 0;
}