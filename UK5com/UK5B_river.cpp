#include "UK5B_river.h"
#include <fstream>

double UK5B_river::UK5B_eval_dog(UK5B_varD qst)													//qst
{
	const double pi = 3.1415;
	const double _qst = qst.UK5B_getValue();
	
	return pow(4. * _qst / pi / 25., 0.4);
}

double UK5B_river::UK5B_eval_nn(UK5B_varD vr, UK5B_varD dog, UK5B_varD qst)						//vr dog qst
{
	double pi = 3.1415;
	double dvm = 0.1;
	double _vr  = vr.UK5B_getValue();
	double _dog = dog.UK5B_getValue();
	double _qst = qst.UK5B_getValue();
	
	auto vst = 2 * _qst * 2 / (pi * _dog * _dog);											// V сточное
	auto m = _vr / vst;																	// m
	auto dt = sqrt(8.1 / (((1 - m) * dvm * dvm / 0.92) + (2 * m * dvm / 0.96)));		// d с тильдой
//	if (_h / dzz <= 1)
//	nnr = nnr * f(_h / dzz);																	// проверка на влияние дна

	return (0.248 * dt * dt / (1 - m)) * (sqrt(m * m + 8.1 * (1 - m) / dt / dt) - m);
}

double UK5B_river::UK5B_eval_xn(UK5B_varD vr, UK5B_varD dog, UK5B_varD qst)						//vr dog qst
{
	double pi = 3.1415;
	double dvm = 0.1;
	double _vr  = vr.UK5B_getValue();
	double _dog = dog.UK5B_getValue();
	double _qst = qst.UK5B_getValue();

	auto vst = 2 * _qst * 2 / (pi * _dog * _dog);											// V сточное
	auto m = _vr / vst;																	// m
	auto dt = sqrt(8.1 / (((1 - m) * dvm * dvm / 0.92) + (2 * m * dvm / 0.96)));		// d с тильдой
	auto dzz = dt * _dog;																	// диаметр струи
	
	return dzz / 0.48 / (1 - 3.12 * m);
}

double UK5B_river::UK5B_eval_pc(UK5B_varD h, UK5B_varD psh)										//h psh
{
	double  g = 9.8110;
	double _h = h.UK5B_getValue();
	double _psh = psh.UK5B_getValue();

	auto const a1 = 1 / _psh - sqrt(g) * (1 - log10(_h)) / 0.13;
	auto const a2 = sqrt(g) * (1 / _psh + sqrt(g) * log10(_h)) / 0.13;
	
	return a1 / 2 + sqrt(a1 * a1 / 4 + a2);	//-V112
}

double UK5B_river::UK5B_eval_pd(UK5B_varD vr, UK5B_varD h, UK5B_varD pc)						//vr h pc
{
	double  g = 9.8110;
	double _vr  = vr.UK5B_getValue();
	double _h	= h.UK5B_getValue();
	double _pc  = pc.UK5B_getValue();
	
	auto const mm = _pc < 60 ? 0.75 * _pc + 6 : 48;									

	return g * _h * _vr / mm / _pc;																
}

double UK5B_river::UK5B_eval_dydz(UK5B_varD vr, UK5B_varD qst, UK5B_varI n, UK5B_varD nn)		//vr qst n nn
{
	double _vr  = vr.UK5B_getValue();
	double _qst = qst.UK5B_getValue();
	   int _n	= n.UK5B_getValue();
	double _nn  = nn.UK5B_getValue();

	const auto dn = _qst * _nn / _vr;													// условная площадь поперечного сечения загрязненной струи
	const auto on = dn / static_cast<double>(_n) / static_cast<double>(_n);
	
	return sqrt(on);
}

double UK5B_river::UK5B_eval_dx(UK5B_varD vr, UK5B_varD pd, UK5B_varD dydz)						//vr pd dydz
{
	double _vr   = vr.UK5B_getValue();
	double _pd   = pd.UK5B_getValue();
	double _dydz = dydz.UK5B_getValue();

	return _vr * _dydz * _dydz/ (4. * _pd);
}

double UK5B_river::UK5B_eval_rbb(UK5B_varD bb, UK5B_varD dydz)									//bb dydz
{
	double _bb   = bb.UK5B_getValue();
	double _dydz = dydz.UK5B_getValue();

	return 0;
}

std::vector<double> UK5B_river::UK5B_eval_rb(UK5B_varVD b, UK5B_varI nl, UK5B_varD dydz)		//b nl dydz
{
	std::vector<double> _b	 =  b.UK5B_getValue();
					int _nl	 = nl.UK5B_getValue();
				double _dydz = dydz.UK5B_getValue();

	return {};
}

double UK5B_river::UK5B_eval_rw(double rbb, std::vector<double>, UK5B_varI nl)					//rbb rb nl
{
  int _nl  = nl.UK5B_getValue();

  return 0;
}

double UK5B_river::UK5B_eval_rh(UK5B_varD h, UK5B_varD dydz)									//h dydz
{
	double _h	 = h.UK5B_getValue();
	double _dydz = dydz.UK5B_getValue();

	return 0;
}

std::vector<double> UK5B_river::UK5B_eval_rhog(UK5B_varVD hog, UK5B_varD dydz)					//hog dydz
{
	std::vector<double> _hog  = hog.UK5B_getValue();
	            double  _dydz = dydz.UK5B_getValue();

	return {};
}

double UK5B_river::UK5B_eval_rll(UK5B_varD ll, UK5B_varD dx)									//ll=500 dx
{
	double _ll  = ll.UK5B_getValue();
	double _dx  = dx.UK5B_getValue();

	return 0;
}

std::vector<double> UK5B_river::UK5B_eval_rl(UK5B_varVD l, UK5B_varD dx)						//l dx
{
	std::vector<double> _l  = l.UK5B_getValue();
	            double _dx  = dx.UK5B_getValue();

	return {};
}

void UK5B_river::UK5B_eval()
{

	std::ofstream fout;
	fout.open("test.log", std::ios::out);
	fout << "Проверка связи" << std::endl;
	fout.close();
	
}
