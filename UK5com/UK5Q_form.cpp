// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "UK5Q_form.h"
#include "UK5Q_box.h"
#include "UK5B_river.h"
#include "UK5B_out.h"

UK5Q_form::UK5Q_form(QWidget *parent)
	: QMainWindow(parent)
{
	const bool x = QFile::exists("config.ini");

	ui = new Ui::MainWindow();
	ui->setupUi(this);

	ui->UK5Q_progressBar->setValue(0);
	
	map.insert(0, ui->UK5Q_scrollAreaWidgetContents_IN);
	map.insert(1, ui->UK5Q_scrollAreaWidgetContents_IN_OUT);
	map.insert(2, ui->UK5Q_scrollAreaWidgetContents_OUT);

	river = {};

	const std::vector<double> dnul = {0.};

//in
	river.vr	=  UK5Q_init("vr"	,x,0.);
	river.bb	=  UK5Q_init("bb"	,x,0.); 
	river.nog	=  UK5Q_init("nog",x,0);
	river.b		=  UK5Q_init("b"	,x,dnul);
	river.h		=  UK5Q_init("h"	,x,0.);
	river.hog	=  UK5Q_init("hog",x,dnul);
	river.nl	=  UK5Q_init("nl"	,x,0);
	river.l		=  UK5Q_init("l"	,x,dnul);
	river.qst	=  UK5Q_init("qst",x,0.);
	river.cct	=  UK5Q_init("cct",x,0.);
	river.n		=  UK5Q_init("n"	,x,0);
	river.psh	=  UK5Q_init("psh",x,0.);
//in-out
	river.dog	=  UK5Q_init("dog",x,0.);
	river.nn 	=  UK5Q_init("nn"	,x,0.);
	river.xn 	=  UK5Q_init("xn"	,x,0.);
	river.ll	=  UK5Q_init("ll"	,x,500.);
	river.pd	=  UK5Q_init("pd"	,x,0.07);
//out	
//	river.ll	=  UK5Q_init("ll"  ,false	,500.);
	river.pc	=  UK5Q_init("pc"  ,false	,UK5B_river::UK5B_eval_pc(river.h,river.psh));
//	river.pd	=  UK5Q_init("pd"  ,false	,UK5B_river::UK5B_eval_pd(river.vr,river.h,river.pc));
	river.dz	=  UK5Q_init("dz"  ,false	,UK5B_river::UK5B_eval_dydz(river.vr,river.qst,river.n,river.nn));
	river.dy	=  UK5Q_init("dy"  ,false	,UK5B_river::UK5B_eval_dydz(river.vr,river.qst,river.n,river.nn));
	river.dx	=  UK5Q_init("dx"  ,false	,UK5B_river::UK5B_eval_dx(river.vr,river.pd,river.dy));
	river.rbb	=  UK5Q_init("rbb" ,false	,UK5B_river::UK5B_eval_rbb(river.bb,river.dy));
	river.rb	=  UK5Q_init("rb"  ,false	,UK5B_river::UK5B_eval_rb(river.b,river.nog,river.dy));
	river.rw	=  UK5Q_init("rw"  ,false	,UK5B_river::UK5B_eval_rw(river.rbb,river.rb));
	river.rhog	=  UK5Q_init("rhog",false	,UK5B_river::UK5B_eval_rhog(river.hog,river.nog,river.dy));
	river.rh	=  UK5Q_init("rh"  ,false	,UK5B_river::UK5B_eval_rh(river.h,river.dy));
	river.rl	=  UK5Q_init("rl"  ,false	,UK5B_river::UK5B_eval_rl(river.nl,river.l,river.dx,river.xn));
	river.rll	=  UK5Q_init("rll" ,false	,UK5B_river::UK5B_eval_rll(river.ll,river.dx,river.xn));

	river.UK5B_init_cut();

	connect(ui->UK5Q_Exit, SIGNAL(clicked()), this, SLOT(UK5Q_exit()));
	connect(ui->UK5Q_Eval, SIGNAL(clicked()), this, SLOT(UK5Q_eval()));

}

void UK5Q_form::UK5Q_exit()
{
	QApplication::quit();
}

