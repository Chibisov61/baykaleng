#include "UK5B_river.h"


//UK5B_river::UK5B_river()
//{
//}

std::vector<std::vector<double>> UK5B_river::UK5B_karaush(std::vector<std::vector<double>> p)
{
	std::vector<std::vector<double>> r = p;

	const int hh = static_cast<int>(p.size());
	const int w = static_cast<int>(p.front().size());

	for (int i = 1; i < hh - 1; ++i)
		for (int j = 1; j < w - 1; ++j)
			r.at(i).at(j) = (p.at(i).at(j + 1) + p.at(i + 1).at(j) + p.at(i).at(j - 1) + p.at(i - 1).at(j)) / 4;

	for(int i = 0; i < hh; ++i)
	{
		r.at(i).at(0) = r.at(i).at(1);				// дно 
		r.at(i).at(w - 1) = r.at(i).at(w - 2);		// поверхность
	}

	for(int j = 0; j < w; ++j
	{
		r.at(0).at(j) = r.at(1).at(j);				// море
		r.at(hh - 1).at(j) = r.at(hh - 2).at(j);	// берег
	}

	std::vector<double> rr;
	for (int i = 0; i < hh; ++i)
		rr.push_back(*(std::max_element(r.at(i).begin(), r.at(i).end())));
	this->max = *(std::max_element(rr.begin(), rr.end()));
	
	return r;
}

void UK5B_river::UK5B_init_cut()
{
	const int				_h	= this->rh.second;
	const int				_w	= this->rw.second;
	const int				_n	= this->n.UK5B_getValue();
	const double			_t	= this->cct.UK5B_getValue();
	const int				_br = this->rbr.second;
	const int				_bl = this->rbl.second;
	const std::vector<int>	_b	= this->rb.second;
	const std::vector<int>	_ho = this->rhog.second;
	const double			_nn = this->nn.UK5B_getValue();
	
	const int no = static_cast<int>(_ho.size());
	const double tt = _t / _nn;

	cut = {};
		
	std::vector<int> _wo = {};
	int bb = _bl;
	_wo.push_back(bb);
	for (int k = 0; k < no; ++k) _wo.push_back(bb += _b.at(k));
	_wo.push_back(bb += _br);
	
	std::vector<std::pair<int, int>> _o = {};
	_o.reserve(no);
	for (int k = 0; k < no; ++k) _o.emplace_back(_ho.at(k), _wo.at(k));

	for (int i = 1; i < _h + 1; ++i)
	{
		std::vector<double> cut_inner;
		for (int j = 0; j < _w + 2; ++j)
		{
			bool e = false;
			for (int k = 0; k < no; ++k)
				if ((2 * i > 2 * _o.at(k).first - _n)  && 
					(2 * i < 2 * _o.at(k).first + _n + 2)  && 
					(2 * j > 2 * _o.at(k).second - _n) && 
					(2 * j < 2 * _o.at(k).second + _n + 2)) 
				{
					e = true;
					break;
				}
			cut_inner.push_back(e ? tt : 0);
		}
		cut.push_back(cut_inner);
		if (i == 1) cut.push_back(cut_inner);
		cut_inner = {};
	}
	
	cut.push_back(cut.back());

	this->max = tt;
}

double UK5B_river::UK5B_eval_dog(const UK5B_varD& qst)													//qst
{
	const double pi = 3.1415;
	const double _qst = qst.UK5B_getValue();
	
	return pow(4. * _qst / pi / 25., 0.4); 
}

double UK5B_river::UK5B_eval_vst(const UK5B_varD& qst, const UK5B_varD& dog)							//qst dog
{
	const double pi = 3.1415;
	const double _dog = dog.UK5B_getValue();								
	const double _qst = qst.UK5B_getValue();
	
	return (4 * _qst) / (pi * _dog * _dog);
}

double UK5B_river::UK5B_eval_dt(const UK5B_varD& vr, const UK5B_varD& vst)								//vr vst
{
	const double dvm = 0.1;
	const double _vr  = vr.UK5B_getValue();
	const double _vst  = vst.UK5B_getValue();

	const auto m = _vr / _vst;																	// m
	return sqrt(8.1 / (((1 - m) * dvm * dvm / 0.92) + (2 * m * dvm / 0.96)));
}

double UK5B_river::UK5B_eval_dzz(const UK5B_varD& vr, const UK5B_varD& qst, const UK5B_varD& dt, const UK5B_varD& vst)	//vr qst dt vst
{
	const double _qst = qst.UK5B_getValue();
	const double _vr  = vr.UK5B_getValue();
	const double _vst = vst.UK5B_getValue();								
	const double _dt  = dt.UK5B_getValue();

	const auto m = _vr / _vst;																	// m
	return ((m < 0.25) && (_vst < 2.)) ? (_qst / _vr) : (_qst * _dt * _dt / _vst);
}

double UK5B_river::UK5B_eval_nn(const UK5B_varD& vr, const UK5B_varD& vst, const UK5B_varD& dt)			//vr vst dt
{
	const double _vr  = vr.UK5B_getValue();
	const double _vst  = vst.UK5B_getValue();
	const double _dt  = dt.UK5B_getValue();

//	if (_h / dzz <= 1)
//	nnr = nnr * f(_h / dzz);																			// проверка на влияние дна
	
	const auto m = _vr / _vst;																	// m
	return ((m < 0.25) && (_vst < 2.)) ? 1. : (0.248 * _dt * _dt / (1 - m)) * (sqrt(m * m + 8.1 * (1 - m) / _dt / _dt) - m);
}

double UK5B_river::UK5B_eval_xn(const UK5B_varD& vr, const UK5B_varD& vst, const UK5B_varD& dzz)		//vr vst dzz
{
	const double _vr  = vr.UK5B_getValue();
	const double _vst = vst.UK5B_getValue();
	const double _dzz = dzz.UK5B_getValue();

	const auto m = _vr / _vst;																	// m
	return ((m < 0.25) && (_vst < 2.)) ? 0. : (_dzz / 0.48 / (1 - 3.12 * m));
}

double UK5B_river::UK5B_eval_pc(const UK5B_varD& h, const UK5B_varD& psh)								//h psh
{
	const double  g = 9.8110;
	const double _h = h.UK5B_getValue();
	const double _psh = psh.UK5B_getValue();

	auto const a1 = 1 / _psh - sqrt(g) * (1 - log10(_h)) / 0.13;
	auto const a2 = sqrt(g) * (1 / _psh + sqrt(g) * log10(_h)) / 0.13;
	return a1 / 2 + sqrt(a1 * a1 / 4 + a2);	//-V112
}

auto UK5B_river::UK5B_eval_pd(const UK5B_varD& vr, const UK5B_varD& h, const UK5B_varD& pc) -> double	//vr h pc
{
	const double  g = 9.8110;
	const double _vr  = vr.UK5B_getValue();
	const double _h	= h.UK5B_getValue();
	const double _pc  = pc.UK5B_getValue();
	
	auto const mm = _pc < 60 ? 0.75 * _pc + 6 : 48;									
	return g * _h * _vr / mm / _pc;																
}

double UK5B_river::UK5B_eval_dydz(const UK5B_varI& n, const UK5B_varD& dzz)		//vr qst n nn
{
	const int	 _n		= n.UK5B_getValue();
	const double _dzz	= dzz.UK5B_getValue();

	const auto on = _dzz / static_cast<double>(_n) / static_cast<double>(_n);
	return sqrt(on);
}

double UK5B_river::UK5B_eval_dx(const UK5B_varD& vr, const UK5B_varD& pd, const UK5B_varD& dydz)		//vr pd dydz
{
	const double _vr   = vr.UK5B_getValue();
	const double _pd   = pd.UK5B_getValue();
	const double _dydz = dydz.UK5B_getValue();

	return _vr * _dydz * _dydz/ (4. * _pd);
}

std::pair<double,int> UK5B_river::UK5B_eval_rbr(const UK5B_varD& br, const UK5B_varD& dydz)				//bb dydz
{
	const double _br   = br.UK5B_getValue();
	const double _dydz = dydz.UK5B_getValue();

	auto r2 = static_cast<int>(_br / _dydz);
	auto r1 = r2 * _dydz;

	auto r = std::make_pair(r1, r2);
	return r;
}

std::pair<double,int> UK5B_river::UK5B_eval_rbl(const UK5B_varD& bl, const UK5B_varD& dydz)				//bb dydz
{
	const double _bl   = bl.UK5B_getValue();
	const double _dydz = dydz.UK5B_getValue();

	auto r2 = static_cast<int>(_bl / _dydz);
	auto r1 = r2 * _dydz;

	auto r = std::make_pair(r1, r2);
	return r;
}

std::pair<std::vector<double>,std::vector<int>> UK5B_river::UK5B_eval_rb(const UK5B_varVD& b, const UK5B_varI& nog, const UK5B_varD& dydz)		//b nog dydz
{
	std::vector<double> _b		= b.UK5B_getValue();
	const int			_nog	= nog.UK5B_getValue();
	const double		_dydz	= dydz.UK5B_getValue();

	std::vector<double> r1	= {};
	std::vector<int>	r2	= {};

	const int z = static_cast<int>(_b.size());
	int ng = _nog;
						
	do													// по кругу заносим в rx данные из _b (пока не останется меньше z)
	{
		for (auto &i : _b)
		{
			r2.push_back(static_cast<int>(i / _dydz));
			r1.push_back(r2.back() * _dydz);
		}
	} while ((ng -= z) >= 0);

	for(int j=0; j < ng + z; j++)						// заносим остаток. В целом в rx должно оказаться _nog членов
	{
			r2.push_back(static_cast<int>(_b.at(j) / _dydz));
			r1.push_back(r2.back() * _dydz);
	}

	auto r = std::make_pair(r1, r2);
	return r;
}

std::pair<double,int> UK5B_river::UK5B_eval_rw(const std::pair<UK5B_varD,int>& rrbr, const std::pair<UK5B_varVD,std::vector<int>>& rrb, const std::pair<UK5B_varD,int>& rrbl)//rbb rb	
{
	double s1 = 0.;
	int    s2 = 0;

	const auto r1	= rrbr.first.UK5B_getValue();
	const auto r2		= rrbr.second;

	auto t1	= rrb.first.UK5B_getValue();
	auto t2    = rrb.second;

	const auto l1	= rrbl.first.UK5B_getValue();
	const auto l2	= rrbl.second;

	s1 += r1; 
	for( auto &i	: t1) s1 += i;
	s1 += l1;
	
	s2 += r2;
	for( auto &i	: t2) s2 += i;
	s2 += l2;

	auto r = std::make_pair(s1, s2);
	return r;
}

std::pair<double,int> UK5B_river::UK5B_eval_rh(const UK5B_varD& h, const UK5B_varD& dydz)				//h dydz
{
	const double _h	 = h.UK5B_getValue();
	const double _dydz = dydz.UK5B_getValue();

	int r2 = static_cast<int>(_h / _dydz);
	double r1 = r2 * _dydz;

	auto r = std::make_pair(r1, r2);
	return r;
}

std::pair<std::vector<double>,std::vector<int>> UK5B_river::UK5B_eval_rhog(const UK5B_varVD& hog, const UK5B_varI& nog, const UK5B_varD& dydz)	//hog dydz
{
	std::vector<double> _hog	= hog.UK5B_getValue();
	const int			_nog	= nog.UK5B_getValue();
	const double		_dydz	= dydz.UK5B_getValue();

	std::vector<double> r1	= {};
	std::vector<int>	r2	= {};

	const int z = static_cast<int>(_hog.size());
	int ng = _nog;
						
	do													// по кругу заносим в rx данные из _hog (пока не останется меньше z)
	{
		for (auto &i : _hog)
		{
			r2.push_back(static_cast<int>(i / _dydz));
			r1.push_back(r2.back() * _dydz);
		}
	} while ((ng -= z) > 0);

	for(int j=0; j < ng + z; j++)						// заносим остаток. В целом в rx должно оказаться _nog членов
	{
			r2.push_back(static_cast<int>(_hog.at(j) / _dydz));
			r1.push_back(r2.back() * _dydz);
	}
	
	auto r = std::make_pair(r1, r2);
	return r;
}

std::pair<double,int> UK5B_river::UK5B_eval_rll(const UK5B_varD& ll, const UK5B_varD& dx, const UK5B_varD& xn)	//ll=500 dx
{
	const double _ll  = ll.UK5B_getValue();
	const double _dx  = dx.UK5B_getValue();
	const double _xn  = xn.UK5B_getValue();

	int r2 = static_cast<int>((_ll - _xn)/ _dx);
	double r1 = r2 * _dx;

	auto r = std::make_pair(r1, r2);
	return r;
}

std::pair<std::vector<double>,std::vector<int>> UK5B_river::UK5B_eval_rl(const UK5B_varI& nl, const UK5B_varVD& l, const UK5B_varD& dx,const UK5B_varD& xn)	//nl l dx xn
{
	const int			_nl = nl.UK5B_getValue();
	std::vector<double>	_l	= l.UK5B_getValue();
	const double		_dx	= dx.UK5B_getValue();
	const double		_xn	= xn.UK5B_getValue();

	std::vector<double> r1	= {};
	std::vector<int>	r2	= {};

	const int z = static_cast<int>(_l.size());
	const int zz = _nl - z;
	
	if (zz > 0)
		for(int i=1; i < zz + 1; i++)
		{
			r2.push_back(i);
			r1.push_back(i * _dx);
		}
	
	for (auto &it : _l)
	{
		const int t = static_cast<int>((it > _xn) ? it - _xn : _xn / _dx);
		if (t > zz) {
			r2.push_back(static_cast<int>(t / _dx));
			r1.push_back(t);
		}
	}
	
	auto r = std::make_pair(r1, r2);
	return r;
}

