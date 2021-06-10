#include <iostream>
#include "UK5B_var.h"

int main()
{
	try
	{
	const auto test = new uk5_b_var("test_in_out", "quality");

	const std::string n = test->get_name();
	const int t = test->get_type();
	
//  	const std::string s0 = test->get_string(0);
//  	const std::string s1 = test->get_string(1);
//  	const std::string s2 = test->get_string(2);
//  	const std::string s3 = test->get_string(3);
//  	const std::string s4 = test->get_string(4);
//  	const std::string s5 = test->get_string(5);
//  	const std::string s6 = test->get_string(6);
//  	const std::string s7 = test->get_string(7);
	
	std::cout << n << "|" << t << std::endl;
//	std::cout << s0 << std::endl << s1 << std::endl << s2 << std::endl << s3 << std::endl << s4 << std::endl << s5 << std::endl << s6 << std::endl << s7;

	std::cout << std::get<int>(test->get_value(0)) << std::endl;					// int first
	std::cout << std::get<double>(test->get_value(1)) << std::endl;					// double first
	auto tmp = test->get_value(2);
	for (auto& i : std::get<std::vector<int>>(tmp))								// vector<int> first
		std::cout << i << "|";																		
	std::cout << std::endl;
	auto tmp2 = test->get_value(3);
	for (auto& i : std::get<std::vector<double>>(tmp2))						// vector<double> first
		std::cout << i << "|";																		
	std::cout << std::endl;
	std::cout << std::get<int>(test->get_value(4)) << std::endl;					// int second
	std::cout << std::get<double>(test->get_value(5)) << std::endl;					// double second
	auto tmp3 = test->get_value(6);
	for (auto& i : std::get<std::vector<int>>(tmp3))								// vector<int> second
		std::cout << i << "|";																		
	std::cout << std::endl;
	auto tmp4 = test->get_value(7);
	for (auto& i : std::get<std::vector<double>>(tmp4))						// vector<double> second
		std::cout << i << "|";																		
	std::cout << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

}

