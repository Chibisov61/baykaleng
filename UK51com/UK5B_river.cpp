// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "UK5B_river.h"
#include <boost/tokenizer.hpp>


uk5_b_river::uk5_b_river()
{
	if (const bool x = boost::filesystem::exists("rules.xml"); x)
	{
		try
		{
			int k = 0;
			read_xml("rules.xml", pt_);
			for (auto & [fst, snd]: pt_.get_child("variables"))
			{
				for (auto& [p_fst, p_snd] : snd)
				{
					if (p_fst == "<xmlattr>")
					{
						auto name = p_snd.get<std::string>("name");
						auto type = p_snd.get<std::string>("type");

						if (name == "cut") continue;

						river.emplace_back(name, type);
//							river.back().set_place(m_place[p_snd.get<std::string>("place")]);
						river.back().desc = p_snd.get<std::string>("description");
						river.back().state = (p_snd.get<std::string>("state") == "enabled") ? true : false;

						river.back().max = p_snd.get<std::string>("max", "");
						river.back().delta = p_snd.get<std::string>("delta", "");
						river.back().shift = p_snd.get<std::string>("shift", "");

					}
					else if(p_fst == "parameter")
					{
						auto s = p_snd.get<std::string>("<xmlattr>.name");
						river.back().param.push_back(s);
					}
					else if(p_fst == "child")
					{
						river.back().child_place = m_place[p_snd.get<std::string>("<xmlattr>.place")];
						river.back().child_desc = p_snd.get<std::string>("<xmlattr>.description");
					}
				}
				river.back().number = ++k;
				init(river.back());
					river.back().set_init(true);
			}
		}
		catch (boost::property_tree::ptree_error &e)
		{
			std::ofstream f_err;
			f_err.open("error.log", std::ios::out);
			f_err << "Ошибка: " << e.what() << std::endl;
		}
	}
}

void uk5_b_river::init(uk5_b_set v)
{
	const int c = v.get_type();
	const int cc = (c == 2) ? 1 : c;
	std::vector<std::tuple<std::string,std::string,std::string>> p = {};
	const bool y1 = (!v.max.empty()) || (!v.delta.empty()) || (!v.shift.empty());
	const bool y2 = (!v.param.empty());

	for (int j = 0; j < v.number - 1; j++)
	{
		try
		{
			if (!river.at(j).is_init()) throw std::runtime_error("Обнаружена не инициализированная переменная (" + river.at(j).get_name() + ")");

			if (!v.delta.empty())
				if (river.at(j).get_name() == v.delta)
					river.back().set_delta(std::make_pair(std::stod(river.at(j).get_string(1)),std::stod(river.at(j).get_string(5))));

			if (!v.shift.empty())
				if (river.at(j).get_name() == v.shift)
					river.back().set_shift(std::make_pair(std::stod(river.at(j).get_string(1)),std::stod(river.at(j).get_string(5))));

			if (!v.max.empty())
				if (river.at(j).get_name() == v.max)
				{
					int max = std::stoi(river.at(j).get_string(0));
					if (v.delta == "dy") max = -max;
					river.back().set_max(max);
				}

			if (!v.param.empty())
				if (auto res = std::find(v.param.begin(), v.param.end(), river.at(j).get_name()); res != v.param.end())
				{
					const int cc0 = (river.at(j).get_type() == 2) ? 1 : river.at(j).get_type();
					const int cc4 = cc0 + 4;
					auto r = std::make_tuple(river.at(j).get_name(), river.at(j).get_string(cc0), river.at(j).get_string(cc4));
					p.push_back(r);
				}
		}
		catch (const std::exception& e)
		{
			std::ofstream f_err;
			f_err.open("error.log", std::ios::out);
			f_err << "Ошибка: " << e.what() << std::endl;
		}
	}
	
	try
	{
		if (y1)
		{
			river.back().set_value(v.get_string(cc), cc);
			river.back().set_value(v.get_string(cc + 4), cc + 4);
		}
		
		if (y2) {
			if (river.back().get_place() != 1) 
			{
				if (river.back().state)
					river.back().set_value(eval(v.get_name(), p, "first"), cc);
				else
					river.back().set_value(eval(v.get_name(), p, "second"), cc);
			}
			river.back().set_value(eval(v.get_name(), p, "second"), cc + 4);
		}
	}
	catch (const std::exception& e)
	{
		std::ofstream f_err;
		f_err.open("error.log", std::ios::out);
		f_err << "Ошибка: " << e.what() << std::endl;
	}
}

