#pragma once
#include "UK5B_river.h"
#include <xlnt/xlnt.hpp>

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

				void header_print(uk5_b_river&);
				void body_print(int,uk5_b_river&);

protected:
				std::vector<std::string>				left_	= { "vr","qst","dog","bl", "b","br", "w", "h", "l","ll","hog","nn","xn","cct", "n","nog"};
				std::vector<std::string>				right_	= { "vst","dt","dzz","--","--","--","--","--","--","--", "--","dx","dy","dz", "pd", "pc"};

				xlnt::workbook							wb_;
				xlnt::worksheet							ws_;
				std::string								filename_;
				int										count_;
				bool									check_;
};	