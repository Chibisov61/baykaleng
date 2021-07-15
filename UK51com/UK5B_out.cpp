// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "UK5B_out.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/tokenizer.hpp>

//UK5B_out::UK5B_out()
//{
//	f_out_.open("error.log", std::ios::out);
//}

uk5_b_out::uk5_b_out(const std::string& ext, const bool ch)
{
	check_ = ch;
	const auto tt = boost::posix_time::second_clock::local_time();
	const std::string date = to_iso_string(tt);
	if( check_) filename_ = "UK5." + date + '.' + ext;
//	wb_.clear();
	ws_ = wb_.active_sheet();
	ws_.cell("A1").value("Расчет кратного разбавления по методу Караушева");
	ws_.cell("Q1").value(date);
	count_ = 0;
//	wb_.save(filename_);
}

void uk5_b_out::header_print(uk5_b_river& r)
{
	if (check_)
	{
//		wb_.load(filename_);		
//		ws_ = wb_.active_sheet();
		for (int j = 0; j < 16; j++)
		{
			const uk5_b_set rsl = r.river.at(r.search(left_[j]));
			std::string desc = rsl.desc;
			const auto type = rsl.get_type();
			const int c = (type == 2) ? 1 : type;
			auto val = rsl.get_value(c);
			
			std::string desc_right;
			std::string val_right;
			if (type < 2)
			{
				const uk5_b_set rsr = r.river.at(r.search(right_[j]));
				desc_right = rsr.desc;
				const int type_right = rsr.get_type();
				const int c_right = ( type_right == 2) ? 1 : type_right;
				val_right = rsr.get_value(c_right);

			}
			else
			{
				desc_right = desc + " (в ячейках)";
				val_right = rsl.get_value(c - 1);
				if (type == 3)
				{
					std::replace(val.begin(), val.end(), ';', ',');
					std::replace(val_right.begin(), val_right.end(), ';', ',');
				}
			}
  			ws_.cell(xlnt::cell_reference(1,j + 2)).value(desc);
  			ws_.cell(xlnt::cell_reference(2,j + 2)).value(val);
  			ws_.cell(xlnt::cell_reference(17,j + 2)).value(desc_right);
  			ws_.cell(xlnt::cell_reference(18,j + 2)).value(val_right);
		}
//		wb_.save(filename_);
	}
}

void uk5_b_out::body_print(const int lvl, uk5_b_river& r)
{
	if (check_)
	{
//		wb_.load(filename_);		
//		ws_ = wb_.active_sheet();
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

  		for (int i = 1; i < h + 1; ++i)
  			for (int j = 1; j < w + 1; j++)
  				ws_.cell(xlnt::cell_reference(j,i + 18 + (h + 2) * count_)).value(round(cut.at(h - i) .at(j - 1) * 10000) / 10000);
		
		count_++;

  		ws_.cell(xlnt::cell_reference(1,17 + (h + 2) * count_)).value(lvl * dx + xn);
  		ws_.cell(xlnt::cell_reference(2,17 + (h + 2) * count_)).value(lvl);
  		ws_.cell(xlnt::cell_reference(3,17 + (h + 2) * count_)).value(tt / mx);
  		ws_.cell(xlnt::cell_reference(4,17 + (h + 2) * count_)).value(mx);
		wb_.save(filename_);
	}
}
