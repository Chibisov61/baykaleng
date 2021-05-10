// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <iostream>
#include <boost/filesystem.hpp>
#include "UK5B_Var.h"

using namespace std;
using namespace boost::filesystem;

auto main() -> int  //NOLINT
{

	UK5B_varD test;
<<<<<<< HEAD
	UK5B_varVI test2;
=======
	UK5B_varVD test2;
>>>>>>> fa77c25b74643853fa49c8d8614903b4253493c7
	
	test.UK5B_setName("t2");
	test.UK5B_setValue(exists("config.ini"), 50);

	std::cout << test.UK5B_getValue() << " " << test.UK5B_getPlace() << endl;

	test2.UK5B_setName("t3");
	test2.UK5B_setValue(true, {});
	

	auto v = test2.UK5B_getValue();
<<<<<<< HEAD
	for (int& i : v)
		std::cout << i << ' ';
=======
	for (auto i = v.begin(); i != v.end(); ++i)
		std::cout << *i << ' ';
>>>>>>> fa77c25b74643853fa49c8d8614903b4253493c7
	std::cout << ":" << test2.UK5B_getPlace() << endl;

}	