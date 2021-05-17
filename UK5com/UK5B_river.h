﻿#pragma once
#include "UK5B_var.h"
#include "UK5B_out.h"


class UK5B_river
{
public:
	UK5B_river() = default;
	
	UK5B_varD  vr, bb, h, dog, qst, cct, psh, nn, xn, ll, pc, pd, dx, dy, dz;
	UK5B_varI  nog,nl,n;
	UK5B_varVD b,l,hog;
	std::pair<UK5B_varD,int>  rbb,rw,rh,rll;
	std::pair<UK5B_varVD,std::vector<int>> rb, rhog ,rl;

	std::deque<std::deque<double>> cut = {{}};
	std::deque<std::deque<double>> UK5B_karaush(std::deque<std::deque<double>>);
	double max = 0;
	
	static				double	UK5B_eval_dog(const UK5B_varD&);																				//qst
	static				double	UK5B_eval_nn(const UK5B_varD&,const UK5B_varD&,const UK5B_varD&);												//vr dog qst
	static				double	UK5B_eval_xn(const UK5B_varD&, const UK5B_varD&,const UK5B_varD&);												//vr dog qst
	static				double	UK5B_eval_pc(const UK5B_varD&, const UK5B_varD&);																//h psh
	static				double	UK5B_eval_pd(const UK5B_varD&, const UK5B_varD&, const UK5B_varD&);												//vr h pc
	static				double	UK5B_eval_dydz(const UK5B_varD&,const UK5B_varD&,const UK5B_varI&,const UK5B_varD&);							//vr qst n nn
	static				double	UK5B_eval_dx(const UK5B_varD&,const UK5B_varD&,const UK5B_varD&);												//vr pd dydz
	
	static	std::pair<double,int>							UK5B_eval_rbb(const UK5B_varD&,const UK5B_varD&);									//bb dydz
	static	std::pair<std::vector<double>,std::vector<int>>	UK5B_eval_rb(const UK5B_varVD&,const UK5B_varI&,const UK5B_varD&);					//b nog dydz
	static	std::pair<double,int>							UK5B_eval_rw(std::pair<UK5B_varD,int>,std::pair<UK5B_varVD,std::vector<int>>);		//rbb rb
	static	std::pair<double,int>							UK5B_eval_rh(const UK5B_varD&,const UK5B_varD&);									//h dydz
	static	std::pair<std::vector<double>,std::vector<int>>	UK5B_eval_rhog(const UK5B_varVD&,const UK5B_varI&,const UK5B_varD&);				//hog nog dydz
	static	std::pair<double,int>							UK5B_eval_rll(const UK5B_varD&,const UK5B_varD&,const UK5B_varD&);					//ll=500 dx xn
	static	std::pair<std::vector<double>,std::vector<int>>	UK5B_eval_rl(const UK5B_varI&, const UK5B_varVD&,const UK5B_varD&,const UK5B_varD&);//l dx xn
	void UK5B_eval(UK5B_out*);

	void UK5B_init_cut();
	
};

