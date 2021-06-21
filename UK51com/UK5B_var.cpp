// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//#include <utility>
#include <string>
#include <boost/algorithm/string.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/tokenizer.hpp>
#include <utility>
#include "UK5B_var.h"

uk5_b_var::uk5_b_var(std::string n, const std::string& t, const std::string& def, const int c, const int max, const std::pair<double, double> delta, const std::pair<double, double> shift) : name_(std::move(n)),delta_(delta),shift_(shift)
{
	type_	= m_type.at(t);
	place_	= 2;
	max_	= max;
	std::string s = def;
	if (type_ == 5) return;

	if (const bool x = boost::filesystem::exists("config.ini"); x)
	{
		try
			{
				read_ini("config.ini", ipt_);
				for (boost::property_tree::ptree::iterator pos = ipt_.begin(); pos != ipt_.end(); ++pos)
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
	set_value(s, c);
	set_value(s, c+4); //-V112
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

void uk5_b_var::set_value(const std::string& def, int c)
{
	bool da = true;

	if ((c >= 4) && (c < 8) || (c == 12)) //-V112
	{
		da = false;
		c -= 4;	//-V112
	}

	if (c == 8)
	{
		c = 0;
		if (boost::algorithm::contains(def, ".")) c += 1;
		if (boost::algorithm::contains(def, ";")) c += 2;
	}
	
	const std::string s = (def.empty()) ? not_an_empty_string(c) : def;

	auto delta = (da) ? delta_.first : delta_.second;
	auto shift = (da) ? shift_.first : shift_.second;
	
	try
	{
		switch (c)
		{
		case 0: {	// int				счетчик  (quantity and geometry:value?)
			(da ? value_i_.first : value_i_.second) = std::stoi(s);
			if (type_ == 2)
				(da ? value_d_.first : value_d_.second ) = std::stoi(s) * delta + ((max_ > 0) ? shift : 0.);
		}
		break;
		case 1: {	// double			величина (quality and geometry:value)
			(da ? value_d_.first : value_d_.second) = std::stod(s);
			if (type_ == 2)
				(da ? value_i_.first : value_i_.second ) = static_cast<int>(std::round((stod(s) - ((max_ > 0) ? shift : 0)) / delta));
		}
		break;
		case 2: {	// vector<int>		размеры в ячейках (geometry:vector?)
			std::vector<int> vector_i_tmp1,vector_i_tmp2 = {};
			std::vector<double> vector_d_tmp1,vector_d_tmp2 = {};
			boost::char_separator sep(";");
			boost::tokenizer list(s, sep);
			int end_i = 1;
			double end_d = (max_ >= 0) ? 0. : shift;
			for (auto& itr : list)
			{
				if (boost::algorithm::contains(itr, ":"))
				{
					if (max_ < 0) continue;
					std::string itt = boost::algorithm::trim_copy_if(itr, &is_brackets);
					boost::char_separator sep2(":");
					boost::tokenizer list2(itt, sep2);
					auto it = list2.begin();
					const int first = (std::stoi(*it) == 0) ? end_i : std::stoi(*it);
					if (first < end_i) continue;
					const int cc = (std::stoi(*(++it)) == 0) ? 1 : std::stoi(*(it));
					const int step = (std::distance(list2.begin(), list2.end()) == 2) ? 1 : ((std::stoi(*(++it)) == 0) ? 1 : std::stoi(*(it)));
					for (int i = 0; i < cc; i += 1)
					{
						vector_i_tmp2.push_back(first + (i * step));
						vector_d_tmp2.push_back((first + (i * step)) * delta + shift);
					}
				}
				else
				{
					if ((stoi(itr) < end_i) && (max_ >= 0)) continue;
					vector_i_tmp2.push_back(stoi(itr));
					vector_d_tmp2.push_back(stoi(itr) * delta + ((max_ > 0) ? shift : end_d));
				}
				end_i = (vector_i_tmp2.empty()) ? 0 : vector_i_tmp2.back();
				end_d = (vector_d_tmp2.empty()) ? 0 : vector_d_tmp2.back();
			}
			if (auto sz = static_cast<int>(vector_i_tmp2.size()); max_ > 0)
			{
				auto min = (vector_i_tmp2.empty()) ? 0 : vector_i_tmp2.front();
				if( max_ > sz)
					for(int i = 1; i <= max_ - sz; i++)
					{
						if (i >= min) break;
						vector_i_tmp1.push_back(i);
						vector_d_tmp1.push_back(i * delta + shift);
					}
					
				vector_i_tmp1.insert(vector_i_tmp1.end(), vector_i_tmp2.begin(),vector_i_tmp2.end());
				vector_d_tmp1.insert(vector_d_tmp1.end(), vector_d_tmp2.begin(),vector_d_tmp2.end());
					
			}
			else
			{
				int mz = (max_ == 0) ? 1 : -max_/sz;
				while(mz > 0)
				{
					vector_i_tmp1.insert(vector_i_tmp1.end(), vector_i_tmp2.begin(),vector_i_tmp2.end());
					vector_d_tmp1.insert(vector_d_tmp1.end(), vector_d_tmp2.begin(),vector_d_tmp2.end());
					mz--;
				}
				if (auto ost = -max_ % sz; ost > 0)
				{
					vector_i_tmp1.insert(vector_i_tmp1.end(), vector_i_tmp2.begin(),vector_i_tmp2.begin()+ost);
					vector_d_tmp1.insert(vector_d_tmp1.end(), vector_d_tmp2.begin(),vector_d_tmp2.begin()+ost);
				}
			}	
				
			(da ? vector_i_.first : vector_i_.second) = vector_i_tmp1;
			(da ? vector_d_.first : vector_d_.second) = vector_d_tmp1;
			(da ? value_i_.first : value_i_.second) = max_;
		}
		break;
		case 3: {	// vector<double>	размеры в метрах (geometry:vector)
			std::vector<int> vector_i_tmp1,vector_i_tmp2 = {};
			std::vector<double> vector_d_tmp1,vector_d_tmp2 = {};
			boost::char_separator sep(";");
			boost::tokenizer list(s, sep);
			double end_d = 0.;
			for (auto& itr : list)
			{
				if (boost::algorithm::contains(itr, ":"))
				{
					if (max_ < 0) continue;
					std::string itt = boost::algorithm::trim_copy_if(itr, &is_brackets);
					boost::char_separator sep2(":");
					boost::tokenizer list2(itt, sep2);
					auto it = list2.begin();
					const double first = (fabs(std::stod(*it) - 0.) < DBL_EPSILON) ? end_d : std::stod(*it);
					if (first < end_d) continue;
					const int cc = (fabs(std::stoi(*(++it)) - 0.) < DBL_EPSILON) ? 1 : std::stoi(*(it));
					const double step = (std::distance(list2.begin(), list2.end()) == 2) ? delta : (fabs(std::stod(*(++it)) - 0.) < DBL_EPSILON) ? delta : std::stod(*(it));	
					for (int i = 0; i < cc; i += 1)
						if (double tmp = first + (i * step); tmp >= shift)
						{
							vector_d_tmp2.push_back(tmp);
							vector_i_tmp2.push_back(static_cast<int>(std::round((tmp - shift) / delta)));
						}
				}
				else if (stod(itr) >= shift)
				{
					if ((stod(itr) < end_d) && (max_ > 0)) continue;
					vector_d_tmp2.push_back(stod(itr));
					vector_i_tmp2.push_back(static_cast<int>(std::round((stod(itr) - ((max_ > 0) ? shift : 0)) / delta)));	
				}
				end_d = (vector_d_tmp2.empty()) ? 0 : vector_d_tmp2.back();
			}
			if (auto sz = static_cast<int>(vector_d_tmp2.size()); max_ > 0)
			{
				auto min = (vector_i_tmp2.empty()) ? 0 : vector_i_tmp2.front();
				if( max_ > sz)
					for(int i = 1; i <= max_ - sz; i++)
					{
						if (i >= min) break;
						vector_i_tmp1.push_back(i);
						vector_d_tmp1.push_back(i * delta + shift);
					}
					
				vector_d_tmp1.insert(vector_d_tmp1.end(), vector_d_tmp2.begin(),vector_d_tmp2.end());
				vector_i_tmp1.insert(vector_i_tmp1.end(), vector_i_tmp2.begin(),vector_i_tmp2.end());
					
			}
			else
			{
				int mz = (max_ == 0) ? 1 : -max_/sz;
				while(mz > 0)
				{
					vector_d_tmp1.insert(vector_d_tmp1.end(), vector_d_tmp2.begin(),vector_d_tmp2.end());
					vector_i_tmp1.insert(vector_i_tmp1.end(), vector_i_tmp2.begin(),vector_i_tmp2.end());
					mz--;
				}
				if (auto ost = -max_ % sz; ost > 0)
				{
					vector_d_tmp1.insert(vector_d_tmp1.end(), vector_d_tmp2.begin(),vector_d_tmp2.begin()+ost);
					vector_i_tmp1.insert(vector_i_tmp1.end(), vector_i_tmp2.begin(),vector_i_tmp2.begin()+ost);
				}
			}	
				
			(da ? vector_d_.first : vector_d_.second) = vector_d_tmp1;
			(da ? vector_i_.first : vector_i_.second) = vector_i_tmp1;
			(da ? value_i_.first : value_i_.second) = max_;
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

std::string uk5_b_var::get_value(const int c) const
{
	std::string s = not_an_empty_string(c % 4);	//-V112
	switch (c)
	{
	case 0:
		s = std::to_string(value_i_.first);
		break;
	case 1:
		s = std::to_string(value_d_.first);
		break;
	case 2:
		s.clear();
		for (auto &it : vector_i_.first)
			s.append(std::to_string(it) + ";");
		break;
	case 3:
		s.clear();
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
		s.clear();
		for (auto &it : vector_i_.second)
			s.append(std::to_string(it) + ";");
		break;
	case 7:
		s.clear();
		for (auto &it : vector_d_.second)
			s.append(std::to_string(it) + ";");
		break;
	default: 
		break;
	}
	
	return s;
}

void uk5_b_var::set_max(const int max)
{
	max_ = max;
}

int uk5_b_var::get_max() const
{
	return max_;
}

void uk5_b_var::set_delta(const std::pair<double, double> delta)
{
	delta_ = delta;
}

std::pair<double, double> uk5_b_var::get_delta() const
{
	return delta_;
}

void uk5_b_var::set_shift(const std::pair<double, double> shift)
{
	shift_ = shift;
}

std::pair<double, double> uk5_b_var::get_shift() const
{
	return shift_;
}
