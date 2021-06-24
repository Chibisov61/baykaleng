#pragma once
#include "UK5B_river.h"

class uk5_b_out
{
public:
//				UK5B_out();
	explicit	uk5_b_out(const std::string&,bool);
//				~UK5B_out() = default;
//				UK5B_out(const UK5B_out&) = delete;
//				UK5B_out(UK5B_out&&) = default;
//				UK5B_out& operator = (const UK5B_out&) = delete;
//				UK5B_out& operator = (UK5B_out&&) = default;

				void uk5_b_header_print(uk5_b_river&);
				void uk5_b_body_print(int,uk5_b_river&);

protected:
				const std::string			t_		= ";;;;;;;;;;;;;;";
				std::vector<std::string>	left_	= { "vr","qst","dog","bl", "b","br", "w", "h", "l","ll","hog","nn","xn","cct", "n","nog"};
				std::vector<std::string>	right_	= { "vst","dt","dzz","--","--","--","--","--","--","--", "--","dx","dy","dz", "pd", "pc"};

				std::ofstream				f_out_;
				bool						check_;
};	