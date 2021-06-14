// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "UK5B_river.h"

uk5_b_river::uk5_b_river()
{

	if (const bool x = boost::filesystem::exists("rules.xml"); x)
	{
				read_xml("rules.xml", pt_);
				for (auto & [fst, snd]: pt_.get_child("variables"))
				{
					auto name	= snd.get<std::string>("<xmlattr>.name");
					auto type	= snd.get<std::string>("<xmlattr>.type");
					
					if (name != "cut")
					{
						river.emplace_back(name,type);
						river.back().set_place(m_place[snd.get<std::string>("<xmlattr>.place")]);
						river.back().desc	= snd.get<std::string>("<xmlattr>.description");
						river.back().state	= (snd.get<std::string>("<xmlattr>.state") == "enable") ? true : false ;
						
						river.back().max	= snd.get<int>("<xmlattr>.max",100);
						river.back().delta	= snd.get<double>("<xmlattr>.delta", 1.);
						river.back().shift	= snd.get<double>("<xmlattr>.shift",0.);
						
					}

					if (type == "geometry")
					{
						auto& pt2 = snd.get_child("child_variable");
						auto name2	= pt2.get<std::string>("<xmlattr>.name");
						auto type2	= pt2.get<std::string>("<xmlattr>.type");
						river.emplace_back(uk5_b_set(name2,type2));
						river.back().set_place(m_place[pt2.get<std::string>("<xmlattr>.place")]);
						river.back().desc	= pt2.get<std::string>("<xmlattr>.description");
						river.back().state	= (pt2.get<std::string>("<xmlattr>.state") == "enable") ? true : false ;
						
						river.back().max	= pt2.get<int>("<xmlattr>.max",100);
						river.back().delta	= pt2.get<double>("<xmlattr>.delta",1.);
						river.back().shift	= pt2.get<double>("<xmlattr>.shift",0.);
					}
				}
	}
}
