// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "UKcom4.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/tokenizer.hpp>
#include <boost/math/constants/constants.hpp>
#include <boost/filesystem.hpp>
#include <cmath>
#include <fstream>
#include <string>
#include <windows.h>
#include <QtWidgets/QMessageBox>
#include "bigChart.h"

using namespace std;
using namespace boost;
using namespace boost::property_tree;
using namespace boost::filesystem;
using namespace posix_time;

string Utf8_to_cp1251(const char* str)
{
	string res;

	const auto result_u = MultiByteToWideChar(CP_UTF8, 0, str, -1, nullptr, 0);

	if (!result_u)
		return nullptr;

	auto* ures = new wchar_t[result_u];

	if (!MultiByteToWideChar(CP_UTF8, 0, str, -1, ures, result_u))
	{
		delete[] ures;
		return nullptr;
	}

	const auto result_c = WideCharToMultiByte(1251, 0, ures, -1, nullptr, 0, nullptr, nullptr);

	if (!result_c)
	{
		delete[] ures;
		return nullptr;
	}

	auto* cres = new char[result_c];

	if (!WideCharToMultiByte(1251, 0, ures, -1, cres, result_c, nullptr, nullptr))
	{
		delete[] cres;
		return nullptr;
	}
	delete[] ures;
	res.append(cres);
	delete[] cres;
	return res;
}

// функция влияния дна
double f(double dh) {
	double dh0[11] = { 0.0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0 };
	double fdh0[11] = { 0.0,0.173,0.324,0.47,0.6,0.72,0.82,0.9,0.96,0.99,1.0 };	// данные получены из номограммы для определения поправочного коэффициента

	if (dh > 1.0) return -1;

	size_t ii = 0;
	for (size_t i = 0; i < 10; i++)
		if (dh < dh0[i + 1]) {
			ii = i;
			break;
		}
	const auto с = (dh0[ii + 1] - dh0[ii]) / (fdh0[ii + 1] - fdh0[ii]);	// линейная аппроксимация
	const auto fdh = (dh - dh0[ii]) / с + fdh0[ii];
	return fdh;
}