void UK5Q_form::UK5Q_eval()
{
	const QString f = "UK5." + QDateTime::currentDateTime().toString("yyyyMMddTHHmmss") + ".csv";
	UK5B_out print(f.toStdString());
	
	print.UK5B_header_print(river);
	print.UK5B_body_print(0, river);

	ui->UK5Q_progressBar->setValue(0);

	const int lll = river.rll.second;
	for(int i = 1; i < lll + 1; ++i)
	{
		river.cut = river.UK5B_karaush(river.cut);
		if ((std::binary_search(river.rl.second.begin(), river.rl.second.end(), i)) || (i == lll))
			print.UK5B_body_print(i,river);
		const auto ii = static_cast<double>(i) * 100. / (static_cast<double>(lll) - 1.);
		ui->UK5Q_progressBar->setValue(static_cast<int>(ii));
	}
	
	river.UK5B_init_cut();
}	

void UK5Q_form::UK5Q_rewrite(const QString& s)
{
	const auto box = map_box[s];

	switch (bmap[s])
	{
	case 1:			//vr
		UK5Q_read(s, river.vr, box);
		if (!map_box["nn"]->UK5Q_getMode()) { river.nn = UK5Q_recount("nn", UK5B_river::UK5B_eval_nn(river.vr, river.qst, river.dog));	UK5Q_rewrite("nn"); }
		if (!map_box["xn"]->UK5Q_getMode()) { river.xn = UK5Q_recount("xn", UK5B_river::UK5B_eval_xn(river.vr, river.qst, river.dog));	UK5Q_rewrite("xn"); }
		river.pd = UK5Q_recount("pd",UK5B_river::UK5B_eval_pd(river.vr,river.h,river.pc));												UK5Q_rewrite("pd");
		break;
	case 2:			//bb
		UK5Q_read(s, river.bb, box);
		river.rbb = UK5Q_recount("rbb",UK5B_river::UK5B_eval_rbb(river.bb,river.dy));				UK5Q_rewrite("rbb");
		break;
	case 3:			//nog
		UK5Q_read(s, river.nog, box);
		river.rb	= UK5Q_recount("rb",UK5B_river::UK5B_eval_rb(river.b,river.nog,river.dy));		UK5Q_rewrite("rb");
		river.rhog = UK5Q_recount("rhog",UK5B_river::UK5B_eval_rhog(river.hog,river.nog,river.dy));				
		break;
	case 4:			//b
		UK5Q_read(s, river.b, box);
		river.rb = UK5Q_recount("rb",UK5B_river::UK5B_eval_rb(river.b,river.nog,river.dy));			UK5Q_rewrite("rb");
		break;
	case 5:			//h
		UK5Q_read(s, river.h, box);
		river.pc = UK5Q_recount("pc",UK5B_river::UK5B_eval_pc(river.h,river.psh));					UK5Q_rewrite("pc");
		river.rh = UK5Q_recount("rh",UK5B_river::UK5B_eval_rh(river.h,river.dy));					
		break;
	case 6:			//hog
		UK5Q_read(s, river.hog, box);
		river.rhog = UK5Q_recount("rhog",UK5B_river::UK5B_eval_rhog(river.hog,river.nog,river.dy));				
		break;
	case 7:			//nl
		UK5Q_read(s, river.nl, box);
		river.rl = UK5Q_recount("rl",UK5B_river::UK5B_eval_rl(river.nl,river.l,river.dx,river.xn));	UK5Q_rewrite("rl");
		break;
	case 8:			//l
		UK5Q_read(s, river.l, box);
		river.rl = UK5Q_recount("rl",UK5B_river::UK5B_eval_rl(river.nl,river.l,river.dx,river.xn));			
		break;
	case 9:			//qst
		UK5Q_read(s, river.qst, box);
		if (!map_box["nn"]->UK5Q_getMode())  { river.nn = UK5Q_recount("nn", UK5B_river::UK5B_eval_nn(river.vr, river.qst, river.dog));	UK5Q_rewrite("nn");  }
		if (!map_box["xn"]->UK5Q_getMode())  { river.xn = UK5Q_recount("xn", UK5B_river::UK5B_eval_xn(river.vr, river.qst, river.dog));	UK5Q_rewrite("xn");  }
		if (!map_box["dog"]->UK5Q_getMode()) { river.dog = UK5Q_recount("dog", UK5B_river::UK5B_eval_dog(river.qst));					UK5Q_rewrite("dog"); }
		break;
	case 11:		//n
		UK5Q_read(s, river.n, box);
		river.dz = UK5Q_recount("dz",UK5B_river::UK5B_eval_dydz(river.vr,river.qst,river.n,river.nn));	UK5Q_rewrite("dz");
		river.dy = UK5Q_recount("dy",UK5B_river::UK5B_eval_dydz(river.vr,river.qst,river.n,river.nn));	UK5Q_rewrite("dy");
		break;
	case 12:		//psh
		UK5Q_read(s, river.psh, box);
		river.pc = UK5Q_recount("pc",UK5B_river::UK5B_eval_pc(river.h,river.psh));						UK5Q_rewrite("pc");
		break;
	case 13:		//dog
		UK5Q_read(s, river.dog, box);
		if (!map_box["nn"]->UK5Q_getMode()) { river.nn = UK5Q_recount("nn", UK5B_river::UK5B_eval_nn(river.vr, river.qst, river.dog));	UK5Q_rewrite("nn"); }
		if (!map_box["xn"]->UK5Q_getMode()) { river.xn = UK5Q_recount("xn", UK5B_river::UK5B_eval_xn(river.vr, river.qst, river.dog));	UK5Q_rewrite("xn"); }
		break;
	case 14:		//nn
		UK5Q_read(s, river.nn, box);
		river.dz = UK5Q_recount("dz",UK5B_river::UK5B_eval_dydz(river.vr,river.qst,river.n,river.nn));	UK5Q_rewrite("dz");
		river.dy = UK5Q_recount("dy",UK5B_river::UK5B_eval_dydz(river.vr,river.qst,river.n,river.nn));	UK5Q_rewrite("dy");
		break;
	case 15:		//xn
		UK5Q_read(s, river.xn, box);
		river.rl	= UK5Q_recount("rl",UK5B_river::UK5B_eval_rl(river.nl,river.l,river.dx,river.xn));		
		river.rll	= UK5Q_recount("rll",UK5B_river::UK5B_eval_rll(river.ll,river.dx,river.xn));		
		break;
	case 17:		//pc
		river.pd	= UK5Q_recount("pd",UK5B_river::UK5B_eval_pd(river.vr,river.h,river.pc));			UK5Q_rewrite("pd");
		break;
	case 18:		//pd
		river.dx	= UK5Q_recount("dx",UK5B_river::UK5B_eval_dx(river.vr,river.pd,river.dy));			UK5Q_rewrite("dx");
		break;
	case 19:		//dz
	case 20:		//dy
		river.dx	= UK5Q_recount("dx",UK5B_river::UK5B_eval_dx(river.vr,river.pd,river.dy));			UK5Q_rewrite("dx");
		river.rbb	= UK5Q_recount("rbb",UK5B_river::UK5B_eval_rbb(river.bb,river.dy));					UK5Q_rewrite("rbb");
		river.rb	= UK5Q_recount("rb",UK5B_river::UK5B_eval_rb(river.b,river.nog,river.dy));			UK5Q_rewrite("rb");
		river.rhog	= UK5Q_recount("rhog",UK5B_river::UK5B_eval_rhog(river.hog,river.nog,river.dy));				
		river.rh	= UK5Q_recount("rh",UK5B_river::UK5B_eval_rh(river.h,river.dy));					
		break;
	case 21:		//dx
		river.rl	= UK5Q_recount("rl",UK5B_river::UK5B_eval_rl(river.nl,river.l,river.dx,river.xn));		
		river.rll	= UK5Q_recount("rll",UK5B_river::UK5B_eval_rll(river.ll,river.dx,river.xn));
		break;
	case 22:		//rbb
	case 23:		//rb
		river.rw = UK5Q_recount("rw",UK5B_river::UK5B_eval_rw(river.rbb,river.rb));		
		break;
	default: ;
		break;
	}
}

