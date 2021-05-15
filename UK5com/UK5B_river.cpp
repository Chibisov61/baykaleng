#include "UK5B_river.h"
#include <fstream>

double UK5B_river::UK5B_eval_dog(const UK5B_varD& qst)													//qst
{
	const double pi = 3.1415;
	const double _qst = qst.UK5B_getValue();
	
	return pow(4. * _qst / pi / 25., 0.4);
}

double UK5B_river::UK5B_eval_nn(const UK5B_varD& vr, const UK5B_varD& dog, const UK5B_varD& qst)						//vr dog qst
{
	const double pi = 3.1415;
	const double dvm = 0.1;
	const double _vr  = vr.UK5B_getValue();
	const double _dog = dog.UK5B_getValue();
	const double _qst = qst.UK5B_getValue();

	const auto vst = 2 * _qst * 2 / (pi * _dog * _dog);											// V сточное
	const auto m = _vr / vst;																	// m
	const auto dt = sqrt(8.1 / (((1 - m) * dvm * dvm / 0.92) + (2 * m * dvm / 0.96)));		// d с тильдой
//	if (_h / dzz <= 1)
//	nnr = nnr * f(_h / dzz);																	// проверка на влияние дна

	return (0.248 * dt * dt / (1 - m)) * (sqrt(m * m + 8.1 * (1 - m) / dt / dt) - m);
}

double UK5B_river::UK5B_eval_xn(const UK5B_varD& vr, const UK5B_varD& dog, const UK5B_varD& qst)						//vr dog qst
{
	const double pi = 3.1415;
	const double dvm = 0.1;
	const double _vr  = vr.UK5B_getValue();
	const double _dog = dog.UK5B_getValue();
	const double _qst = qst.UK5B_getValue();

	const auto vst = 2 * _qst * 2 / (pi * _dog * _dog);											// V сточное
	const auto m = _vr / vst;																	// m
	const auto dt = sqrt(8.1 / (((1 - m) * dvm * dvm / 0.92) + (2 * m * dvm / 0.96)));		// d с тильдой
	const auto dzz = dt * _dog;																	// диаметр струи
	
	return dzz / 0.48 / (1 - 3.12 * m);
}

double UK5B_river::UK5B_eval_pc(const UK5B_varD& h, const UK5B_varD& psh)										//h psh
{
	const double  g = 9.8110;
	const double _h = h.UK5B_getValue();
	const double _psh = psh.UK5B_getValue();

	auto const a1 = 1 / _psh - sqrt(g) * (1 - log10(_h)) / 0.13;
	auto const a2 = sqrt(g) * (1 / _psh + sqrt(g) * log10(_h)) / 0.13;
	
	return a1 / 2 + sqrt(a1 * a1 / 4 + a2);	//-V112
}

auto UK5B_river::UK5B_eval_pd(const UK5B_varD& vr, const UK5B_varD& h, const UK5B_varD& pc) -> double //vr h pc
{
	const double  g = 9.8110;
	const double _vr  = vr.UK5B_getValue();
	const double _h	= h.UK5B_getValue();
	const double _pc  = pc.UK5B_getValue();
	
	auto const mm = _pc < 60 ? 0.75 * _pc + 6 : 48;									

	return g * _h * _vr / mm / _pc;																
}

double UK5B_river::UK5B_eval_dydz(const UK5B_varD& vr, const UK5B_varD& qst, const UK5B_varI& n, const UK5B_varD& nn)		//vr qst n nn
{
	const double _vr	= vr.UK5B_getValue();
	const double _qst	= qst.UK5B_getValue();
	const int	 _n		= n.UK5B_getValue();
	const double _nn	= nn.UK5B_getValue();

	const auto dn = _qst * _nn / _vr;													// условная площадь поперечного сечения загрязненной струи
	const auto on = dn / static_cast<double>(_n) / static_cast<double>(_n);
	
	return sqrt(on);
}

double UK5B_river::UK5B_eval_dx(const UK5B_varD& vr, const UK5B_varD& pd, const UK5B_varD& dydz)												//vr pd dydz
{
	const double _vr   = vr.UK5B_getValue();
	const double _pd   = pd.UK5B_getValue();
	const double _dydz = dydz.UK5B_getValue();

	return _vr * _dydz * _dydz/ (4. * _pd);
}

