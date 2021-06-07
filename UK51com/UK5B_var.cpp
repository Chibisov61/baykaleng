//#include <utility>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>

#include "UK5B_var.h"

UK5B_var::UK5B_var(const std::string& n)
{
	name_ = n;
	const bool x = boost::filesystem::exists("config.ini");

	if (x)
	{
		try
			{
				boost::property_tree::ptree pt;
				read_ini("config.ini", pt);
				place_ = 2;
				for (boost::property_tree::ptree::iterator pos = pt.begin(); pos != pt.end(); ++pos)
				{
					auto f = pos->second.find(name_);
					if (f != pos->second.not_found())
					{
						place_ = m_place[pos->first];
						const auto s = (*f).second.get_value<std::string>();
						int c = 0;
						if (s.find('.')) c += 1;
						if (s.find(';')) c += 2;
						switch (c)
						{
						case 0:								// int				счетчик 
							type_ = 0;							
								
							break;
						case 1:								// double			величина
							type_ = 1;
							
							break;
						case 2:								// vector<int>		размеры в €чейках
							type_ = 2;
							
							break;
						case 3:								// vector<double>	размеры в метрах
							type_ = 2;
							
							break;
						default: ;
						}
						break;
					}
				}
			}
			
		catch (const boost::property_tree::ptree_error &e)
			{
				f_err_.open("error.log", std::ios::out);
				f_err_ << e.what() << std::endl;
			}

	}
	else
	{
		place_ = 2;
	}
}

std::string UK5B_var::get_name() const
{
	return name_;
}

int UK5B_var::get_type() const
{
	return type_;
}

void UK5B_var::set_place(const int p)
{
	place_ = p;
}

int UK5B_var::get_place() const
{
	return place_;
}

void UK5B_var::set_init(bool b)
{
	init_ = b;
}

bool UK5B_var::is_init() const
{
	return init_;
}