// расчет по Караушеву и вывод в файлы. отдельно передаются: диаметр оголовка, начальное разбавление,расстояние от створа выпуска до замыкающего створа зоны начального разбавления и имя выходного файла
std::vector<double> UKcom4::eval(const Param& PP, double nn, double xn, const string& f, int c)
{
	//выходные файлы
	std::ofstream fout;	// выходной файл csv
	std::ofstream fouts;// выходной файл txt для surfer-а
	std::ofstream ferr;	// файл сообщений об ошибке

	double max = 0;
	std::vector<double> vmax;

	//расчет коэффициента диффузии
//	auto const psh = 0.035;														// коэффициент шероховатости (взят из таблицы Срибнова М.Ф.)
	auto const g = 9.81;														// ускорение свободного падения

//	auto const gamma = 2.5*sqrt(psh) - 0.13 - 0.75*sqrt(PP.H)*(sqrt(psh)-0.1);	//
//	auto const C = pow(PP.H,gamma)/psh;									// коэффициент Шези по формуле Павловского Н.Н.

	auto const A1 = 1 / PP.psh - sqrt(g) * (1 - log10(PP.H)) / 0.13;
	auto const A2 = sqrt(g) * (1 / PP.psh + sqrt(g) * log10(PP.H)) / 0.13;

	auto const C = A1 / 2 + sqrt(A1 * A1 / 4 + A2);				//-V112			// коэффициент Шези по формуле Железнякова Г.В.

	auto const MM = C < 60 ? 0.75 * C + 6 : 48;										//
	auto const D = g * PP.H * PP.VR / MM / C;											// коэффициент диффузии (из Караушева А.В.)
//расчет параметров рабочего поля
	//dx,dy,dz
	const auto dn = PP.qst * nn / PP.VR;										// условная площадь поперечного сечения загрязненной струи
	const auto on = dn / static_cast<double>(PP.N) / static_cast<double>(PP.N);
	const auto dz = sqrt(on);
	const auto dy = dz;
	const auto dx = PP.VR * dy * dy / (4. * D);
	//ширина
	auto min = PP.b[0];
	auto* B = new size_t[PP.NOG - 1];								// массив попарных расстояний между оголовками
	size_t BNC = 0;
	auto BN = 0.;
	double tmp;
	string ppp, ppp2;													//для вывода в файл
	for (size_t i = 0; i < PP.NOG - 1; i++) {
		min = (PP.b[i] < min) ? PP.b[i] : min;					// находится минимальное расстояние между оголовками
		BN += PP.b[i];											// суммируем расстояния между оголовками в метрах
		B[i] = static_cast<size_t>(ceil(PP.b[i] / dy));		// переводим метры в ячейки.
		BNC += B[i];											// суммируем расстояния между оголовками в ячейках
		ppp += to_string(PP.b[i]).erase(to_string(PP.b[i]).size() - 2 - 2);	//формируем строку расстояний между оголовками разделенных
		ppp += (i == PP.NOG - 2) ? "" : ",";								//запятыми (2 знака после запятой)
		tmp = static_cast<double>(B[i])* dy;
		ppp2 += to_string(tmp).erase(to_string(tmp).size() - 2 - 2);	//формируем строку расстояний между оголовками разделенных
		ppp2 += (i == PP.NOG - 2) ? "" : ",";								//запятыми (2 знака после запятой)
	}
	BN += (5. * min);												// прибавляем метры после последнего оголовка
	const auto min2 = static_cast<size_t>(ceil((5. * min) / dy));	// ячейки после последнего оголовка
	BNC += min2;												// прибавляем ячейки после последнего оголовка
	//расстояние до берега
	const auto BBC = static_cast<size_t>(ceil((PP.BB) / dy));	// в ячейках
	//общая ширина
	const auto BMC = BNC + BBC;										// в ячейках
	//глубина
	const auto HMC = static_cast<size_t>(ceil(PP.H / dz));		// в ячейках
	//оголовок
	const auto HGC = static_cast<size_t>(round(PP.HOG / dz));	// в ячейках
	//длина общая
	const auto LM = 500. - xn;										// в метрах
	const auto LMC = static_cast<size_t>(ceil(LM / dx));			// в ячейках

	// массив расстояний до промежуточных срезов
	auto* lt = new double[PP.nl];
	auto* ltc = new size_t[PP.nl];
	string ttt, ttt2;
	for (size_t i = 0; i < PP.nl; i++)
	{
		lt[i] = (PP.l[i] > xn) ? PP.l[i] - xn : xn;										// в метрах
		ltc[i] = static_cast<size_t>(ceil(lt[i] / dx));			// в ячейках
		ttt += to_string(PP.l[i]).erase(to_string(PP.l[i]).size() - 2 - 2);	//формируем строку расстояний до промежуточных срезов разделенных
		ttt += (i == PP.nl - 1) ? "" : ",";							//запятыми (2 знака после запятой)
		tmp = static_cast<double>(ltc[i])* dx;
		ttt2 += to_string(tmp).erase(to_string(tmp).size() - 2 - 2);	//формируем строку расстояний до промежуточных срезов (рассчитываемых) разделенных
		ttt2 += (i == PP.nl - 1) ? "" : ",";							//запятыми (2 знака после запятой)
	}
	if (nn < DBL_EPSILON) {											// начальное разбавление не может быть равно нулю
		delete[] B;
		delete[] lt;
		delete[] ltc;
		vmax = { -2. };
		return vmax;												// поэтому выход с ошибкой
	}
	//загрязнение
	auto cct = PP.CCT / nn;										// загрязняющее вещество деленное на начальное разбавление
 //создание двух рабочих сечений
	auto** p0 = new double* [BMC + 2];
	auto** p1 = new double* [BMC + 2];
	for (size_t i = 0; i < BMC + 2; i++)
	{
		p0[i] = new double[HMC + 2];
		p1[i] = new double[HMC + 2];
	}
	//инициализация
	for (size_t j = 0; j < HMC + 2; j++)
		for (size_t k = 0; k < BMC + 2; k++)
		{
			p0[k][j] = 0;
			p1[k][j] = 0;
		}
	//начальное заполнение
	if (HGC + PP.N <= HMC)
	{
		//Основное поле
		size_t BBB = 0;
		const auto BBC0 = static_cast<size_t>(ceil(static_cast<double>(min2 - PP.N) / 2.));
		for (size_t l = 0; l < PP.NOG; l++)
		{
			for (size_t i = 0; i < PP.N; i++)
				for (size_t j = 0; j < PP.N; j++)
					p0[BBC0 + BBB + i + 1][HMC - j - HGC] = cct;
			if (l != PP.NOG - 1) BBB += B[PP.NOG - 2 - l];
		}
		//Дно
		for (size_t k = 0; k < BMC + 2; k++)
			p0[k][HMC + 1] = p0[k][HMC];
		//Воздух
		for (size_t k = 0; k < BMC + 2; k++)
			p0[k][0] = p0[k][1];
	}
	else
	{
		if ((PP.flag == 1) || (PP.flag == 3)) {
			ferr.open("error.csv", ios::out);
			// Вывод заголовка csv
			ferr << Utf8_to_cp1251("Скорость реки (VR): ") << PP.VR << endl;
			ferr << Utf8_to_cp1251("Расход сточных вод на один оголовок (qst): ") << PP.qst << endl;
			ferr << Utf8_to_cp1251("Диаметр оголовка (DOG): ") << PP.DOG << endl;

			ferr << Utf8_to_cp1251("Расстояние между оголовками: ") << ppp << ";;;;;;;;;;;;;;" << Utf8_to_cp1251("Рассчитываемое расстояние между всеми оголовками: ") << static_cast<double>(BNC)* dy << endl;
			ferr << Utf8_to_cp1251("Расстояние до берега (BB): ") << PP.BB << ";;;;;;;;;;;;;;" << Utf8_to_cp1251("Рассчитываемое расстояние до берега: ") << static_cast<double>(BBC)* dy << endl;
			ferr << Utf8_to_cp1251("Ширина рассматриваемого участка (BN+BB): ") << BN + PP.BB << ";;;;;;;;;;;;;;" << Utf8_to_cp1251("Рассчитываемая ширина: ") << static_cast<double>(BMC)* dy << endl;
			ferr << Utf8_to_cp1251("Средняя глубина реки (H): ") << PP.H << ";;;;;;;;;;;;;;" << Utf8_to_cp1251("Рассчитываемая глубина: ") << static_cast<double>(HMC)* dz << endl;
			ferr << Utf8_to_cp1251("Промежуточные сечения: ") << ttt << ";;;;;;;;;;;;;;" << Utf8_to_cp1251("Промежуточные сечения (рассчитываемые): ") << ttt2 << endl;
			ferr << Utf8_to_cp1251("Длина рассматриваемого участка (500): ") << 500 << ";;;;;;;;;;;;;;" << Utf8_to_cp1251("Рассчитываемая длина: ") << static_cast<double>(LMC)* dx << endl;

			ferr << Utf8_to_cp1251("Высота оголовка (HOG): ") << PP.HOG << ";;;;;;;;;;;;;;" << Utf8_to_cp1251("dx: ") << dx << endl;
			ferr << Utf8_to_cp1251("Начальное разбавление (NN): ") << nn << ";;;;;;;;;;;;;;" << Utf8_to_cp1251("dy: ") << dy << endl;
			ferr << Utf8_to_cp1251("Расстояние от створа выпуска до замыкающего створа зоны начального разбавления (XN): ") << xn << ";;;;;;;;;;;;;;" << Utf8_to_cp1251("dz: ") << dz << endl;

			ferr << Utf8_to_cp1251("Величина загрязняющего вещества (CCT): ") << PP.CCT << ";;;;;;;;;;;;;;" << Utf8_to_cp1251("Коэффициент турбулентной диффузии (D): ") << D << endl;;
			ferr << Utf8_to_cp1251("Сторона квадрата начального загрязнения (N): ") << static_cast<__int64>(PP.N) << ";;;;;;;;;;;;;;" << Utf8_to_cp1251("Коэффициент Шези (C): ") << C << endl;
			ferr << Utf8_to_cp1251("Количество оголовков (NOG): ") << static_cast<__int64>(PP.NOG) << endl;

			ferr << endl;
			//сообщение об ошибке и выход
			ferr << Utf8_to_cp1251("HGС: ") << to_string(HGC) << Utf8_to_cp1251(" HMC: ") << to_string(HMC) << endl;
			ferr << Utf8_to_cp1251("Нет достаточной глубины") << endl;
			ferr.close();
		}
		delete[] p0;
		delete[] p1;
		delete[] B;
		delete[] lt;
		delete[] ltc;
		vmax = { -1. };
		return vmax;
	}
	//Расчет и вывод
		//выходные файлы

	if ((PP.flag == 1) || (PP.flag == 3)) {
		string ff = f + ".csv";
		fout.open(ff, ios::out);
		// Вывод заголовка csv
		fout << Utf8_to_cp1251("Скорость реки (VR): ") << PP.VR << endl;
		fout << Utf8_to_cp1251("Расход сточных вод на один оголовок (qst): ") << PP.qst << endl;
		fout << Utf8_to_cp1251("Диаметр оголовка (DOG): ") << PP.DOG << endl;

		fout << Utf8_to_cp1251("Расстояние между оголовками: ") << ppp << ";;;;;;;;;;;;;;" << Utf8_to_cp1251("Рассчитываемое расстояние между всеми оголовками: ") << static_cast<double>(BNC)* dy << endl;
		fout << Utf8_to_cp1251("Расстояние до берега (BB): ") << PP.BB << ";;;;;;;;;;;;;;" << Utf8_to_cp1251("Рассчитываемое расстояние до берега: ") << static_cast<double>(BBC)* dy << endl;
		fout << Utf8_to_cp1251("Ширина рассматриваемого участка (BN+BB): ") << BN + PP.BB << ";;;;;;;;;;;;;;" << Utf8_to_cp1251("Рассчитываемая ширина: ") << static_cast<double>(BMC)* dy << endl;
		fout << Utf8_to_cp1251("Средняя глубина реки (H): ") << PP.H << ";;;;;;;;;;;;;;" << Utf8_to_cp1251("Рассчитываемая глубина: ") << static_cast<double>(HMC)* dz << endl;
		fout << Utf8_to_cp1251("Промежуточные сечения: ") << ttt << ";;;;;;;;;;;;;;" << Utf8_to_cp1251("Промежуточные сечения (рассчитываемые): ") << ttt2 << endl;
		fout << Utf8_to_cp1251("Длина рассматриваемого участка (500): ") << 500 << ";;;;;;;;;;;;;;" << Utf8_to_cp1251("Рассчитываемая длина: ") << static_cast<double>(LMC)* dx << endl;

		fout << Utf8_to_cp1251("Высота оголовка (HOG): ") << PP.HOG << ";;;;;;;;;;;;;;" << Utf8_to_cp1251("dx: ") << dx << endl;
		fout << Utf8_to_cp1251("Начальное разбавление (NN): ") << nn << ";;;;;;;;;;;;;;" << Utf8_to_cp1251("dy: ") << dy << endl;
		fout << Utf8_to_cp1251("Расстояние от створа выпуска до замыкающего створа зоны начального разбавления (XN): ") << xn << ";;;;;;;;;;;;;;" << Utf8_to_cp1251("dz: ") << dz << endl;

		fout << Utf8_to_cp1251("Величина загрязняющего вещества (CCT): ") << PP.CCT << ";;;;;;;;;;;;;;" << Utf8_to_cp1251("Коэффициент турбулентной диффузии (D): ") << D << endl;;
		fout << Utf8_to_cp1251("Сторона квадрата начального загрязнения (N): ") << static_cast<__int64>(PP.N) << ";;;;;;;;;;;;;;" << Utf8_to_cp1251("Коэффициент Шези (C): ") << C << endl;
		fout << Utf8_to_cp1251("Количество оголовков (NOG): ") << static_cast<__int64>(PP.NOG) << endl;

		fout << endl;
	}

	ui->comboBox_BN->setCurrentText(QString("%1").arg(BN + PP.BB));
	ui->comboBox_L500->setCurrentText(QString("%1").arg(500));

	if (!c)
	{
		ui->comboBox_BBR->setCurrentText(QString("%1").arg(static_cast<double>(BBC)* dy));
		ui->comboBox_br->setCurrentText(QString::fromStdString(ppp2));
		ui->comboBox_BNR->setCurrentText(QString("%1").arg(static_cast<double>(BMC)* dy));
		ui->comboBox_HR->setCurrentText(QString("%1").arg(static_cast<double>(HMC)* dz));
		ui->comboBox_bsr->setCurrentText(QString::fromStdString(ttt2));
		ui->comboBox_LR500->setCurrentText(QString("%1").arg(static_cast<double>(LMC)* dx));
		ui->comboBox_dx->setCurrentText(QString("%1").arg(dx));
		ui->comboBox_dy->setCurrentText(QString("%1").arg(dy));
		ui->comboBox_dz->setCurrentText(QString("%1").arg(dz));
	}

	vector<size_t> S;
	vector<string> ss;
	vector<string> SS;
	string sss;

	S.push_back(0);
	ss.push_back("0.00");
	SS.push_back(f + "-SRF0.00.txt");
	for (size_t s = 0; s < PP.nl; s++)
	{
		S.push_back(ltc[s]);
		sss = to_string(lt[s]).erase(to_string(lt[s]).size() - 2 - 2);
		ss.push_back(sss);
		SS.push_back(f + "-SRF" + sss + ".txt");
	}
	S.push_back(LMC - 1);
	ss.push_back("500.00");
	SS.push_back(f + "-SRF500.00.txt");

	size_t s0 = 0;

	ui->progressBar->setValue(0);

	if ((PP.flag == 1) || (PP.flag == 3)) {
		// вывод в csv среза 0
		for (size_t j = 0; j < HMC + 2; j++) {
			for (size_t k = 0; k < BMC + 2; k++)
				fout << p0[k][j] << ";";
			fout << endl;
		}
		fout << endl;
	}
	if ((PP.flag == 2) || (PP.flag == 3)) {
		//Вывод промежуточного среза 0 в txt для сёрфера
		fouts.open(SS[0], ios::out);
		for (size_t k = 0; k < BMC + 2; k++)
			for (size_t j = 0; j < HMC + 2; j++)
				fouts << to_string(k) << " " << to_string(j) << " " << p0[k][HMC + 1 - j] << endl;
		fouts.close();
	}

	for (size_t i = 1; i < LMC; i++)
	{
		//расчет
				// Основное поле
		for (size_t j = 1; j < HMC + 1; j++)
			for (size_t k = 1; k < BMC + 1; k++)
				p1[k][j] = (p0[k][j + 1] + p0[k + 1][j] + p0[k][j - 1] + p0[k - 1][j]) / 4;
		// Воздух
		for (size_t k = 0; k < BMC + 2; k++)
			p1[k][HMC + 1] = p1[k][HMC];
		// Дно
		for (size_t k = 0; k < BMC + 2; k++)
			p1[k][0] = p1[k][1];
		// Левая граница
		for (size_t j = 0; j < HMC + 2; j++)
			p1[0][j] = p1[1][j];
		// Берег
		for (size_t j = 0; j < HMC + 2; j++)
			p1[BMC + 1][j] = p1[BMC][j];
		//нахождение максимального загрязнения
		max = 0;
		for (size_t j = 1; j < HMC + 1; j++)
			for (size_t k = 1; k < BMC + 1; k++)
				max = (p1[k][j] < max ? max : p1[k][j]);
		vmax.push_back(max);

		for (size_t j = 0; j < HMC + 2; j++)
			for (size_t k = 0; k < BMC + 2; k++)
				p0[k][j] = p1[k][j];

		//вывод
		for (auto s = s0 + 1; s < static_cast<size_t>(PP.nl + 2); s++)
			if (S[s] == i) {
				s0 = s;
				if ((PP.flag == 1) || (PP.flag == 3)) {
					// вывод в csv среза S[i]
					for (size_t j = 0; j < HMC + 2; j++) {
						for (size_t k = 0; k < BMC + 2; k++)
							fout << p0[k][j] << ";";
						fout << endl;
					}
					fout << endl;
					// вывод в csv итогового значения
					fout << ss[s] << ";" << vmax[i - 1] << ";" << PP.CCT / vmax[i - 1] << endl << endl;
				}

				if ((PP.flag == 2) || (PP.flag == 3)) {
					//Вывод промежуточного среза в txt для сёрфера
					fouts.open(SS[s], ios::out);
					for (size_t k = 0; k < BMC + 2; k++)
						for (size_t j = 0; j < HMC + 2; j++)
							fouts << to_string(k) << " " << to_string(j) << " " << p0[k][HMC + 1 - j] << endl;
					fouts.close();
				}
			}

		auto ii = static_cast<double>(i) * 100. / (static_cast<double>(LMC) - 1.);
		ui->progressBar->setValue(static_cast<int>(ii));
	}

	// Завершение программы
	if ((PP.flag == 1) || (PP.flag == 3))
		fout.close();
	delete[] p0;
	delete[] p1;
	delete[] B;
	delete[] lt;
	delete[] ltc;
	return vmax;
}

