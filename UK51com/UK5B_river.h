#pragma once
#include "UK5B_var.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/filesystem/operations.hpp>

struct uk5_b_set : uk5_b_var
{
	using uk5_b_var::uk5_b_var;
	bool		state	= true;
	int			number	= 0;
	std::string desc;
	
	std::vector<std::string> param = {};
	std::string	max;
	std::string	delta;
	std::string	shift;
	
	int			child_place = 2;
	std::string	child_desc;
};

class uk5_b_river
{
public:
		uk5_b_river();																														//  конструктор по умолчанию
//		~uk5_b_river() = default;																											//	деструктор 
// 		uk5_b_river(const uk5_b_river&) = default;																							//	конструктор копирования
//		uk5_b_river(uk5_b_river&&)  noexcept = default;																						//	конструктор присваивания
//		uk5_b_river& operator = (const uk5_b_river&) = default;																				//	оператор копирования
//		uk5_b_river& operator = (uk5_b_river&&) noexcept = default;																			//	оператор присваивания

		int									recount(uk5_b_set&);
		int									re_init(uk5_b_set&, bool);
		int									search(const std::string&);
		void								init_cut(const std::vector<std::tuple<std::string,std::string,std::string>>&);
		std::vector<std::vector<double>>	karaushev(std::vector<std::vector<double>>);

		std::vector<uk5_b_set>				river = {};
		std::vector<std::vector<double>>	cut = {{}};
	
		std::map<std::string,int>			m_place = {{"in",0},{"in_out",1},{"out",2},{"final",3}};
		std::map<int,std::string>			m_type = {{0,"quantity"},{1,"quality"},{2,"geometry:value"},{3,"geometry:vector"},{4,"section"}};
protected:	
		static std::string					eval(const std::string&, const std::vector<std::tuple<std::string,std::string,std::string>>&, const std::string&);
		static std::string					var(const std::string&, const std::vector<std::tuple<std::string,std::string,std::string>>&, const std::string&);
		boost::property_tree::ptree			pt_;
};


