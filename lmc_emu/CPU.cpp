#include "CPU.h"

CPU::CPU(std::vector<int> memory, bool debug)
{
	m_memory = memory;
	m_PC = 0;
	m_ACC = 0;
	m_curInstruction = 0;
	m_shouldRun = true;
	m_debug = debug;
}

CPU::~CPU()
{
	//todo
}

bool CPU::run()
{
	while (m_shouldRun)
	{
		m_fetch();
		m_decode();
	}

	std::cout << "CPU has halted! " << std::endl;
	return true;
}

void CPU::m_fetch()
{
	if(m_debug)
		std::cout << "[INFO] Fetch instruction at PC=" << m_PC << std::endl;
	m_curInstruction = m_memory[m_PC];
	m_PC++;
}

void CPU::m_decode()
{
	int instr = m_curInstruction;
	if(m_debug)
		std::cout << "[INFO] Decoding opcode " << instr << std::endl;
	int op = instr / 100;

	switch (op)
	{
	case 1:
		m_add(instr - 100);	//-100 to extract memory address (as instruction is 1xx where xx is mem address)
		break;
	case 2:
		m_sub(instr - 200); 
		break;
	case 3:
		m_store(instr - 300);
		break;
	case 5:
		m_load(instr - 500);
		break;
	case 6:
		m_jmp(instr - 600);
		break;
	case 7:
		m_jmpZero(instr - 700);
		break;
	case 8:
		m_jmpPos(instr - 800);
		break;
	case 9:
		if (instr == 901)
			m_input();
		else
		{
			if (instr == 902)
				m_output();
			else if (instr == 922)
				m_outputChar();
		}
		break;
	case 0:
		m_shouldRun = false;
		break;
	default:
		std::cout << "[FATAL] Invalid opcode at PC=" << m_PC - 1 << "\nHalting.." << std::endl;
		m_shouldRun = false;
	}

	if (m_shouldRun && m_debug)
		std::cout << "Instruction executed successfully; PC=" << m_PC << " ACC=" << m_ACC << std::endl;
}

void CPU::m_add(int addr)
{
	int num = m_memory[addr];
	m_ACC += num;
}

void CPU::m_sub(int addr)
{
	int num = m_memory[addr];
	m_ACC -= num;
}

void CPU::m_store(int addr)
{
	m_memory[addr] = m_ACC;
}

void CPU::m_load(int addr)
{
	m_ACC = m_memory[addr];
}

void CPU::m_jmp(int addr)
{
	m_PC = addr;
}

void CPU::m_jmpZero(int addr)
{
	if (m_ACC <= 0)
		m_PC = addr;
}

void CPU::m_jmpPos(int addr)
{
	if (m_ACC >= 0)
		m_PC = addr;
}

void CPU::m_input()
{
	std::cout << "INPUT Required: ";
	std::cin >> m_ACC;
}

void CPU::m_output()
{
	std::cout << "[OUT] " << m_ACC << std::endl;
}

void CPU::m_outputChar()
{
	std::cout << "[OUT] " << (char)m_ACC << std::endl;
}