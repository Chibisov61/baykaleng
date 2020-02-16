#pragma once

// ReSharper disable once CppUnusedIncludeDirective
#include <QtWidgets/QMainWindow>
#include <vector>
#include <string>

#include "ui_UKcom4.h"
#include "bigChart.h"


struct Param	// ��������� ��� �������� ���������� � ������� ������� �� ��������� � ������ � �����
{
	double VR = 0.15;	// �������� ����
	double qst = 0.011; // ������ ������� ��� �� ���� �������� = Q�� / NOG

	mutable std::vector<double> b = { { 9.0, 9.0, 9.0, 9.0, 9.0, 9.0, 9.0, 9.0, 9.0, 9.0, 9.0, 9.0, 9.0, 9.0 } };	// ������ ���������� ����� ���������� (NOG-1)

	double BB = 20.0;	// ���������� �� ������
	double H = 1.5;	// ������� ����

	std::vector<double> l = {100.0};	// ������ ���������� �� ������������� �������
	size_t nl = 1;  // ���������� ������������� �������

	double HOG = 0.6;	// ������ ��������
	double CCT = 100; // �������� ��������� ������������� �������� � ������� �����

	size_t N = 8;	// ������� ���������� ��������
	size_t NOG = 15;	// ���������� ���������
	
	double DOG = 0.3; //������� ��������
	double psh = 0.025; //����������� �������������
	
	int	   flag = 0;	// ����������� �����

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
//���������
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
