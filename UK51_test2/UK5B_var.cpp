//#include <utility>
#include <string>
#include <boost/algorithm/string.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/tokenizer.hpp>
#include "UK5B_var.h"

uk5_b_var::uk5_b_var(const std::string& n, const std::string& t, const std::string& def, const int max, const double delta, const double shift, const int c)
{
	name_ = n;
	type_ = m_type.at(t);
	
	place_ = 2;
	std::string s = def;

	if (const bool x = boost::filesystem::exists("config.ini"); x)
	{
		try
			{
				boost::property_tree::ptree pt;
				read_ini("config.ini", pt);
				for (boost::property_tree::ptree::iterator pos = pt.begin(); pos != pt.end(); ++pos)
				{
					if (auto f = pos->second.find(name_); f != pos->second.not_found())
					{
						place_ = m_place[pos->first];
						s = (*f).second.get_value<std::string>();
						break;
					}
				}
			}
			
		catch (const boost::property_tree::ptree_error &e)
			{
				std::ofstream f_err;
				f_err.open("error.log", std::ios::out);
				f_err << "Ошибка: " << e.what() << std::endl;
			}

	}

	delta_	= delta;
	shift_	= shift;
	set_value(s, max, c);
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

void uk5_b_var::set_init(const bool b)
{
	init_ = b;
}

bool uk5_b_var::is_init() const
{
	return init_;
}

void uk5_b_var::set_value(const std::string& def, int max, int c)
{
	if (c == -1) {
		c = 0;
		if (boost::algorithm::contains(def, ".")) c += 1;
		if (boost::algorithm::contains(def, ";")) c += 2;
	}
	const auto s = (def.empty()) ? not_an_empty_string(c) : def;
	try
	{
		switch (c)
		{
		case 0:		// int				счетчик 
			((place_ != 1) ? value_d_.first : value_d_.second) = stod(s);
		break;
		case 1:		// double			величина
			((place_ != 1) ? value_i_.first : value_i_.second) = stoi(s);
		break;
		case 2: {	// vector<int>		размеры в ячейках
			std::vector<int> vector_i_tmp1,vector_i_tmp2 = {};
			std::vector<double> vector_d_tmp1,vector_d_tmp2 = {};
			boost::char_separator<char> sep(";");
			boost::tokenizer< boost::char_separator<char> > list(s, sep);
			int end_i = 1;
			double end_d = (max >= 0) ? 0. : shift_;
			for (auto& itr : list)
			{
				if (boost::algorithm::contains(itr, ":"))
				{
					if (max < 0) continue;
					std::string itt = boost::algorithm::trim_copy_if(itr, &is_brackets);
					boost::char_separator<char> sep2(":");
					boost::tokenizer< boost::char_separator<char> > list2(itt, sep2);
					auto it = list2.begin();
					const int first = (std::stoi(*it) == 0) ? end_i : std::stoi(*it);
					if (first < end_i) continue;
					const int cc = (std::stoi(*(++it)) == 0) ? 1 : std::stoi(*(it));
					const int step = (std::distance(list2.begin(), list2.end()) == 2) ? 1 : ((std::stoi(*(++it)) == 0) ? 1 : std::stoi(*(it)));
					for (int i = 0; i < cc; i += 1)
					{
						vector_i_tmp2.push_back(first + (i * step));
						vector_d_tmp2.push_back((first + (i * step)) * delta_ + shift_);
					}
				}
				else
				{
					if ((stoi(itr) < end_i) && (max >= 0)) continue;
					vector_i_tmp2.push_back(stoi(itr));
					vector_d_tmp2.push_back(stoi(itr) * delta_ + ((max >= 0) ? shift_ : end_d));
				}
				end_i = vector_i_tmp2.back();
				end_d = vector_d_tmp2.back();
			}
			if (auto sz = static_cast<int>(vector_i_tmp2.size()); max >= 0)
			{
				auto min = vector_i_tmp2.front();
				if( max > sz)
					for(int i = 1; i <= max - sz; i++)
					{
						if (i >= min) break;
						vector_i_tmp1.push_back(i);
						vector_d_tmp1.push_back(i * delta_ + shift_);
					}
					
				vector_i_tmp1.insert(vector_i_tmp1.end(), vector_i_tmp2.begin(),vector_i_tmp2.end());
				vector_d_tmp1.insert(vector_d_tmp1.end(), vector_d_tmp2.begin(),vector_d_tmp2.end());
					
			}
			else
			{
				max = -max;
				int mz = static_cast<int>(max/sz);
				while(mz > 0)
				{
					vector_i_tmp1.insert(vector_i_tmp1.end(), vector_i_tmp2.begin(),vector_i_tmp2.end());
					vector_d_tmp1.insert(vector_d_tmp1.end(), vector_d_tmp2.begin(),vector_d_tmp2.end());
					mz--;
				}
				if (auto ost = max % sz; ost > 0)
				{
					vector_i_tmp1.insert(vector_i_tmp1.end(), vector_i_tmp2.begin(),vector_i_tmp2.begin()+ost);
					vector_d_tmp1.insert(vector_d_tmp1.end(), vector_d_tmp2.begin(),vector_d_tmp2.begin()+ost);
				}
			}	
				
			((place_ != 1) ? vector_i_.first : vector_i_.second) = vector_i_tmp1;
			((place_ != 1) ? vector_d_.first : vector_d_.second) = vector_d_tmp1;
			(place_ != 1) ? value_i_.first : value_i_.second = max;
		}
		break;
		case 3: {	// vector<double>	размеры в метрах
			std::vector<int> vector_i_tmp1,vector_i_tmp2 = {};
			std::vector<double> vector_d_tmp1,vector_d_tmp2 = {};
			boost::char_separator<char> sep(";");
			boost::tokenizer< boost::char_separator<char> > list(s, sep);
			double end_d = 0.;
			for (auto& itr : list)
			{
				if (boost::algorithm::contains(itr, ":"))
				{
					if (max < 0) continue;
					std::string itt = boost::algorithm::trim_copy_if(itr, &is_brackets);
					boost::char_separator<char> sep2(":");
					boost::tokenizer< boost::char_separator<char> > list2(itt, sep2);
					auto it = list2.begin();
					const double first = (std::stod(*it) == 0.) ? end_d : std::stod(*it);
					if (first < end_d) continue;
					const int cc = (std::stoi(*(++it)) == 0) ? 1 : std::stoi(*(it));
					const double step = (std::distance(list2.begin(), list2.end()) == 2) ? delta_ : ((std::stod(*(++it)) == 0.) ? delta_ : std::stod(*(it)));
					for (int i = 0; i < cc; i += 1)
						if (double tmp = first + (i * step); tmp >= shift_)
						{
							vector_d_tmp2.push_back(tmp);
							vector_i_tmp2.push_back(static_cast<int>(std::round((tmp - shift_) / delta_)));
						}
				}
				else if (stod(itr) >= shift_)
				{
					if ((stod(itr) < end_d) && (max >= 0)) continue;
					vector_d_tmp2.push_back(stod(itr));
					vector_i_tmp2.push_back(static_cast<int>(std::round((stod(itr) - ((max > 0) ? shift_ : 0)) / delta_)));	
				}
				end_d = vector_d_tmp2.back();
			}
			if (auto sz = static_cast<int>(vector_d_tmp2.size()); max >= 0)
			{
				auto min = vector_i_tmp2.front();
				if( max > sz)
					for(int i = 1; i <= max - sz; i++)
					{
						if (i >= min) break;
						vector_i_tmp1.push_back(i);
						vector_d_tmp1.push_back(i * delta_ + shift_);
					}
					
				vector_d_tmp1.insert(vector_d_tmp1.end(), vector_d_tmp2.begin(),vector_d_tmp2.end());
				vector_i_tmp1.insert(vector_i_tmp1.end(), vector_i_tmp2.begin(),vector_i_tmp2.end());
					
			}
			else
			{
				max = -max;
				int mz = static_cast<int>(max/sz);
				while(mz > 0)
				{
					vector_d_tmp1.insert(vector_d_tmp1.end(), vector_d_tmp2.begin(),vector_d_tmp2.end());
					vector_i_tmp1.insert(vector_i_tmp1.end(), vector_i_tmp2.begin(),vector_i_tmp2.end());
					mz--;
				}
				if (auto ost = max % sz; ost > 0)
				{
					vector_d_tmp1.insert(vector_d_tmp1.end(), vector_d_tmp2.begin(),vector_d_tmp2.begin()+ost);
					vector_i_tmp1.insert(vector_i_tmp1.end(), vector_i_tmp2.begin(),vector_i_tmp2.begin()+ost);
				}
			}	
				
			((place_ != 1) ? vector_d_.first : vector_d_.second) = vector_d_tmp1;
			((place_ != 1) ? vector_i_.first : vector_i_.second) = vector_i_tmp1;
			(place_ != 1) ? value_i_.first : value_i_.second = max;
		}
		break; 
		default:
		break;
		}
	}
	catch (const std::exception& e)
	{
		std::ofstream f_err_;
		f_err_.open("error.log", std::ios::out);
		f_err_ << "Ошибка: " << e.what() << std::endl;
	}

}

std::string uk5_b_var::get_string(const int c) const
{
	std::string s = not_an_empty_string(c % 4);
	switch (c)
	{
	case 0:
		s = std::to_string(value_i_.first);
		break;
	case 1:
		s = std::to_string(value_d_.first);
		break;
	case 2:
		s = "";
		for (auto &it : vector_i_.first)
			s.append(std::to_string(it) + ";");
		break;
	case 3:
		s = "";
		for (auto &it : vector_d_.first)
			s.append(std::to_string(it) + ";");
		break;
	case 4:
		s = std::to_string(value_i_.second);
		break;
	case 5:
		s = std::to_string(value_d_.second);
		break;
	case 6:
		s = "";
		for (auto &it : vector_i_.second)
			s.append(std::to_string(it) + ";");
		break;
	case 7:
		s = "";
		for (auto &it : vector_d_.second)
			s.append(std::to_string(it) + ";");
		break;
	default: 
		break;
	}
	
	return s;
}

std::variant<double,std::vector<double>,int,std::vector<int>> uk5_b_var::get_value(const int c) const
{
	switch(c)
	{
	case 0:
		return value_i_.first;
	case 1:
		return value_d_.first;
	case 2:
		return vector_i_.first;
	case 3:
		return vector_d_.first;
	case 4:
		return value_i_.second;
	case 5:
		return value_d_.second;
	case 6:
		return vector_i_.second;
	case 7:
		return vector_d_.second;
	default: 
		return -1;
	}
}

void uk5_b_var::set_delta(const double delta)
{
	delta_ = delta;
}

double uk5_b_var::get_delta() const
{
	return delta_;
}

void uk5_b_var::set_shift(const double shift)
{
	shift_ = shift;
}

double uk5_b_var::get_shift() const
{
	return shift_;
}

void uk5_b_var::swap_value()
{
		std::swap(value_i_.second,value_i_.first);
		std::swap(value_d_.second,value_d_.first);
		std::swap(vector_i_.second,vector_i_.first);
		std::swap(vector_d_.second,vector_d_.first);
}


