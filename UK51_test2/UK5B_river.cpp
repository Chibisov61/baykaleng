// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "UK5B_river.h"

uk5_b_river::uk5_b_river()
{
	if (const bool x = boost::filesystem::exists("rules.xml"); x)
	{
				int n = 0;
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
					river.back().number = ++n;
				}
	}
}