std::pair<double,int> UK5B_river::UK5B_eval_rbb(const UK5B_varD& bb, const UK5B_varD& dydz)														//bb dydz
{
	const double _bb   = bb.UK5B_getValue();
	const double _dydz = dydz.UK5B_getValue();

	auto r2 = static_cast<int>(_bb / _dydz);
	auto r1 = r2 * _dydz;

	auto r = std::make_pair(r1, r2);
	return r;
}

std::pair<std::vector<double>,std::vector<int>> UK5B_river::UK5B_eval_rb(const UK5B_varVD& b, const UK5B_varI& nog, const UK5B_varD& dydz)		//b nl dydz
{
	std::vector<double> _b	 = b.UK5B_getValue();
					int _nog	 = nog.UK5B_getValue();
				double _dydz = dydz.UK5B_getValue();

	std::vector<double> r1	= {};
	std::vector<int>	r2	= {};

	if (_b.size() == static_cast<size_t>(_nog)-1)
	{
		for (auto &i : _b)
		{
			r2.push_back(static_cast<int>(i / _dydz));
			r1.push_back(r2.back() * _dydz);
		}
	}
	
	auto r = std::make_pair(r1, r2);
	return r;
}

std::pair<double,int> UK5B_river::UK5B_eval_rw(std::pair<UK5B_varD,int> rrbb, std::pair<UK5B_varVD,std::vector<int>> rrb, const UK5B_varI& nog)	//rbb rb nl	
{
	int _nog			= nog.UK5B_getValue();
	auto r1	= rrbb.first.UK5B_getValue();
	auto r2		= rrbb.second;

	auto t1	= rrb.first.UK5B_getValue();
	std::vector<int> t2;
	t2 = rrb.second;

	if (t2.size() == static_cast<size_t>(_nog)-1)
	{
		for( auto &i	: t1) r1 += i;
		for( auto &i	: t2) r2 += i;
	}

	r1 += *(std::min_element(t1.begin(), t1.end())) * 5.;
	r2 += *(std::min_element(t2.begin(), t2.end())) * 5;

	auto r = std::make_pair(r1, r2);
	return r;
}

std::pair<double,int> UK5B_river::UK5B_eval_rh(const UK5B_varD& h, const UK5B_varD& dydz)														//h dydz
{
	const double _h	 = h.UK5B_getValue();
	const double _dydz = dydz.UK5B_getValue();

	int r2 = static_cast<int>(_h / _dydz);
	double r1 = r2 * _dydz;

	auto r = std::make_pair(r1, r2);
	return r;
}

std::pair<std::vector<double>,std::vector<int>> UK5B_river::UK5B_eval_rhog(const UK5B_varVD& hog, const UK5B_varD& dydz)						//hog dydz
{
	std::vector<double> _hog  = hog.UK5B_getValue();
	            double  _dydz = dydz.UK5B_getValue();

	std::vector<double> r1	= {};
	std::vector<int>	r2	= {};

	for (auto &i : _hog)
	{
		r2.push_back(static_cast<int>(i / _dydz));
		r1.push_back(r2.back() * _dydz);
	}
	
	auto r = std::make_pair(r1, r2);
	return r;
}

std::pair<double,int> UK5B_river::UK5B_eval_rll(const UK5B_varD& ll, const UK5B_varD& dx, const UK5B_varD& xn)									//ll=500 dx
{
	const double _ll  = ll.UK5B_getValue();
	const double _dx  = dx.UK5B_getValue();
	const double _xn  = xn.UK5B_getValue();

	int r2 = static_cast<int>((_ll - _xn)/ _dx);
	double r1 = r2 * _dx;

	auto r = std::make_pair(r1, r2);
	return r;
}

std::pair<std::vector<double>,std::vector<int>> UK5B_river::UK5B_eval_rl(const UK5B_varVD& l, const UK5B_varD& dx,const UK5B_varD& xn)			//l dx xn
{
	std::vector<double>	_l	= l.UK5B_getValue();
	const double		_dx	= dx.UK5B_getValue();
	const double		_xn	= xn.UK5B_getValue();

	std::vector<double> r1	= {};
	std::vector<int>	r2	= {};

	for (auto &i : _l)
	{
		r2.push_back(static_cast<int>(((i >_xn) ? i-_xn : _xn)/ _dx));
		r1.push_back(r2.back() * _dx);
	}
	
	auto r = std::make_pair(r1, r2);
	return r;
}

void UK5B_river::UK5B_eval()
{

	std::ofstream fout;
	fout.open("test.log", std::ios::out);
	fout << "Проверка связи" << std::endl;
	fout.close();
	
}
