#pragma once

#include<iostream>
#include<vector>

class CPU
{
public:
	CPU(std::vector<int> memory, bool debug);
	~CPU();

	bool run();
private:
	void m_fetch();
	void m_decode();
	//all instructions that must be implemented
	void m_add(int addr);
	void m_sub(int addr);
	void m_store(int addr);
	void m_load(int addr);
	void m_jmp(int addr);
	void m_jmpZero(int addr);
	void m_jmpPos(int addr);
	void m_input();
	void m_output();
	void m_outputChar();
	//and register stuff
	int m_PC;
	int m_ACC;
	int m_curInstruction;

	std::vector<int> m_memory;

	bool m_shouldRun;
	bool m_debug;
};