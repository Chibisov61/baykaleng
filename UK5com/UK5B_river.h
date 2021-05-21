#pragma once
#include "UK5B_var.h"

class UK5B_river
{
public:
	UK5B_river() = default;
	
	UK5B_varD  vr, br, bl, h, dog, qst, cct, psh, nn, xn, ll, pc, pd, dx, dy, dz;
	UK5B_varI  nog,nl,n;
	UK5B_varVD b,l,hog;
	std::pair<UK5B_varD,int>  rbr,rbl,rw,rh,rll;
	std::pair<UK5B_varVD,std::vector<int>> rb, rhog ,rl;
	std::vector<std::vector<double>> cut = {{}};
	double max = 0;
	
	std::vector<std::vector<double>> UK5B_karaush(std::vector<std::vector<double>>);
	void UK5B_init_cut();
	
	static				double	UK5B_eval_dog(const UK5B_varD&);																				//qst
	static				double	UK5B_eval_nn(const UK5B_varD&,const UK5B_varD&,const UK5B_varD&);												//vr dog qst
	static				double	UK5B_eval_xn(const UK5B_varD&, const UK5B_varD&,const UK5B_varD&);												//vr dog qst
	static				double	UK5B_eval_pc(const UK5B_varD&, const UK5B_varD&);																//h psh
	static				double	UK5B_eval_pd(const UK5B_varD&, const UK5B_varD&, const UK5B_varD&);												//vr h pc
	static				double	UK5B_eval_dydz(const UK5B_varD&,const UK5B_varD&,const UK5B_varI&,const UK5B_varD&);							//vr qst n nn
	static				double	UK5B_eval_dx(const UK5B_varD&,const UK5B_varD&,const UK5B_varD&);												//vr pd dydz
	
	static	std::pair<double,int>							UK5B_eval_rbr(const UK5B_varD&,const UK5B_varD&);									//br dydz
	static	std::pair<std::vector<double>,std::vector<int>>	UK5B_eval_rb(const UK5B_varVD&,const UK5B_varI&,const UK5B_varD&);					//b nog dydz
	static	std::pair<double,int>							UK5B_eval_rbl(const UK5B_varD&,const UK5B_varD&);									//bl dydz
	static	std::pair<double,int>							UK5B_eval_rw(const std::pair<UK5B_varD,int>&,const std::pair<UK5B_varVD,std::vector<int>>&,const std::pair<UK5B_varD,int>&);//rbr rb rbl
	static	std::pair<double,int>							UK5B_eval_rh(const UK5B_varD&,const UK5B_varD&);									//h dydz
	static	std::pair<std::vector<double>,std::vector<int>>	UK5B_eval_rhog(const UK5B_varVD&,const UK5B_varI&,const UK5B_varD&);				//hog nog dydz
	static	std::pair<double,int>							UK5B_eval_rll(const UK5B_varD&,const UK5B_varD&,const UK5B_varD&);					//ll=500 dx xn
	static	std::pair<std::vector<double>,std::vector<int>>	UK5B_eval_rl(const UK5B_varI&, const UK5B_varVD&,const UK5B_varD&,const UK5B_varD&);//l dx xn
	
};