void UKcom4::viewCharts(QtCharts::QChartView* chw, std::vector<double> r, const double tmin, const double tmax, const size_t tx, const size_t ty) const
{
	const auto res = minmax_element(r.begin(), r.end());

	auto low = *res.first;
	auto high = *res.second;

	low = static_cast<int>(P.CCT / low / 10.) * 10.;
	high = static_cast<int>((P.CCT / high / 10.) + 1) * 10.;

	auto ch = new QtCharts::QChart;
	ch->layout()->setContentsMargins(0, 0, 0, 0);
	ch->setBackgroundRoundness(0);
	ch->setMargins(QMargins(0, 0, 0, 0));
	ch->legend()->hide();
	chw->setRenderHint(QPainter::Antialiasing);
	chw->setChart(ch);

	const auto cit = r.size();
	const auto step = (tmax - tmin) / static_cast<double>(cit - 1);

	QPen pen;
	pen.setStyle(Qt::SolidLine);
	pen.setWidth(2);
	pen.setColor(Qt::red);
	auto* series = new QLineSeries();
	for (size_t j = 1; j < cit; j++)
		series->append(step * static_cast<double>(j - 1) + tmin, P.CCT / r[j]);
	series->setPen(pen);

	// ось Х
	auto* axisX = new QValueAxis;
	axisX->setRange(tmin, tmax);		// диапазон значений на оси X
	axisX->setTickCount(static_cast<int>(tx));				// число линий сетки
	axisX->setLabelFormat("%g");			// формат отображения чисел на оси X
	axisX->setGridLineVisible(true);

	// ось Y
	auto* axisY = new QValueAxis;
	axisY->setRange(low, high);		// диапазон значений на оси Y
	axisY->setTickCount(static_cast<int>(ty));					//-V112 // число линий сетки
	axisY->setLabelFormat("%g");			// формат отображения чисел на оси Y
	axisY->setGridLineVisible(true);

	ch->addSeries(series);
	ch->addAxis(axisX, Qt::AlignBottom);
	series->attachAxis(axisX);
	ch->addAxis(axisY, Qt::AlignLeft);
	series->attachAxis(axisY);
}

