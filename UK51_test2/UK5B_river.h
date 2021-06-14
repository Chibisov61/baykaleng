#pragma once
#include "UK5B_var.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/filesystem/operations.hpp>

class uk5_b_river
{
public:
		uk5_b_river();																														//  конструктор по умолчанию
	//	~uk5_b_river() = default;																											//	деструктор 
	// 	uk5_b_river(const uk5_b_river&) = default;																							//	конструктор копирования
	//	uk5_b_river(uk5_b_river&&)  noexcept = default;																						//	конструктор присваивания
	//	uk5_b_river& operator = (const uk5_b_river&) = default;																				//	оператор копирования
	//	uk5_b_river& operator = (uk5_b_river&&) noexcept = default;																			//	оператор присваивания

		std::vector<uk5_b_set>				river = {};
		std::vector<std::vector<double>>	cut = {{}};
	
		std::map<std::string,int>			m_place = {{"in",0},{"in_out",1},{"out",2},{"final",3}};
protected:	
		boost::property_tree::ptree			pt_;
};
