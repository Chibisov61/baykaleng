// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "UK5B_river.h"
#include <iostream>
#include <Windows.h>

int main()  {
//	SetConsoleOutputCP(CP_UTF8);
//	setvbuf(stdout, nullptr, _IOFBF, 1000);
	
	try
	{
		const auto r = new uk5_b_river();
//тело программы
		delete r;
	}
	catch (const std::exception& e)
	{
		std::ofstream f_err;
		f_err.open("error.log", std::ios::out);
		f_err << "Ошибка: " << e.what() << std::endl;
	}
	
	return 0;
}