void UKcom4::makeEditable(int t) const {
	const auto group = qobject_cast<QGroupBox*>(sender()->parent());
	const string gn = group->objectName().toStdString();

	std::map <std::string, int> mapping;

	mapping["groupBox_NN"] = 1;
	mapping["groupBox_XN"] = 2;

	switch (mapping[gn])
	{
	case 1:
		ui->comboBox_NN->setCurrentText((t) ? QString("%1").arg(NN_def) : "-1");
		ui->comboBox_NN->setEnabled(t);
		break;
	case 2:
		ui->comboBox_XN->setCurrentText((t) ? QString("%1").arg(XN_def) : "-1");
		ui->comboBox_XN->setEnabled(t);
		break;
	default:
		break;
	}

	ui->radioButton_B->setEnabled(!(ui->checkBox_NN->isChecked() && ui->checkBox_XN->isChecked()));
}

void UKcom4::bigCharts() const
{
	auto bChart = new bigChart();

	viewCharts(bChart->ui->bchrt, retb, 0., 500., 21, 11);

	bChart->show();
}
void UKcom4::changeFlag(int t)
{
	const auto box = qobject_cast<QCheckBox*>(sender());
	const string bx = box->objectName().toStdString();

	std::map <std::string, int> mapping;

	mapping["checkBox_xls"] = 1;
	mapping["checkBox_srf"] = 2;

	switch (mapping[bx])
	{
	case 1:
		P.flag = (t) ? P.flag + 1 : P.flag - 1;
		break;
	case 2:
		P.flag = (t) ? P.flag + 2 : P.flag - 2;
		break;
	default:
		break;
	}
}
void UKcom4::setChart()
{
	const string rb = sender()->objectName().toStdString();

	std::map <std::string, int> mapping;

	mapping["radioButton_A"] = 1;
	mapping["radioButton_B"] = 2;

	Chart = mapping[rb];
}

