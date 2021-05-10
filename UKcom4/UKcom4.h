#pragma once

// ReSharper disable once CppUnusedIncludeDirective
#include <vector>
#include <string>

#include "ui_UKcom4.h"


struct Param	// структура для передачи параметров в функцию расчета по Караушеву и вывода в файлы
{
	double VR = 0.15;	// скорость реки
	double qst = 0.011; // расход сточных вод на один оголовок = Qст / NOG

	mutable std::vector<double> b = { { 9.0, 9.0, 9.0, 9.0, 9.0, 9.0, 9.0, 9.0, 9.0, 9.0, 9.0, 9.0, 9.0, 9.0 } };	// массив расстояний между оголовками (NOG-1)

	double BB = 20.0;	// расстояние до берега
	double H = 1.5;	// глубина реки

	std::vector<double> l = {100.0};	// массив расстояний до промежуточных сечений
	size_t nl = 1;  // количество промежуточных сечений

	double HOG = 0.6;	// высота оголовка
	double CCT = 100; // исходное состояние загрязняющего вещества в сточных водах

	size_t N = 8;	// сторона расчетного квадрата
	size_t NOG = 15;	// количество оголовков
	
	double DOG = 0.3; //диаметр оголовка
	double psh = 0.025; //коэффициент шероховатости
	
	int	   flag = 0;	// управляющие флаги

};

class UKcom4 final : public QMainWindow
{
	Q_OBJECT

public:
	explicit UKcom4(QWidget *parent = Q_NULLPTR);
	~UKcom4();
	std::vector<double> ret = {0.};
	std::vector<double> retb = {0.};

	double NN_def = 10;
	double XN_def = 0;

private:
//Параметры
	Param P;
	double NN = -1.;
	double XN = -1.;
	std::string bs, bs2;
	int Chart;

	Ui::UKcom4Class *ui;

	std::vector<double> eval(const Param&, double, double, const std::string&, int);
	void viewCharts(QtCharts::QChartView*, std::vector<double>, double, double, size_t, size_t) const;
	QtCharts::QChart* chrt{};
private slots:
	void changeFlag(int);
	void makeEditable(int) const;
	void bigCharts() const;
	void setChart();
	void eval0();
	static void exit();

};
