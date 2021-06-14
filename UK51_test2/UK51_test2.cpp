// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "UK5B_river.h"
#include <iostream>
#include <Windows.h>

int main()
{
	SetConsoleOutputCP(CP_UTF8);
	setvbuf(stdout, nullptr, _IOFBF, 1000);
	
	auto r = new uk5_b_river();
	for(auto& v: r->river)
	{
		std::cout << v.get_name() << ":" << v.get_place() << ":" << v.get_type() << " | "  << v.desc << std::endl;
	}
	
}

