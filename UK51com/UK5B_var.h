#pragma once

#include <string>
#include <vector>
#include <map>
#include <variant>

inline std::string not_an_empty_string(const int c)
{
	std::string z = "0";
	if (c % 2 == 1) z = z + '.';
	if (c > 1) z = z + ';';
	
	return z;
}

inline bool is_brackets(const char ch)
{
	return ch == '(' || ch == ')';
}

class uk5_b_var
{

public:
//	uk5_b_var() = default;																														//  конструктор по умолчанию
explicit uk5_b_var(const std::string& n, const std::string& t, const std::string& def="0.", int max = 0, double delta = 1.0, double shift = 0., int c = -1);	//	конструктор (пользовательский) 
//	~uk5_b_var() = default;																														//	деструктор 
// 	uk5_b_var(const uk5_b_var&) = default;																										//	конструктор копирования
//	uk5_b_var(uk5_b_var&&)  noexcept = default;																									//	конструктор присваивания
//	uk5_b_var& operator = (const uk5_b_var&) = default;																							//	оператор копирования
//	uk5_b_var& operator = (uk5_b_var&&) noexcept = default;																						//	оператор присваивания

[[nodiscard]]	std::string	get_name() const;
[[nodiscard]]	int			get_type() const;
	
				void		set_place(int);
[[nodiscard]]	int			get_place() const;

				void		set_init(bool);
[[nodiscard]]	bool		is_init() const;

				void		set_value(const std::string& def = "0.", int c = -1);
[[nodiscard]]	std::string	get_string(int) const;
[[nodiscard]]	std::variant<double,std::vector<double>,int,std::vector<int>>	get_value(int) const;

				void		set_max(int);
[[nodiscard]]	int			get_max() const;

				void		set_delta(double);
[[nodiscard]]	double		get_delta() const;
	
				void		set_shift(double);
[[nodiscard]]	double		get_shift() const;

				void		swap_value();

				std::map<std::string,int>							m_place = {{"in",0},{"in_out",1},{"out",2},{"final",3}};
				std::map<std::string,int>							m_type = {{"quantity",0},{"quality",1},{"geometry:value",2},{"geometry:vector",3},{"section",4}};

private:
				std::string											name_;
				int													type_	= 0;
				int													place_	= 2;
				bool												init_	= false;
	
				std::pair<double, double>							value_d_	= {};
				std::pair<std::vector<double>, std::vector<double>>	vector_d_	= {};
				std::pair<int, int>									value_i_	= {};
				std::pair<std::vector<int>, std::vector<int>>		vector_i_	= {};

				   int												max_	= 100;
				double												delta_	= 1.;
				double												shift_	= 0.;

};