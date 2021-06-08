//#include <utility>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>

#include "UK5B_var.h"

uk5_b_var::uk5_b_var(const std::string& n, const std::string& t, const std::string& def, const double delta)
{
	name_ = n;
	type_ = m_type.at(t);
	
	place_ = 2;
	std::string s = def;
	
	const bool x = boost::filesystem::exists("config.ini");
	if (x)
	{
		try
			{
				boost::property_tree::ptree pt;
				read_ini("config.ini", pt);
				for (boost::property_tree::ptree::iterator pos = pt.begin(); pos != pt.end(); ++pos)
				{
					auto f = pos->second.find(name_);
					if (f != pos->second.not_found())
					{
						place_ = m_place[pos->first];
						s = (*f).second.get_value<std::string>();
						break;
					}
				}
			}
			
		catch (const boost::property_tree::ptree_error &e)
			{
				f_err_.open("error.log", std::ios::out);
				f_err_ << "Ошибка: " << e.what() << std::endl;
			}

	}
	set_value(s, delta);
}

std::string uk5_b_var::get_name() const
{
	return name_;
}

int uk5_b_var::get_type() const
{
	return type_;
}

void uk5_b_var::set_place(const int p)
{
	place_ = p;
}

int uk5_b_var::get_place() const
{
	return place_;
}

void uk5_b_var::set_init(bool b)
{
	init_ = b;
}

bool uk5_b_var::is_init() const
{
	return init_;
}

void uk5_b_var::set_value(const std::string& def, double delta)
{
	int c = 0;
	if (boost::algorithm::contains(def, ".")) c += 1;
	if (boost::algorithm::contains(def, ";")) c += 2;
	const auto s = (def.empty()) ? "0" : def;
	boost::char_separator<char> sep(";");
	boost::tokenizer< boost::char_separator<char> > list(s, sep);
	try
	{
		switch (c)
		{
		case 0:								// int				счетчик 
			((place_ != 1) ? value_d_.first : value_d_.second) = stod(s);
			break;
		case 1:								// double			величина
			((place_ != 1) ? value_i_.first : value_i_.second) = stoi(s);
			break;
		case 2:								// vector<int>		размеры в ячейках
			int end_i = 1;
			for (auto& itr : list)
			{
					if (boost::algorithm::contains(itr, ":"))
					{
						boost::trim_if(itr, is_brackets);
						boost::char_separator<char> sep2(":");
						boost::tokenizer< boost::char_separator<char> > list2(itr, sep2);
						auto it = list2.begin();
						const int first = (std::stoi(*it) == 0) ? end_i : std::stoi(*it);
						const int cc = (std::stoi(*(++it)) == 0) ? 1 : std::stoi(*(++it));
						const int step = (std::distance(list2.begin(), list2.end()) == 2) ? 1 : ((std::stoi(*(++it)) == 0) ? 1 : std::stoi(*(++it)));
						for (int i = 1; i <= cc; i += 1) 
							((place_ != 1) ? vector_i_.first : vector_i_.second).push_back(first + ((i - 1) * step));
					}
					else
						((place_ != 1) ? vector_i_.first : vector_i_.second).push_back(stoi(itr));
					end_i = ((place_ != 1) ? vector_i_.first : vector_i_.second).back();
			}
			(place_ != 1) ? value_i_.first = static_cast<int>(vector_i_.first.size()) : value_i_.second = static_cast<int>(vector_i_.second.size());
			break;
		case 3:								// vector<double>	размеры в метрах
			double end_d = 0.;
			for (auto& itr : list)
			{
					if (boost::algorithm::contains(itr, ":"))
					{
						boost::trim_if(itr, is_brackets);
						boost::char_separator<char> sep2(":");
						boost::tokenizer< boost::char_separator<char> > list2(itr, sep2);
						auto it = list2.begin();
						const double first = (std::stod(*it) == 0.) ? end_d : std::stod(*it);
						const int cc = (std::stoi(*(++it)) == 0) ? 1 : std::stoi(*(++it));
						const double step = (std::distance(list2.begin(), list2.end()) == 2) ? delta : ((std::stod(*(++it)) == 0.) ? delta : std::stod(*(++it)));
						for (int i = 1; i <= cc; i += 1)
						{ 
							double tmp = first + ((i - 1) * step);
							((place_ != 1) ? vector_d_.first : vector_d_.second).push_back(tmp);
							((place_ != 1) ? vector_i_.first : vector_i_.second).push_back(static_cast<int>(std::round(tmp / delta)));
						}
					}
					else
					{
						((place_ != 1) ? vector_d_.first : vector_d_.second).push_back(stod(itr));
						((place_ != 1) ? vector_i_.first : vector_i_.second).push_back(static_cast<int>(std::round(stod(itr) / delta)));
						
					}
					end_d = ((place_ != 1) ? vector_d_.first : vector_d_.second).back();
			}
			(place_ != 1) ? value_i_.first = static_cast<int>(vector_d_.first.size()) : value_i_.second = static_cast<int>(vector_d_.second.size());
			break;
		default: ;
		}
	}
	catch (const std::exception& e)
	{
		f_err_.open("error.log", std::ios::out);
		f_err_ << "Ошибка: " << e.what() << std::endl;
	}

}

std::string uk5_b_var::get_value()
{
	std::string s = "0";
	
	return s;
}

bool uk5_b_var::is_brackets(const char c)
{
	return c == '(' || c == ')';
}
