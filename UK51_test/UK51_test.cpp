#include <iostream>
#include "UK5B_var.h"

int main()
{
	try
	{
	const auto test = new uk5_b_var("test_out", "quality");

	const std::string n = test->get_name();
	const int t = test->get_type();
	
	const std::string s0 = test->get_value(0);
	const std::string s1 = test->get_value(1);
	const std::string s2 = test->get_value(2);
	const std::string s3 = test->get_value(3);
	const std::string s4 = test->get_value(4);
	const std::string s5 = test->get_value(5);
	const std::string s6 = test->get_value(6);
	const std::string s7 = test->get_value(7);
	
    std::cout << n << "|" << t << std::endl << s0 << std::endl << s1 << std::endl << s2 << std::endl << s3 << std::endl << s4 << std::endl << s5 << std::endl << s6 << std::endl << s7;
	}
	catch(const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

}

