// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "UK5B_river.h"

uk5_b_river::uk5_b_river()
{
	if (const bool x = boost::filesystem::exists("rules.xml"); x)
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

							river.emplace_back(name, type);
							river.back().set_place(m_place[p_snd.get<std::string>("place")]);
							river.back().desc = p_snd.get<std::string>("description");
							river.back().state = (p_snd.get<std::string>("state") == "enable") ? true : false;

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
				}
	}
}

void uk5_b_river::init(uk5_b_set v, const std::string& def)
{
	int max = 0;
	const int c = v.get_type();
	std::vector<std::pair<std::string,std::string>> p = {};

	for (int j = 1; j < v.number; j++)
	{
		if (!river.at(j).is_init()) throw std::runtime_error("Обнаружена не инициализированная переменная (" + river.at(j).get_name() + ")");
		
		if (!v.max.empty())
			if (river.at(j).get_name() == v.max)
			{
//				max = std::get<int>(river.at(j).get_value(0));
				max = std::stoi(river.at(j).get_string(0));
				if (v.max == "dy") max = -max;
			}

		if (!v.delta.empty())
			if (river.at(j).get_name() == v.delta)
			{
//				const double delta = std::get<double>(river.at(j).get_value(1));
				const double delta = std::stod(river.at(j).get_string(1));
				v.set_delta(delta);
			}
		
		if (!v.shift.empty())
			if (river.at(j).get_name() == v.shift)
			{
//				const double shift = std::get<double>(river.at(j).get_value(1));
				const double shift = std::stod(river.at(j).get_string(1));
				v.set_shift(shift);
			}

		if (!v.param.empty())
			if (boost::algorithm::contains(v.param,river.at(j).get_name()))
			{
				const int cc = river.at(j).get_type();
				auto r = std::make_pair(river.at(j).get_name(), river.at(j).get_string((cc == 2) ? 1 : cc));
				p.push_back(r);
			}
	}
	const std::string d = (def == "-") ? eval(v.get_name(), p) : def;
	
	v.set_value(d, max, (c == 2) ? 1 : c);
	v.set_init(true);
}

std::string uk5_b_river::eval(const std::string& name, const std::vector<std::pair<std::string,std::string>>& p)
{
	switch(std::map<std::string,int> map_n =	{{"dog",0},{"vst",1},{"dt",2},{"dzz",3},{"nn",4},{"xn",5},{"dz",6},{"dy",7},{"pc",8},{"pd",9},{"dx",10},{"w",11}}; map_n[name])
	{
	case 0:	//dog
		{
			const double pi		= 3.1415;
			const double qst	= stod(var("qst",p));
			
			return std::to_string(pow(4. * qst / pi / 25., 0.4)); 
		}
	case 1:	//vst
		{
			const double pi		= 3.1415;
			const double dog	= std::stod(var("dog",p));
			const double qst	= std::stod(var("qst",p));
			
			return std::to_string((4 * qst) / (pi * dog * dog));
		}
	case 2:	//dt
		{
			const double dvm	= 0.1;
			const double vr		= std::stod(var("vr",p));
			const double vst	= std::stod(var("vst",p));

			const auto m = vr / vst;																	
			return std::to_string(sqrt(8.1 / (((1 - m) * dvm * dvm / 0.92) + (2 * m * dvm / 0.96))));
		}
	case 3:	//dzz
		{
			const double qst	= std::stod(var("qst",p));
			const double vr		= std::stod(var("vr",p));
			const double vst	= std::stod(var("vst",p));
			const double dt		= std::stod(var("dt",p));

			return std::to_string((((vr / vst) < 0.25) && (vst < 2.)) ? (qst / vr) : (qst * dt * dt / vst));
		}
	case 4:	//nn
		{
			const double vr		= std::stod(var("vr",p));
			const double vst	= std::stod(var("vst",p));
			const double dt		= std::stod(var("dt",p));

//	if (_h / dzz <= 1)
//	nnr = nnr * f(_h / dzz);	// проверка на влияние дна
			
			const auto m = vr / vst;																	
			return std::to_string(((m < 0.25) && (vst < 2.)) ? 1. : (0.248 * dt * dt / (1 - m)) * (sqrt(m * m + 8.1 * (1 - m) / dt / dt) - m));
		}
	case 5:	//xn
		{
			const double vr		= std::stod(var("vr",p));
			const double vst	= std::stod(var("vst",p));
			const double dzz	= std::stod(var("dzz",p));

			const auto m = vr / vst;
			return std::to_string(((m < 0.25) && (vst < 2.)) ? 0. : (dzz / 0.48 / (1 - 3.12 * m)));
		}
	case 6:	//dz
		{
			const double n		= std::stod(var("n",p));
			const double dzz	= std::stod(var("dzz",p));

			const auto on = dzz / static_cast<double>(n) / static_cast<double>(n);
			return std::to_string(sqrt(on));
		}
	case 7:	//dy
		{
			const double dz	= std::stod(var("dz",p));
			
			return std::to_string(dz);
		}
	case 8:	//pc
		{
			const double g		= 9.8110;
			const double h		= std::stod(var("h",p));
			const double psh	= std::stod(var("psh",p));

			auto const a1 = 1 / psh - sqrt(g) * (1 - log10(h)) / 0.13;
			auto const a2 = sqrt(g) * (1 / psh + sqrt(g) * log10(h)) / 0.13;
			return std::to_string(a1 / 2 + sqrt(a1 * a1 / 4 + a2));
		}
	case 9:	//pd
		{
			const double g		= 9.8110;
			const double vr		= std::stod(var("vr",p));
			const double h		= std::stod(var("h",p));
			const double pc		= std::stod(var("pc",p));
			
			auto const mm = pc < 60 ? 0.75 * pc + 6 : 48;									
			
			return std::to_string(g * h * vr / mm / pc);
		}
	case 10://dx
		{
			const double vr		= std::stod(var("vr",p));
			const double pd		= std::stod(var("pd",p));
			const double dy		= std::stod(var("dy",p));
			
			return std::to_string(vr * dy * dy/ (4. * pd));
		}
	case 11://w 
		{
			const double br		= std::stod(var("br",p));
			const double bl		= std::stod(var("bl",p));
			
				  double w		= br + bl;
			std::string	 b		= var("b", p);
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

std::string uk5_b_river::var(const std::string& var, const std::vector<std::pair<std::string, std::string>>& val)
{
	for (const auto& [fst, snd] : val)
		if (fst  == var) return snd;
	
	return {};
}