UK5B_varD UK5Q_form::UK5Q_init(const QString& s, const bool x, const double def) const
{
	UK5B_varD u;
	u.UK5B_setName(s.toStdString());
	u.UK5B_setValue(x, def);
	const int place = u.UK5B_getPlace();

	auto box = new UK5Q_box(map[place]);
	box->setObjectName("UK5Q_BOX_"+s);
	map_box.key(box);
	map_box.value(s);
	connect(box, SIGNAL(UK5Q_edit(QString)), this, SLOT(UK5Q_newtext(QString)));
	
	box->UK5Q_setLabel(lmap[s]);
	box->UK5Q_setValue(u.UK5B_getValue());

	box->UK5Q_setMode((place !=1 ) ? false : true);
	box->UK5Q_state((place == 2) ? 0 : 2);
	
	box->show();
	return u;
}

UK5B_varI UK5Q_form::UK5Q_init(const QString& s, const bool x, const int def) const
{
	UK5B_varI u;
	u.UK5B_setName(s.toStdString());
	u.UK5B_setValue(x, def);
	const int place = u.UK5B_getPlace();

	auto box = new UK5Q_box(map[place]);
	box->setObjectName("UK5Q_BOX_"+s);
	map_box.key(box);
	map_box.value(s);
	connect(box, SIGNAL(UK5Q_edit(QString)), this, SLOT(UK5Q_newtext(QString)));
	
	box->UK5Q_setLabel(lmap[s]);
	box->UK5Q_setValueI(u.UK5B_getValue());
	
	box->UK5Q_setMode((place !=1 ) ? false : true);
	box->UK5Q_state((place == 2) ? 0 : 2);
	
	box->show();
	return u;
}

