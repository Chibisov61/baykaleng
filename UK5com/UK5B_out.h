#pragma once
#include "UK5B_river.h"
//#include <fstream>
//#include <map>

class UK5B_river;


class UK5B_out
{
public:
	explicit UK5B_out(const std::string&);
	UK5B_out(const UK5B_out&) = delete;
	UK5B_out(UK5B_out&&) = default;
	UK5B_out& operator = (const UK5B_out&) = delete;
	UK5B_out& operator = (UK5B_out&&) = default;
	

	void UK5B_header_print(const UK5B_river&);
	void UK5B_body_print(int,const UK5B_river&);


protected:
	std::ofstream fout;
	std::map<std::string, std::string>  lmap{{"vr","Скорость реки"},
										{"bb","Расстояние до берега"},
										{"nog","Количество оголовков"},				
										{"b","Расстояния между оголовками"},
										{"h","Глубина"},
										{"hog","Высоты оголовков"},
										{"nl","Количество промежуточных сечений"},
										{"l","Промежуточные сечения"},
										{"qst","Расход сточных вод на оголовок"},
										{"cct","Величина загрязняющего вещества"},		
										{"n","Сторона расчетного квадрата"},
										{"psh","Коэффициент шероховатости"},
										{"dog","Диаметр оголовка"},
										{"nn","Начальное разбавление"},
										{"xn","Расстояние от створа выпуска"},
										{"ll","Расчетный участок реки"},
										{"pc","Коэффициент Шези"},
										{"pd","Коэффициент диффузии"},
										{"dz","dz"},
										{"dy","dy"},							
										{"dx","dx"},							
										{"rbb","Расстояние до берега (расч.)"},
										{"rb","Расстояния между оголовками (расч.)"},
										{"rw","Общая ширина (расч.)"},
										{"rhog","Высоты оголовков (расч.)"},
										{"rh","Глубина (расч.)"},
										{"rl","Помежуточные сечения (расч.)"},
										{"rll","Участок реки (расч.)"}};
	
};