void UKcom4::eval0()
{
	//константы
	const auto pi = boost::math::constants::pi<double>();
	const double dvm = 0.1;
	double MaxMax = 0;
	string ff;
	auto tt = second_clock::local_time();

	P.VR = ui->comboBox_VR->currentText().toDouble();
	P.qst = ui->comboBox_qst->currentText().toDouble();
	P.BB = ui->comboBox_BB->currentText().toDouble();
	P.H = ui->comboBox_H->currentText().toDouble();
	P.HOG = ui->comboBox_HOG->currentText().toDouble();
	P.CCT = ui->comboBox_CCT->currentText().toDouble();
	NN = (ui->comboBox_NN->isEnabled()) ? ui->comboBox_NN->currentText().toDouble() : -1;
	XN = (ui->comboBox_XN->isEnabled()) ? ui->comboBox_XN->currentText().toDouble() : -1;
	P.N = static_cast<size_t>(ui->comboBox_N->currentText().toInt());
	P.NOG = static_cast<size_t>(ui->comboBox_NOG->currentText().toInt());
	bs = ui->comboBox_b->currentText().toStdString();
	bs2 = ui->comboBox_bs->currentText().toStdString();
	P.DOG = (ui->comboBox_DOG->isEnabled()) ? ui->comboBox_DOG->currentText().toDouble() : -1;
	P.psh = ui->comboBox_psh->currentText().toDouble();

	std::vector<double> B;			// создаем вектор расстояний между оголовками

// разбираем строку bs разделенную ";" (и проверяем количество элементов с числом оголовков, если больше - обрезаем, если меньше - дополняем повторением последнего элемента)
	char_separator<char> sep(";");
	tokenizer< char_separator<char> > tokens(bs, sep);
	size_t nt = 0;
	for (const auto& t : tokens)
		if (nt < P.NOG)
		{
			B.push_back(stod(t));
			nt++;
		}
		else
			break;
	if (nt < P.NOG - 1)	// если в строке меньше значений чем расстояний между оголовками
		for (auto i = nt; i < P.NOG; i++)
			B.push_back(B[i % nt]);	// по циклу повторяем список
	P.b = std::move(B);			// заносим вектор в параметры

	std::vector<double> B2;	// создаем вектор расстояний между оголовками

// разбираем строку bs2 разделенную ";" (количество элементов неизвестно)
//	char_separator<char> sep2(";");
	tokenizer< char_separator<char> > tokens2(bs2, sep);
	size_t nt2 = 0;
	for (const auto& t : tokens2)
	{
		B2.push_back(stod(t));
		nt2++;
	}
	P.nl = nt2;
	P.l = std::move(B2);	// заносим в параметры вектор

	if (P.qst * P.VR * P.H * static_cast<double>(P.N) < DBL_EPSILON)
		ret[0] = -3.; //выход с ошибкой если хотя бы одно из значений VR,qst,H,N равно нулю.
	else
	{
		//расчет диаметра оголовка
		if (P.DOG == -1) P.DOG = pow(4 * P.qst / pi / 25, 0.4);

		if ((NN == -1) || (XN == -1))	// если хотя или NN или XN отсутствуют - считаем все варианты
		{
			//Расчет nnr и xnr
			auto vst = 2 * P.qst * 2 / (pi * P.DOG * P.DOG);											// V сточное
			auto m = P.VR / vst;																		// m
			auto dt = sqrt(8.1 / (((1 - m) * dvm * dvm / 0.92) + (2 * m * dvm / 0.96)));				// d с тильдой
			auto dzz = dt * P.DOG;																		// диаметр струи
			auto nnr = (0.248 * dt * dt / (1 - m)) * (sqrt(m * m + 8.1 * (1 - m) / dt / dt) - m);		// расчетное начальное разбавление
			if (P.H / dzz <= 1)
				nnr = nnr * f(P.H / dzz);																// проверка на влияние дна
			auto xnr = dzz / 0.48 / (1 - 3.12 * m);														// расчетное расстояние от створа выпуска до замыкающего створа зоны начального разбавления
			ui->comboBox_NN->setCurrentText(QString("%1").arg(nnr));
			ui->comboBox_XN->setCurrentText(QString("%1").arg(xnr));

			ff = "ukA." + to_iso_string(tt);
			ret = eval(P, 10., 0., ff, !(Chart - 1));
			MaxMax = ret.back();
			ui->comboBox_max->setCurrentText(QString("%1").arg(MaxMax));
			ui->comboBox_itog->setCurrentText(QString("%1").arg(P.CCT / MaxMax));
			if (Chart == 1)
			{
				viewCharts(ui->ChartView, ret, 0., 500., 11, 4);//-V112
				retb = ret;
			}

			if (ret[0] >= 0) { 				// если в первом случае ошибка второе уже не считаем
				ff = "ukB." + to_iso_string(tt);
				ret = eval(P, nnr, xnr, ff, Chart - 1);
				MaxMax = ret.back();
				ui->comboBox_max_B->setCurrentText(QString("%1").arg(MaxMax));
				ui->comboBox_itog_B->setCurrentText(QString("%1").arg(P.CCT / MaxMax));
				if (Chart == 2)
				{
					viewCharts(ui->ChartView, ret, 0., 500., 11, 4);//-V112
					retb = ret;
				}
			}
		}
		else
		{
			ui->radioButton_A->setChecked(true);
			ff = "uk." + to_iso_string(tt);
			ret = eval(P, NN, XN, ff, Chart - 1);
			MaxMax = ret.back();
			ui->comboBox_max->setCurrentText(QString("%1").arg(MaxMax));
			ui->comboBox_itog->setCurrentText(QString("%1").arg(P.CCT / MaxMax));
			ui->comboBox_max_B->setCurrentText(QString("%1").arg(0.));
			ui->comboBox_itog_B->setCurrentText(QString("%1").arg(0.));
			viewCharts(ui->ChartView, ret, 0., 500., 11, 4);	//-V112
		}
	}

	switch (static_cast<int>(ret.front()))
	{
	case -1:
		QMessageBox::critical(nullptr, "Ошибка!", "Нет достаточной глубины");
//		QApplication::quit();
		break;
	case -2:
		QMessageBox::critical(nullptr, "Ошибка!", "Начальное разбавление не может быть равно нулю");
//		QApplication::quit();
		break;
	case -3:
		QMessageBox::critical(nullptr, "Ошибка!", "Хотя бы одно из значений VR,qst,H,N равно нулю");
//		QApplication::quit();
		break;
	default:
		//			QMessageBox::warning(nullptr, "Внимание","Расчет произведен");
		break;
	}
}

