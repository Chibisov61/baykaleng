#pragma once

#include "ui_UK5Q_form.h"
#include "UK5B_river.h"
#include "UK5Q_box.h"
#include <vector>
#include <array>
#include <QMap>


class UK5Q_form : public QMainWindow
{
	Q_OBJECT

public:
	UK5B_river					river;
	std::array<QSize, 3>		p;
	QMap<QString, UK5Q_box*>	map_box;
	QMap<int, QVBoxLayout*>		map;
	QMap<QString,int>			bmap = {{"vr",1}, {"br",2},	{"bl",3}, {"nog",4},{"b",5},  {"h",6},  {"hog",7},{"nl",8},  {"l",9},	{"qst",10},{"cct",11},{"n",12},	  {"psh",13},{"dog",14},{"nn",15},
										{"xn",16},{"ll",17},{"pc",18},{"pd",19},{"dz",20},{"dy",21},{"dx",22},{"rbr",23},{"rbl",24},{"rb",25}, {"rw",26}, {"rhog",27},{"rh",28}, {"rl",29},	{"rll",30},
										{"vst",31},{"dt",32},{"dzz",33}};
	QMap<QString, QString>	lmap =	{	 {"vr",QStringLiteral(u"Скорость реки")},
										 {"br",QStringLiteral(u"Расстояние до правого берега")},
										 {"bl",QStringLiteral(u"Расстояние до левого берега")},
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
										{"vst",QStringLiteral(u"Скорость сточных вод (на оголовок)")},
										 {"dt",QStringLiteral(u"Относительный диаметр")},
										{"dzz",QStringLiteral(u"Диаметр пятна загрязнения")},
										 {"nn",QStringLiteral(u"Начальное разбавление")},
										 {"xn",QStringLiteral(u"Расстояние от створа выпуска")},
										 {"ll",QStringLiteral(u"Расчетный участок реки")},
										 {"pc",QStringLiteral(u"Коэффициент Шези")},
										 {"pd",QStringLiteral(u"Коэффициент диффузии")},
										 {"dz",QStringLiteral(u"dz")},
										 {"dy",QStringLiteral(u"dy")},							
										 {"dx",QStringLiteral(u"dx")},							
										{"rbr",QStringLiteral(u"Расстояние до правого берега (расч.)")},
										{"rbl",QStringLiteral(u"Расстояние до левого берега (расч.)")},
										 {"rb",QStringLiteral(u"Расстояния между оголовками (расч.)")},
										 {"rw",QStringLiteral(u"Общая ширина (расч.)")},
									   {"rhog",QStringLiteral(u"Высоты оголовков (расч.)")},
										 {"rh",QStringLiteral(u"Глубина (расч.)")},
										 {"rl",QStringLiteral(u"Помежуточные сечения (расч.)")},
										{"rll",QStringLiteral(u"Участок реки (расч.)")}};
								explicit	UK5Q_form(QWidget* parent = Q_NULLPTR);
	
									void	viewCharts(QChartView*, std::vector<double>, double, double, int, int) const;
									void	UK5Q_rewrite(const QString&);
	
	UK5B_varD								UK5Q_init(const QString&, bool, double);
	UK5B_varI								UK5Q_init(const QString&, bool, int);
	UK5B_varVD								UK5Q_init(const QString&, bool, std::vector<double>);
	UK5B_varVI								UK5Q_init(const QString&, bool, std::vector<int>);
	std::pair<UK5B_varD,int>				UK5Q_init(const QString&, bool, std::pair<double,int>);
	std::pair<UK5B_varVD,std::vector<int>>	UK5Q_init(const QString&, bool, std::pair<std::vector<double>,std::vector<int>>);
	
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