UK5B_varVD UK5Q_form::UK5Q_init(const QString& s, const bool x, std::vector<double> def) const
{
	UK5B_varVD u;
	u.UK5B_setName(s.toStdString());
	u.UK5B_setValue(x, def);
	const int place = u.UK5B_getPlace();

	auto box = new UK5Q_box(map[place]);
	box->setObjectName("UK5Q_BOX_"+s);
	map_box.key(box);
	map_box.value(s);
	connect(box, SIGNAL(UK5Q_edit(QString)), this, SLOT(UK5Q_newtext(QString)));
	
	box->UK5Q_setLabel(lmap[s]);
	box->UK5Q_setVector(u.UK5B_getValue());

	box->UK5Q_setMode((place !=1 ) ? false : true);
	box->UK5Q_state((place == 2) ? 0 : 2);
	
	box->show();
	return u;
}

UK5B_varVI UK5Q_form::UK5Q_init(const QString& s, const bool x, std::vector<int> def) const
{
	UK5B_varVI u;
	u.UK5B_setName(s.toStdString());
	u.UK5B_setValue(x, def);
	const int place = u.UK5B_getPlace();

	auto box = new UK5Q_box(map[place]);
	box->setObjectName("UK5Q_BOX_"+s);
	map_box.key(box);
	map_box.value(s);
	connect(box, SIGNAL(UK5Q_edit(QString)), this, SLOT(UK5Q_newtext(QString)));
	
	box->UK5Q_setLabel(lmap[s]);
	box->UK5Q_setVectorI(u.UK5B_getValue());
	
	box->UK5Q_setMode((place !=1 ) ? false : true);
	box->UK5Q_state((place == 2) ? 0 : 2);
	
	box->show();
	return u;

}

std::pair<UK5B_varD, int> UK5Q_form::UK5Q_init(const QString& s, const bool x, std::pair<double, int> def) const
{
	UK5B_varD u1;
	int u2 = def.second;
	auto u = std::make_pair(u1, u2);
	u.first.UK5B_setName(s.toStdString());
	u.first.UK5B_setValue(x, def.first);
	const int place = u.first.UK5B_getPlace();

	auto box = new UK5Q_box(map[place]);
	box->setObjectName("UK5Q_BOX_"+s);
	map_box.key(box);
	map_box.value(s);
	connect(box, SIGNAL(UK5Q_edit(QString)), this, SLOT(UK5Q_newtext(QString)));
	
	box->UK5Q_setLabel(lmap[s]);
	box->UK5Q_setValue(u.first.UK5B_getValue());

	box->UK5Q_setMode((place !=1 ) ? false : true);
	box->UK5Q_state((place == 2) ? 0 : 2);
	
	box->show();
	return u;
}

std::pair<UK5B_varVD, std::vector<int>> UK5Q_form::UK5Q_init(const QString& s, const bool x, std::pair<std::vector<double>, std::vector<int>> def) const
{
	UK5B_varVD u1;
	std::vector<int> u2 = def.second;
	auto u = std::make_pair(u1, u2);
	u.first.UK5B_setName(s.toStdString());
	u.first.UK5B_setValue(x, def.first);
	const int place = u.first.UK5B_getPlace();

	auto box = new UK5Q_box(map[place]);
	box->setObjectName("UK5Q_BOX_"+s);
	map_box.key(box);
	map_box.value(s);
	connect(box, SIGNAL(UK5Q_edit(QString)), this, SLOT(UK5Q_newtext(QString)));
	
	box->UK5Q_setLabel(lmap[s]);
	box->UK5Q_setVector(u.first.UK5B_getValue());
	
	box->UK5Q_setMode((place !=1 ) ? false : true);
	box->UK5Q_state((place == 2) ? 0 : 2);
	
	box->show();
	return u;
	
}

UK5B_varD UK5Q_form::UK5Q_recount(const QString& s, double def) const
{
	UK5B_varD u;
	const auto box = map_box[s];
		
	u.UK5B_setName(s.toStdString());
	u.UK5B_setValue(false, def);
	u.UK5B_setPlace(map.key(qobject_cast<UK5Q_box*>(box->UK5Q_getPlace())));

	box->UK5Q_setValue(u.UK5B_getValue());
	box->show();

	return u;
}

