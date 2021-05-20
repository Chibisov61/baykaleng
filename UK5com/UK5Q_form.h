#pragma once

#include "ui_UK5Q_form.h"
#include "UK5B_river.h"
#include "UK5Q_box.h"
#include <vector>
#include <QMap>


class UK5Q_form : public QMainWindow
{
	Q_OBJECT

public:
	UK5B_river					river;
	QMap<QString, UK5Q_box*>	map_box;
	QMap<int, QWidget*>			map;
	QMap<QString,int>			bmap = {{"vr",1},	{"bb",2},	{"nog",3},{"b",4},	{"h",5},	{"hog",6},{"nl",7},	{"l",8},	{"qst",9},{"cct",10},	{"n",11},	{"psh",12},	{"dog",13},	{"nn",14},
										{"xn",15},	{"ll",16},	{"pc",17},{"pd",18},{"dz",19},	{"dy",20},{"dx",21},{"rbb",22},	{"rb",23},{"rw",24},	{"rhog",25},{"rh",26},	{"rl",27},	{"rll",28}};
	QMap<QString, QString>		lmap = {{"vr",QStringLiteral(u"Скорость реки")},
										{"bb",QStringLiteral(u"Расстояние до берега")},
										{"nog",QStringLiteral(u"Количество оголовков")},				
										{"b",QStringLiteral(u"Расстояния между оголовками")},
										{"h",QStringLiteral(u"Глубина")},
										{"hog",QStringLiteral(u"Высоты оголовков")},
										{"nl",QStringLiteral(u"Количество промеж. сечений")},
										{"l",QStringLiteral(u"Промежуточные сечения (доп.)")},
										{"qst",QStringLiteral(u"Расход сточных вод на оголовок")},
										{"cct",QStringLiteral(u"Величина загрязняющего вещества")},		
										{"n",QStringLiteral(u"Сторона расчетного квадрата")},
										{"psh",QStringLiteral(u"Коэффициент шероховатости")},
										{"dog",QStringLiteral(u"Диаметр оголовка")},
										{"nn",QStringLiteral(u"Начальное разбавление")},
										{"xn",QStringLiteral(u"Расстояние от створа выпуска")},
										{"ll",QStringLiteral(u"Расчетный участок реки")},
										{"pc",QStringLiteral(u"Коэффициент Шези")},
										{"pd",QStringLiteral(u"Коэффициент диффузии")},
										{"dz",QStringLiteral(u"dz")},
										{"dy",QStringLiteral(u"dy")},							
										{"dx",QStringLiteral(u"dx")},							
										{"rbb",QStringLiteral(u"Расстояние до берега (расч.)")},
										{"rb",QStringLiteral(u"Расстояния между оголовками (расч.)")},
										{"rw",QStringLiteral(u"Общая ширина (расч.)")},
										{"rhog",QStringLiteral(u"Высоты оголовков (расч.)")},
										{"rh",QStringLiteral(u"Глубина (расч.)")},
										{"rl",QStringLiteral(u"Помежуточные сечения (расч.)")},
										{"rll",QStringLiteral(u"Участок реки (расч.)")}};
	
								explicit	UK5Q_form(QWidget* parent = Q_NULLPTR);

									void	UK5Q_rewrite(const QString&);
	
	UK5B_varD								UK5Q_init(const QString&, bool, double) const;
	UK5B_varI								UK5Q_init(const QString&, bool, int) const;
	UK5B_varVD								UK5Q_init(const QString&, bool, std::vector<double>) const;
	UK5B_varVI								UK5Q_init(const QString&, bool, std::vector<int>) const;
	std::pair<UK5B_varD,int>				UK5Q_init(const QString&, bool, std::pair<double,int>) const;
	std::pair<UK5B_varVD,std::vector<int>>	UK5Q_init(const QString&, bool, std::pair<std::vector<double>,std::vector<int>>) const;
	
	UK5B_varD								UK5Q_recount(const QString&, double) const;
	UK5B_varI								UK5Q_recount(const QString&, int) const;
	UK5B_varVD								UK5Q_recount(const QString&, std::vector<double>) const;
	UK5B_varVI								UK5Q_recount(const QString&, std::vector<int>) const;
	std::pair<UK5B_varD,int>				UK5Q_recount(const QString&, std::pair<double,int>) const;
	std::pair<UK5B_varVD,std::vector<int>>	UK5Q_recount(const QString&, std::pair<std::vector<double>,std::vector<int>>) const;
	
									void	UK5Q_read(const QString&,	UK5B_varD&,	UK5Q_box*) const;
									void	UK5Q_read(const QString&,	UK5B_varI&,	UK5Q_box*) const;
									void	UK5Q_read(const QString&,	UK5B_varVD&,UK5Q_box*) const;
									void	UK5Q_read(const QString&,	UK5B_varVI&,UK5Q_box*) const;

public slots:
	void UK5Q_newtext(QString);
private:
	Ui::MainWindow* ui;
private slots:
	static void UK5Q_exit();
	void UK5Q_eval();
signals:
	void UK5Q_edit(QString);
};

