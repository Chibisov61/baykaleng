// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "UK5B_river.h"
#include <iostream>
#include <Windows.h>

int main()  // NOLINT(bugprone-exception-escape)
{
//	SetConsoleOutputCP(CP_UTF8);
//	setvbuf(stdout, nullptr, _IOFBF, 1000);
	
	auto r = new uk5_b_river();
	for(auto& v: r->river)
	{
		for (int j = 1; j < v.number; j++)
		{
			if (!v.max.empty())
				if (int max = 0; r->river.at(j).get_name() == v.max)
					max = std::get<int>(r->river.at(j).get_value(0));
			
			if (!v.delta.empty())
				if (r->river.at(j).get_name() == v.delta)
				{
					const double delta = std::get<double>(r->river.at(j).get_value(1));
					v.set_delta(delta);
				}
			
			if (!v.shift.empty())
				if (r->river.at(j).get_name() == v.shift)
				{
					const double shift = std::get<double>(r->river.at(j).get_value(1));
					v.set_shift(shift);
				}
//
//	осталось  "def" из "param"  и "с". потом set_value(def,max,c)
//			
		}
	}
	return 0;
}

