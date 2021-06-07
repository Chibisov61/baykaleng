#pragma once

#include <string>
#include <vector>
#include <map>
#include <fstream>

class UK5B_var
{

public:
	explicit UK5B_var(const std::string&);										//	конструктор 
	~UK5B_var() = default;														//	деструктор 
//	UK5B_var(const UK5B_var&) = default;										//	конструктор копирования
//	UK5B_var(UK5B_var&&)  noexcept = default;									//	конструктор переноса
//	UK5B_var& operator = (const UK5B_var&) = default;							//	оператор копирования
//	UK5B_var& operator = (UK5B_var&&) noexcept = default;						//	оператор переноса

	std::string get_name() const;
	int get_type() const;
	
	void set_place(int);
	int get_place() const;

	void set_init(bool);
	bool is_init() const;
	
	std::map<std::string,int>							m_place = {{"in",0},{"in_out",1},{"out",2},{"final",3}};
	std::map<std::string,int>							m_type = {{"quantity",0},{"quality",1},{"geometry",2}};

private:
	std::string											name_;
	int													type_ = 0;
	int													place_ = 2;
	bool												init_ = false;
	
	std::pair<double, double>							value_d_ = { 0.,0. };
	std::pair<std::vector<double>, std::vector<double>>	vector_d_ = { {},{} };
	std::pair<int, int>									value_i_ = {0,0};
	std::pair<std::vector<int>, std::vector<int>>		vector_i_ = { {},{} };

	std::ofstream										f_err_;

};