void UKcom4::exit()
{
	QApplication::quit();
}

// основная программа
UKcom4::UKcom4(QWidget* parent)    // NOLINT
	: QMainWindow(parent)
{
	if (exists("config.ini"))
	{
		//чтение файла параметров
		ptree pt;
		read_ini("config.ini", pt);
		//ввод параметров
		P.VR = pt.get<double>("First.VR");
		P.qst = pt.get<double>("First.qst");

		bs = pt.get<string>("First.b");

		P.BB = pt.get<double>("First.BB", 0);
		P.H = pt.get<double>("First.H");

		bs2 = pt.get<string>("First.l");

		P.HOG = pt.get<double>("First.HOG", 0);
		P.CCT = pt.get<double>("First.CCT", 100);
		P.DOG = pt.get<double>("First.DOG",-1);
		P.psh = pt.get<double>("First.psh");

		NN = pt.get<double>("Second.NN", -1); // если отсутствуют в ini файле то присваиваем -1
		XN = pt.get<double>("Second.XN", -1); // аналогично

		P.N = pt.get<size_t>("Control.N", 8);
		P.NOG = pt.get<size_t>("Control.NOG", 5);

	}
	else
	{
		bs = "9.0";
		bs2 = "100.";
		NN = -1;
		XN = -1;
	}

	ui = new Ui::UKcom4Class();
	ui->setupUi(this);

	ui->comboBox_VR->setCurrentText(QString("%1").arg(P.VR));
	ui->comboBox_qst->setCurrentText(QString("%1").arg(P.qst));
	ui->comboBox_BB->setCurrentText(QString("%1").arg(P.BB));
	ui->comboBox_b->setCurrentText(QString::fromStdString(bs));
	ui->comboBox_H->setCurrentText(QString("%1").arg(P.H));
	ui->comboBox_bs->setCurrentText(QString::fromStdString(bs2));
	ui->comboBox_HOG->setCurrentText(QString("%1").arg(P.HOG));
	ui->comboBox_CCT->setCurrentText(QString("%1").arg(P.CCT));
	ui->comboBox_NN->setCurrentText(QString("%1").arg(NN));
	ui->comboBox_XN->setCurrentText(QString("%1").arg(XN));
	ui->comboBox_N->setCurrentText(QString("%1").arg(P.N));
	ui->comboBox_NOG->setCurrentText(QString("%1").arg(P.NOG));
	ui->comboBox_psh->setCurrentText(QString("%1").arg(P.psh));
	ui->comboBox_DOG->setCurrentText(QString("%1").arg(P.DOG));

	if (NN != -1)
	{
		ui->checkBox_NN->setCheckState(Qt::Checked);
		ui->comboBox_NN->setEnabled(true);
		NN_def = NN;
	};

	if (XN != -1)
	{
		ui->checkBox_XN->setCheckState(Qt::Checked);
		ui->comboBox_XN->setEnabled(true);
		XN_def = XN;
	};

/*	if (P.DOG != -1)
	{
		ui->checkBox_DOG->setCheckState(Qt::Checked);
		ui->comboBox_DOG->setEnabled(true);
//		DOG_def = P.DOG;
	}; */

	ui->radioButton_A->setChecked(true);
	ui->radioButton_B->setEnabled(!(ui->checkBox_NN->isChecked() && ui->checkBox_XN->isChecked()));
	Chart = 1;

	connect(ui->pushButton_ok, SIGNAL(clicked()), this, SLOT(eval0()));
	connect(ui->pushButton_exit, SIGNAL(clicked()), this, SLOT(exit()));

	connect(ui->checkBox_srf, SIGNAL(stateChanged(int)), this, SLOT(changeFlag(int)));
	connect(ui->checkBox_xls, SIGNAL(stateChanged(int)), this, SLOT(changeFlag(int)));

	connect(ui->checkBox_NN, SIGNAL(stateChanged(int)), this, SLOT(makeEditable(int)));
	connect(ui->checkBox_XN, SIGNAL(stateChanged(int)), this, SLOT(makeEditable(int)));

	connect(ui->zoom, SIGNAL(clicked()), this, SLOT(bigCharts()));

	connect(ui->radioButton_A, SIGNAL(clicked()), this, SLOT(setChart()));
	connect(ui->radioButton_B, SIGNAL(clicked()), this, SLOT(setChart()));
}

UKcom4::~UKcom4()
{
	delete ui;
}

// Проверка