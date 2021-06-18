// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "UK5B_river.h"
#include <boost/tokenizer.hpp>
#include <boost/math/constants/constants.hpp>
	
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
						
						river.emplace_back(name, type);
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
				river.back().number = k++;
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

void uk5_b_river::init(uk5_b_set& v)
{
	auto t = v.get_type();
	auto c = (t == 2) ? 1 : t;
	auto name = v.get_name();
	
	std::vector<std::tuple<std::string,std::string,std::string>> p = {};
	const bool y1 = (!v.max.empty()) || (!v.delta.empty()) || (!v.shift.empty());
	const bool y2 = (!v.param.empty());

	for (int j = 0; j < v.number; j++)
	{
		try
		{
			auto rj = river.at(j);
			auto j_name = rj.get_name();
			
			if (!rj.is_init()) throw std::runtime_error("Обнаружена не инициализированная переменная (" + j_name + ")");

			if (!v.delta.empty())
				if (j_name == v.delta)
					v.set_delta(std::make_pair(std::stod(rj.get_value(1)),std::stod(rj.get_value(5))));

			if (!v.shift.empty())
				if (j_name == v.shift)
					v.set_shift(std::make_pair(std::stod(rj.get_value(1)),std::stod(rj.get_value(5))));

			if (!v.max.empty())
				if (j_name == v.max)
				{
					int max = std::stoi(rj.get_value(0));
					if (v.delta == "dy") max = -max;
					v.set_max(max);
				}

			if (!v.param.empty())
				if (auto res = std::find(v.param.begin(), v.param.end(), j_name); res != v.param.end())
				{
					auto tt = rj.get_type();
					const int cc0 = ( tt == 2) ? 1 : river.at(j).get_type();
					const int cc4 = cc0 + 4; //-V112
					if (((name == "w") || (name == "cut")) && (tt >= 2))
					{
						p.emplace_back("i"+ j_name, rj.get_value(cc0-1), rj.get_value(cc4-1));
					}
					
					p.emplace_back(j_name, rj.get_value(cc0), rj.get_value(cc4));
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
			v.set_value(v.get_value(c), c);
			v.set_value(v.get_value(c + 4), c + 4); //-V112
		}
		
		if (y2) {
			if (name == "cut")
				init_cut(p);
			else
			{
				if (v.get_place() != 1) 
				{
					if (v.state)
						v.set_value(eval(name, p, "first"), c);
					else
						v.set_value(eval(name, p, "second"), c);
				}
				
				v.set_value(eval(name, p, "second"), c + 4); //-V112

				if (name == "w")
					v.set_value(eval("iw", p, "first"), 0);
			}			
		}
	}
	catch (const std::exception& e)
	{
		std::ofstream f_err;
		f_err.open("error.log", std::ios::out);
		f_err << "Ошибка: " << e.what() << std::endl;
	}
}

uk5_b_set uk5_b_river::search_by_name(const std::string& s)
{
	for (auto& itr : river)
		if (itr.get_name() == s) return itr;
	
	uk5_b_set u("dummy", "dummy");
	return u;
}

std::string uk5_b_river::eval(const std::string& name, const std::vector<std::tuple<std::string,std::string,std::string>>& p, const std::string& acc)
{
	switch(std::map<std::string,int> map_n = {{"dog",0},{"vst",1},{"dt",2},{"dzz",3},{"nn",4},{"xn",5},{"dz",6},{"dy",7},{"pc",8},{"pd",9},{"dx",10},{"w",11},{"iw",12},{"mx",13},{"mm",14}}; map_n[name])
	{
	case 0:	//dog
		{
			auto pi				= boost::math::constants::pi<double>();
			const double qst	= std::stod(var("qst",p,acc));
			
			return std::to_string(pow(4. * qst / pi / 25., 0.4)); 
		}
	case 1:	//vst
		{
			auto pi				= boost::math::constants::pi<double>();
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
	case 12://iw 
		{
			const    int br		= std::stoi(var("ibr",p,acc));
			const    int bl		= std::stoi(var("ibl",p,acc));
			
					 int w		= br + bl;
			std::string	 b		= var("ib",p,acc);
			boost::char_separator sep(";");
			boost::tokenizer list(b, sep);
			for (auto& itr : list)
				w += std::stoi(itr);
			
			return std::to_string(w);
		}
	case 13://mx
		{
			std::vector<double> m = {};
			std::string	 mx_cut = var("cut",p,acc);
			boost::char_separator sep(";");
			boost::tokenizer list(mx_cut, sep);
			for (auto& itr : list)
				m.push_back(std::stod(itr));

			double mx = *std::max_element(m.begin(), m.end());
			return std::to_string(mx);
		}
	case 14://mm
		{
			const double cct		= std::stod(var("cct",p,acc));
			const double mx		= std::stod(var("mx",p,acc));
			
			return std::to_string(cct / mx);
		}
	default: 
			return std::to_string(0);
	}
	
}

void uk5_b_river::init_cut(const std::vector<std::tuple<std::string,std::string,std::string>>& p)
{
	const int		h	= std::stoi(var("ih",p,"first"));		  
	const int		w	= std::stoi(var("iw",p,"first"));		  
	const int		n	= std::stoi(var("n",p,"first"));
	const double	t	= std::stod(var("cct",p,"first"));
	const int		br	= std::stoi(var("ibr",p,"first"));		  
	const int		bl	= std::stoi(var("ibl",p,"first"));		  
	const double	nn	= std::stod(var("nn",p,"first"));

	boost::char_separator	sep(";");
	std::vector<int>		b	= {};
	std::string				sb	= var("ib",p,"first");						  
	boost::tokenizer		list_b(sb, sep);
	for (auto& itr : list_b)
		b.push_back(std::stoi(itr));

	std::vector<int>		ho	= {};
	std::string				sho	= var("ihog", p, "first");					  
	boost::tokenizer		list_ho(sho, sep);
	for (auto& itr : list_ho)
		ho.push_back(std::stoi(itr));
	
	const int no = static_cast<int>(ho.size()) - 1;
	const double tt = t / nn;

	cut = {};
		
	std::vector<int> wo = {};
	int bb = bl;
	wo.push_back(bb);
	for (int k = 0; k < no; ++k) wo.push_back(bb += b.at(k));
	wo.push_back(bb += br);
	
	std::vector<std::pair<int, int>> o = {};
	o.reserve(no);
	for (int k = 0; k < no; ++k) o.emplace_back(ho.at(k), wo.at(k));

	for (int i = 1; i < (h + 1); ++i)
	{
		std::vector<double> cut_inner;
		for (int j = 0; j < (w + 2); ++j)
		{
			bool e = false;
			for (int k = 0; k < no; ++k)
			{
				if (auto [fst, snd] = o.at(k); (2 * i > 2 * fst - n) && (2 * i < 2 * fst + n + 2) && (2 * j > 2 * snd - n) && (2 * j < 2 * snd + n + 2)) 
				{
					e = true;
					break;
				}
				
			}
			cut_inner.push_back(e ? tt : 0);
		}
		cut.push_back(cut_inner);
		if (i == 1) cut.push_back(cut_inner);
		cut_inner = {};
	}
	
	cut.push_back(cut.back());
	
	search_by_name("cut").set_value(std::to_string(tt) + ";", 3);
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


