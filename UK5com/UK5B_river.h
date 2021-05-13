#pragma once
#include "UK5B_var.h"

class UK5B_river
{
public:
	UK5B_varD  vr,bb,h,dog,qst,cct,psh,nn,xn,ll,pc,pd,dx,dy,dz,rbb,rw,rh,rll;
	UK5B_varI  nog,nl,n;
	UK5B_varVD l,b,hog,rb,rl,rhog;
	
	UK5B_river() = default;
	static				double	UK5B_eval_dog(UK5B_varD);									//qst
	static				double	UK5B_eval_nn(UK5B_varD,UK5B_varD,UK5B_varD);				//vr dog qst
	static				double	UK5B_eval_xn(UK5B_varD,UK5B_varD,UK5B_varD);				//vr dog qst
	static				double	UK5B_eval_pc(UK5B_varD,UK5B_varD);							//h psh
	static				double	UK5B_eval_pd(UK5B_varD,UK5B_varD,UK5B_varD);				//vr h pc
	static				double	UK5B_eval_dydz(UK5B_varD,UK5B_varD,UK5B_varI,UK5B_varD);	//vr qst n nn
	static				double	UK5B_eval_dx(UK5B_varD,UK5B_varD,UK5B_varD);				//vr pd dydz
	static				double	UK5B_eval_rbb(UK5B_varD,UK5B_varD);							//bb dydz
	static std::vector<double>	UK5B_eval_rb(UK5B_varVD,UK5B_varI,UK5B_varD);				//b nl dydz
	static				double	UK5B_eval_rw(double,std::vector<double>,UK5B_varI);			//rbb rb nl
	static				double	UK5B_eval_rh(UK5B_varD,UK5B_varD);							//h dydz
	static std::vector<double>	UK5B_eval_rhog(UK5B_varVD,UK5B_varD);						//hog dydz
	static				double	UK5B_eval_rll(UK5B_varD,UK5B_varD);							//ll=500 dx
	static std::vector<double>	UK5B_eval_rl(UK5B_varVD,UK5B_varD);							//l dx
	static void UK5B_eval();

	
	
};