UK5B_varI UK5Q_form::UK5Q_recount(const QString& s, int def) const
{
	UK5B_varI u;
	const auto box = map_box[s];
		
	u.UK5B_setName(s.toStdString());
	u.UK5B_setValue(false, def);
	u.UK5B_setPlace(map.key(qobject_cast<UK5Q_box*>(box->UK5Q_getPlace())));

	box->UK5Q_setValueI(u.UK5B_getValue());
	box->show();

	return u;
}

UK5B_varVD UK5Q_form::UK5Q_recount(const QString& s, std::vector<double> def) const
{
	UK5B_varVD u;
	const auto box = map_box[s];
		
	u.UK5B_setName(s.toStdString());
	u.UK5B_setValue(false, def);
	u.UK5B_setPlace(map.key(qobject_cast<UK5Q_box*>(box->UK5Q_getPlace())));

	box->UK5Q_setVector(u.UK5B_getValue());
	box->show();

	return u;
}

UK5B_varVI UK5Q_form::UK5Q_recount(const QString& s, std::vector<int> def) const
{
	UK5B_varVI u;
	const auto box = map_box[s];
		
	u.UK5B_setName(s.toStdString());
	u.UK5B_setValue(false, def);
	u.UK5B_setPlace(map.key(qobject_cast<UK5Q_box*>(box->UK5Q_getPlace())));

	box->UK5Q_setVectorI(u.UK5B_getValue());
	box->show();

	return u;
}

std::pair<UK5B_varD, int> UK5Q_form::UK5Q_recount(const QString& s, std::pair<double, int> def) const
{
	UK5B_varD	u1;
	int			u2					= def.second;
	auto		u	= std::make_pair(u1, u2);
	const auto	box					= map_box[s];
	
	u.first.UK5B_setName(s.toStdString());
	u.first.UK5B_setValue(false, def.first);
	u.first.UK5B_setPlace(map.key(qobject_cast<UK5Q_box*>(box->UK5Q_getPlace())));
	
	box->UK5Q_setValue(u.first.UK5B_getValue());
	box->show();

	return u;
}

std::pair<UK5B_varVD, std::vector<int>> UK5Q_form::UK5Q_recount(const QString& s, std::pair<std::vector<double>, std::vector<int>> def) const
{
	UK5B_varVD			u1;
	std::vector<int>	u2						= def.second;
	auto				u	= std::make_pair(u1, u2);
	const auto	box								= map_box[s];
	
	u.first.UK5B_setName(s.toStdString());
	u.first.UK5B_setValue(false, def.first);
	u.first.UK5B_setPlace(map.key(qobject_cast<UK5Q_box*>(box->UK5Q_getPlace())));
	
	box->UK5Q_setVector(u.first.UK5B_getValue());
	box->show();

	return u;
}

void UK5Q_form::UK5Q_read(const QString& s, UK5B_varD& u, UK5Q_box* box) const
{
	u.UK5B_setName(s.toStdString());
	u.UK5B_setValue(false, box->UK5Q_getValue());
	u.UK5B_setPlace(map.key(qobject_cast<UK5Q_box*>(box->UK5Q_getPlace())));
}

void UK5Q_form::UK5Q_read(const QString& s, UK5B_varI& u, UK5Q_box* box) const
{
	u.UK5B_setName(s.toStdString());
	u.UK5B_setValue(false, box->UK5Q_getValueI());
	u.UK5B_setPlace(map.key(qobject_cast<UK5Q_box*>(box->UK5Q_getPlace())));
}

void UK5Q_form::UK5Q_read(const QString& s, UK5B_varVD& u, UK5Q_box* box) const
{
	u.UK5B_setName(s.toStdString());
	u.UK5B_setValue(false, box->UK5Q_getVector());
	u.UK5B_setPlace(map.key(qobject_cast<UK5Q_box*>(box->UK5Q_getPlace())));
}

void UK5Q_form::UK5Q_read(const QString& s, UK5B_varVI& u, UK5Q_box* box) const
{
	u.UK5B_setName(s.toStdString());
	u.UK5B_setValue(false, box->UK5Q_getVectorI());
	u.UK5B_setPlace(map.key(qobject_cast<UK5Q_box*>(box->UK5Q_getPlace())));
}

void UK5Q_form::UK5Q_newtext(QString s)
{
	UK5Q_rewrite(s.remove(0,9));
}
