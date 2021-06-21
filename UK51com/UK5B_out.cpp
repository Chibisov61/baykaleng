// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "UK5B_out.h"
#include <boost/tokenizer.hpp>


//UK5B_out::UK5B_out()
//{
//	f_out_.open("error.log", std::ios::out);
//}

uk5_b_out::uk5_b_out(const std::string& f)
{
	f_out_.open(f, std::ios::out);
}

void uk5_b_out::uk5_b_header_print(uk5_b_river& r)
{
	std::string tt;
	const int f = static_cast<int>(r.cut.front().size()) - 15;
	for (int i = 1; i < f; i++) tt.append(";");

	for (int j=0; j < 16; j++)
	{
		const uk5_b_set rsl = r.river.at(r.search(left_[j]));
		std::string desc = rsl.desc;
		if (const auto type	=	rsl.get_type(); type < 2)
		{
			auto val = rsl.get_value(type);
			std::replace(val.begin(), val.end(), ';', ',');
			const uk5_b_set rsr = r.river.at(r.search(right_[j]));
			std::string desc_right = rsr.desc;
			const auto type_right = rsr.get_type();
			auto val_right = rsr.get_value(type_right);
			std::replace(val_right.begin(), val_right.end(), ';', ',');
			
			f_out_ << desc << ":" << val << t_ << desc_right << ":" << val_right << tt << std::endl;
		}
		else
		{
			const auto type_right = (type == 2) ? 0 : 2;
			auto val = rsl.get_value(type);
			std::replace(val.begin(), val.end(), ';', ',');
			std::string desc_right = desc + " (в ячейках)";
			auto val_right = rsl.get_value(type_right);
			std::replace(val_right.begin(), val_right.end(), ';', ',');
			
			f_out_ << desc << ":" << val << t_ << desc_right << ":" << val_right << tt << std::endl;
		}
		
	}

	
}
void uk5_b_out::uk5_b_body_print(const int lvl, uk5_b_river& r)
{
	std::vector<std::vector<double>>	cut = r.cut;
	std::vector<double>					m = {};
	const std::string					mx_cut = r.river.at(r.search("cut")).get_value(3);
	const boost::char_separator			sep(";");
	boost::tokenizer list(mx_cut, sep);
	for (auto& itr : list)
		m.push_back(std::stod(itr));

	const double mx = (m.empty()) ? 0 : m.back();
	const double tt = std::stod(r.river.at(r.search("cct")).get_value(1));
	const double dx = std::stod(r.river.at(r.search("dx")).get_value(1));
	const double xn = std::stod(r.river.at(r.search("xn")).get_value(1));

	const int h = static_cast<int>(cut.size());
	const int w = static_cast<int>(cut.front().size());
	

	f_out_ << std::endl;

	for(int i = 0; i <  h - 1; ++i)
	{
		for (int j = 0; j < w - 1; j++)
			f_out_ << round(cut.at(h - i - 1).at(j)*10000)/10000 << ";";
		f_out_ << std::endl;
	}
	
	f_out_ << std::endl;
	f_out_ << (lvl * dx + xn) << "(" << lvl << ")" << ";" << tt / mx << ";" << mx << std::endl;
	
}
