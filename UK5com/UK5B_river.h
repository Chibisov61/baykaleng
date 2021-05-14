#pragma once
#include "UK5B_var.h"

class UK5B_river
{
public:
	UK5B_varD  vr, bb, h, dog, qst, cct, psh, nn, xn, ll, pc, pd, dx, dy, dz;
	UK5B_varI  nog,nl,n;
	UK5B_varVD b,l,hog;
	std::pair<UK5B_varD,int>  rbb,rw,rh,rll;
	std::pair<UK5B_varVD,std::vector<int>> rb, rhog ,rl;
	
	UK5B_river() = default;
	static				double	UK5B_eval_dog(const UK5B_varD&);																//qst
	static				double	UK5B_eval_nn(const UK5B_varD&,const UK5B_varD&,const UK5B_varD&);								//vr dog qst
	static				double	UK5B_eval_xn(const UK5B_varD&, const UK5B_varD&,const UK5B_varD&);								//vr dog qst
	static				double	UK5B_eval_pc(const UK5B_varD&, const UK5B_varD&);												//h psh
	static				double	UK5B_eval_pd(const UK5B_varD&, const UK5B_varD&, const UK5B_varD&);								//vr h pc
	static				double	UK5B_eval_dydz(const UK5B_varD&,const UK5B_varD&,const UK5B_varI&,const UK5B_varD&);			//vr qst n nn
	static				double	UK5B_eval_dx(const UK5B_varD&,const UK5B_varD&,const UK5B_varD&);								//vr pd dydz
	
	static	std::pair<double,int>							UK5B_eval_rbb(const UK5B_varD&,const UK5B_varD&);					//bb dydz
	static	std::pair<std::vector<double>,std::vector<int>>	UK5B_eval_rb(const UK5B_varVD&,const UK5B_varI&,const UK5B_varD&);	//b nl dydz
	static	std::pair<double,int>							UK5B_eval_rw(std::pair<UK5B_varD,int>,std::pair<UK5B_varVD,std::vector<int>>,const UK5B_varI&);	//rbb rb nl
	static	std::pair<double,int>							UK5B_eval_rh(const UK5B_varD&,const UK5B_varD&);					//h dydz
	static	std::pair<std::vector<double>,std::vector<int>>	UK5B_eval_rhog(const UK5B_varVD&,const UK5B_varD&);					//hog dydz
	static	std::pair<double,int>							UK5B_eval_rll(const UK5B_varD&,const UK5B_varD&);					//ll=500 dx
	static	std::pair<std::vector<double>,std::vector<int>>	UK5B_eval_rl(const UK5B_varVD&,const UK5B_varD&);					//l dx
	static void UK5B_eval();
	
};