std::string uk5_b_river::eval(const std::string& name, const std::vector<std::tuple<std::string,std::string,std::string>>& p, const std::string& acc)
{
	switch(std::map<std::string,int> map_n =	{{"dog",0},{"vst",1},{"dt",2},{"dzz",3},{"nn",4},{"xn",5},{"dz",6},{"dy",7},{"pc",8},{"pd",9},{"dx",10},{"w",11}}; map_n[name])
	{
	case 0:	//dog
		{
			const double pi		= 3.1415;
			const double qst	= std::stod(var("qst",p,acc));
			
			return std::to_string(pow(4. * qst / pi / 25., 0.4)); 
		}
	case 1:	//vst
		{
			const double pi		= 3.1415;
			const double dog	= std::stod(var("dog",p,acc));
			const double qst	= std::stod(var("qst",p,acc));

			return std::to_string((4 * qst) / (pi * dog * dog));
		}
	case 2:	//dt
		{
			const double dvm	= 0.1;
			const double vr		= std::stod(var("vr",p,acc));
			const double vst	= std::stod(var("vst",p,acc));

			const auto m = vr / vst;																	
			return std::to_string(sqrt(8.1 / (((1 - m) * dvm * dvm / 0.92) + (2 * m * dvm / 0.96))));
		}
	case 3:	//dzz
		{
			const double qst	= std::stod(var("qst",p,acc));
			const double vr		= std::stod(var("vr",p,acc));
			const double vst	= std::stod(var("vst",p,acc));
			const double dt		= std::stod(var("dt",p,acc));

			return std::to_string((((vr / vst) < 0.25) && (vst < 2.)) ? (qst / vr) : (qst * dt * dt / vst));
		}
	case 4:	//nn
		{
			const double vr		= std::stod(var("vr",p,acc));
			const double vst	= std::stod(var("vst",p,acc));
			const double dt		= std::stod(var("dt",p,acc));

//	if (_h / dzz <= 1)
//	nnr = nnr * f(_h / dzz);	// проверка на влияние дна
			
			const auto m = vr / vst;																	
			return std::to_string(((m < 0.25) && (vst < 2.)) ? 1. : (0.248 * dt * dt / (1 - m)) * (sqrt(m * m + 8.1 * (1 - m) / dt / dt) - m));
		}
	case 5:	//xn
		{
			const double vr		= std::stod(var("vr",p,acc));
			const double vst	= std::stod(var("vst",p,acc));
			const double dzz	= std::stod(var("dzz",p,acc));

			const auto m = vr / vst;
			return std::to_string(((m < 0.25) && (vst < 2.)) ? 0. : (dzz / 0.48 / (1 - 3.12 * m)));
		}
	case 6:	//dz
		{
			const double n		= std::stod(var("n",p,acc));
			const double dzz	= std::stod(var("dzz",p,acc));

			const auto on = dzz / static_cast<double>(n) / static_cast<double>(n);
			return std::to_string(sqrt(on));
		}
	case 7:	//dy
		{
			const double dz	= std::stod(var("dz",p,acc));
			
			return std::to_string(dz);
		}
	case 8:	//pc
		{
			const double g		= 9.8110;
			const double h		= std::stod(var("h",p,acc));
			const double psh	= std::stod(var("psh",p,acc));

			auto const a1 = 1 / psh - sqrt(g) * (1 - log10(h)) / 0.13;
			auto const a2 = sqrt(g) * (1 / psh + sqrt(g) * log10(h)) / 0.13;
			return std::to_string(a1 / 2 + sqrt(a1 * a1 / 4 + a2));
		}
	case 9:	//pd
		{
			const double g		= 9.8110;
			const double vr		= std::stod(var("vr",p,acc));
			const double h		= std::stod(var("h",p,acc));
			const double pc		= std::stod(var("pc",p,acc));
			
			auto const mm = pc < 60 ? 0.75 * pc + 6 : 48;									
			
			return std::to_string(g * h * vr / mm / pc);
		}
	case 10://dx
		{
			const double vr		= std::stod(var("vr",p,acc));
			const double pd		= std::stod(var("pd",p,acc));
			const double dy		= std::stod(var("dy",p,acc));
			
			return std::to_string(vr * dy * dy/ (4. * pd));
		}
	case 11://w 
		{
			const double br		= std::stod(var("br",p,acc));
			const double bl		= std::stod(var("bl",p,acc));
			
				  double w		= br + bl;
			std::string	 b		= var("b",p,acc);
			boost::char_separator sep(";");
			boost::tokenizer list(b, sep);
			for (auto& itr : list)
				w += std::stod(itr);
			
			return std::to_string(w);
		}
	default: 
			return std::to_string(0);
	}
	
}

std::string uk5_b_river::var(const std::string& var, const std::vector<std::tuple<std::string, std::string, std::string>>& val, const std::string& acc)
{
	
	for (auto& v : val)
		if (std::get<0>(v) == var)
		{
			if (acc == "first")
				return std::get<1>(v);
			if (acc == "second")
				return std::get<2>(v);
			return {};
		}

	return {};
}


