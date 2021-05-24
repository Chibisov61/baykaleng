// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "UK5Q_form.h"
#include "UK5Q_box.h"
#include "UK5B_river.h"
#include "UK5B_out.h"
#include <QWidget>

UK5Q_form::UK5Q_form(QWidget *parent)
	: QMainWindow(parent)
{
	const bool x = QFile::exists("config.ini");

	ui = new Ui::MainWindow();
	ui->setupUi(this);

	ui->UK5Q_progressBar->setValue(0);
	
	map.insert(0, ui->UK5Q_scrollLayout_IN);
	map.insert(1, ui->UK5Q_scrollLayout_IN_OUT);
	map.insert(2, ui->UK5Q_scrollLayout_OUT);

	river = {};

	const std::vector<double> dnul = {0.};

	p[0] = { ui->groupBox_IN->width(),0};
	p[1] = { ui->groupBox_IN_OUT->width(),0};
	p[2] = { ui->groupBox_OUT->width(),0};

//in
	river.vr	=  UK5Q_init("vr"	,x,0.);																			// Скорость реки
	river.br	=  UK5Q_init("br"	,x,0.);																			// Расстояние от правого берега
	river.bl	=  UK5Q_init("bl"	,x,0.);																			// Расстояние от левого берега
	river.nog	=  UK5Q_init("nog",x,0);																			// Количество оголовков
	river.b		=  UK5Q_init("b"	,x,dnul);																			// Расстояния между оголовками
	river.h		=  UK5Q_init("h"	,x,0.);																			// Глубина
	river.hog	=  UK5Q_init("hog",x,dnul);																			// Высоты оголовков
	river.nl	=  UK5Q_init("nl"	,x,0);																			// Количество промеж. сечений
	river.l		=  UK5Q_init("l"	,x,dnul);																			// Расстояния до промеж. сечений
	river.qst	=  UK5Q_init("qst",x,0.);																			// Расход сточных вод на оголовок
	river.cct	=  UK5Q_init("cct",x,0.);																			// Величина загрязняющего вещества
	river.n		=  UK5Q_init("n"	,x,0);																			// Сторона расчетного квадрата в ячейках
	river.psh	=  UK5Q_init("psh",x,0.);																			// Коэффициент шероховатости
//in-out и out
	river.ll	=  UK5Q_init("ll"	,x,500.);																		// Длина участка реки
	river.dog	=  UK5Q_init("dog",x			,UK5B_river::UK5B_eval_dog(river.cct));								// Диаметр оголовка
	river.nn 	=  UK5Q_init("nn"	,x			,UK5B_river::UK5B_eval_nn(river.vr,river.dog,river.qst));			// Начальное разбавление
	river.xn 	=  UK5Q_init("xn"	,x			,UK5B_river::UK5B_eval_xn(river.vr,river.dog,river.qst));			// Расстояние до створа выпуска
	river.pc	=  UK5Q_init("pc"  ,false	,UK5B_river::UK5B_eval_pc(river.h,river.psh));						// Коэффициент Шези
	river.pd	=  UK5Q_init("pd" ,x			,UK5B_river::UK5B_eval_pd(river.vr,river.h,river.pc));				// Коэффициент диффузии
	river.dz	=  UK5Q_init("dz"  ,false	,UK5B_river::UK5B_eval_dydz(river.vr,river.qst,river.n,river.nn));	// dz
	river.dy	=  UK5Q_init("dy"  ,false	,UK5B_river::UK5B_eval_dydz(river.vr,river.qst,river.n,river.nn));	// dy
	river.dx	=  UK5Q_init("dx"  ,false	,UK5B_river::UK5B_eval_dx(river.vr,river.pd,river.dy));				// dx
	river.rbr	=  UK5Q_init("rbr" ,false	,UK5B_river::UK5B_eval_rbr(river.br,river.dy));						// Расстояние от правого берега (расч.)
	river.rbl	=  UK5Q_init("rbl" ,false	,UK5B_river::UK5B_eval_rbl(river.bl,river.dy));						// Расстояние от левого берега (расч.)
	river.rb	=  UK5Q_init("rb"  ,false	,UK5B_river::UK5B_eval_rb(river.b,river.nog,river.dy));				// Расстояния между оголовками (расч.)
	river.rw	=  UK5Q_init("rw"  ,false	,UK5B_river::UK5B_eval_rw(river.rbr,river.rb,river.rbr));			// Общая ширина (расч.) 
	river.rhog	=  UK5Q_init("rhog",false	,UK5B_river::UK5B_eval_rhog(river.hog,river.nog,river.dy));			// Высоты оголовков (расч.)
	river.rh	=  UK5Q_init("rh"  ,false	,UK5B_river::UK5B_eval_rh(river.h,river.dy));						// Глубина (расч.)
	river.rl	=  UK5Q_init("rl"  ,false	,UK5B_river::UK5B_eval_rl(river.nl,river.l,river.dx,river.xn));		// Расстояния до промеж. сечений (расч.)
	river.rll	=  UK5Q_init("rll" ,false	,UK5B_river::UK5B_eval_rll(river.ll,river.dx,river.xn));				// Длина учаска реки (расч.)

	river.UK5B_init_cut();

	ui->verticalLayoutWidget->setGeometry({ QPoint(0,0),p[0] });
	ui->UK5Q_scrollArea_IN->resize(ui->groupBox_IN->size());
	ui->UK5Q_scrollArea_IN->setWidgetResizable(false);

	ui->verticalLayoutWidget_2->setGeometry({ QPoint(0,0),p[1] });
	ui->UK5Q_scrollArea_IN_OUT->resize(ui->groupBox_IN_OUT->size());
	ui->UK5Q_scrollArea_IN_OUT->setWidgetResizable(false);
	
	ui->verticalLayoutWidget_3->setGeometry({ QPoint(0,0),p[2] });
	ui->UK5Q_scrollArea_OUT->resize(ui->groupBox_OUT->size());
	ui->UK5Q_scrollArea_OUT->setWidgetResizable(false);

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
	case 2:			//br
		UK5Q_read(s, river.br, box);
		river.rbr = UK5Q_recount("rbr",UK5B_river::UK5B_eval_rbr(river.br,river.dy));					UK5Q_rewrite("rbr");
		break;
	case 3:			//bl
		UK5Q_read(s, river.bl, box);
		river.rbl = UK5Q_recount("rbl",UK5B_river::UK5B_eval_rbl(river.bl,river.dy));					UK5Q_rewrite("rbl");
		break;
	case 4:			//nog
		UK5Q_read(s, river.nog, box);
		river.rb	= UK5Q_recount("rb",UK5B_river::UK5B_eval_rb(river.b,river.nog,river.dy));			UK5Q_rewrite("rb");
		river.rhog = UK5Q_recount("rhog",UK5B_river::UK5B_eval_rhog(river.hog,river.nog,river.dy));				
		break;
	case 5:			//b
		UK5Q_read(s, river.b, box);
		river.rb = UK5Q_recount("rb",UK5B_river::UK5B_eval_rb(river.b,river.nog,river.dy));			UK5Q_rewrite("rb");
		break;
	case 6:			//h
		UK5Q_read(s, river.h, box);
		river.pc = UK5Q_recount("pc",UK5B_river::UK5B_eval_pc(river.h,river.psh));						UK5Q_rewrite("pc");
		river.rh = UK5Q_recount("rh",UK5B_river::UK5B_eval_rh(river.h,river.dy));					
		break;
	case 7:			//hog
		UK5Q_read(s, river.hog, box);
		river.rhog = UK5Q_recount("rhog",UK5B_river::UK5B_eval_rhog(river.hog,river.nog,river.dy));				
		break;
	case 8:			//nl
		UK5Q_read(s, river.nl, box);
		river.rl = UK5Q_recount("rl",UK5B_river::UK5B_eval_rl(river.nl,river.l,river.dx,river.xn));	UK5Q_rewrite("rl");
		break;
	case 9:			//l
		UK5Q_read(s, river.l, box);
		river.rl = UK5Q_recount("rl",UK5B_river::UK5B_eval_rl(river.nl,river.l,river.dx,river.xn));			
		break;
	case 10:			//qst
		UK5Q_read(s, river.qst, box);
		if (!map_box["nn"]->UK5Q_getMode())  { river.nn = UK5Q_recount("nn", UK5B_river::UK5B_eval_nn(river.vr, river.qst, river.dog));	UK5Q_rewrite("nn");  }
		if (!map_box["xn"]->UK5Q_getMode())  { river.xn = UK5Q_recount("xn", UK5B_river::UK5B_eval_xn(river.vr, river.qst, river.dog));	UK5Q_rewrite("xn");  }
		if (!map_box["dog"]->UK5Q_getMode()) { river.dog = UK5Q_recount("dog", UK5B_river::UK5B_eval_dog(river.qst));						UK5Q_rewrite("dog"); }
		break;
	case 12:		//n
		UK5Q_read(s, river.n, box);
		river.dz = UK5Q_recount("dz",UK5B_river::UK5B_eval_dydz(river.vr,river.qst,river.n,river.nn));	UK5Q_rewrite("dz");
		river.dy = UK5Q_recount("dy",UK5B_river::UK5B_eval_dydz(river.vr,river.qst,river.n,river.nn));	UK5Q_rewrite("dy");
		break;
	case 13:		//psh
		UK5Q_read(s, river.psh, box);
		river.pc = UK5Q_recount("pc",UK5B_river::UK5B_eval_pc(river.h,river.psh));						UK5Q_rewrite("pc");
		break;
	case 14:		//dog
		UK5Q_read(s, river.dog, box);
		if (!map_box["nn"]->UK5Q_getMode()) { river.nn = UK5Q_recount("nn", UK5B_river::UK5B_eval_nn(river.vr, river.qst, river.dog));		UK5Q_rewrite("nn"); }
		if (!map_box["xn"]->UK5Q_getMode()) { river.xn = UK5Q_recount("xn", UK5B_river::UK5B_eval_xn(river.vr, river.qst, river.dog));		UK5Q_rewrite("xn"); }
		break;
	case 15:		//nn
		UK5Q_read(s, river.nn, box);
		river.dz = UK5Q_recount("dz",UK5B_river::UK5B_eval_dydz(river.vr,river.qst,river.n,river.nn));	UK5Q_rewrite("dz");
		river.dy = UK5Q_recount("dy",UK5B_river::UK5B_eval_dydz(river.vr,river.qst,river.n,river.nn));	UK5Q_rewrite("dy");
		break;
	case 16:		//xn
		UK5Q_read(s, river.xn, box);
		river.rl	= UK5Q_recount("rl",UK5B_river::UK5B_eval_rl(river.nl,river.l,river.dx,river.xn));		
		river.rll	= UK5Q_recount("rll",UK5B_river::UK5B_eval_rll(river.ll,river.dx,river.xn));		
		break;
	case 18:		//pc
		river.pd	= UK5Q_recount("pd",UK5B_river::UK5B_eval_pd(river.vr,river.h,river.pc));			UK5Q_rewrite("pd");
		break;
	case 19:		//pd
		river.dx	= UK5Q_recount("dx",UK5B_river::UK5B_eval_dx(river.vr,river.pd,river.dy));			UK5Q_rewrite("dx");
		break;
	case 20:		//dz
	case 21:		//dy
		river.dx	= UK5Q_recount("dx",UK5B_river::UK5B_eval_dx(river.vr,river.pd,river.dy));			UK5Q_rewrite("dx");
		river.rbr	= UK5Q_recount("rbr",UK5B_river::UK5B_eval_rbr(river.br,river.dy));				UK5Q_rewrite("rbr");
		river.rb	= UK5Q_recount("rb",UK5B_river::UK5B_eval_rb(river.b,river.nog,river.dy));			UK5Q_rewrite("rb");
		river.rbl	= UK5Q_recount("rbl",UK5B_river::UK5B_eval_rbl(river.bl,river.dy));				UK5Q_rewrite("rbl");
		river.rhog	= UK5Q_recount("rhog",UK5B_river::UK5B_eval_rhog(river.hog,river.nog,river.dy));				
		river.rh	= UK5Q_recount("rh",UK5B_river::UK5B_eval_rh(river.h,river.dy));					
		break;
	case 22:		//dx
		river.rl	= UK5Q_recount("rl",UK5B_river::UK5B_eval_rl(river.nl,river.l,river.dx,river.xn));		
		river.rll	= UK5Q_recount("rll",UK5B_river::UK5B_eval_rll(river.ll,river.dx,river.xn));
		break;
	case 23:		//rbr
	case 24:		//rbl
	case 25:		//rb
		river.rw = UK5Q_recount("rw",UK5B_river::UK5B_eval_rw(river.rbr,river.rb,river.rbl));		
		break;
	default: ;
		break;
	}
}

UK5B_varD UK5Q_form::UK5Q_init(const QString& s, const bool x, const double def)
{
	UK5B_varD u;
	u.UK5B_setName(s.toStdString());
	u.UK5B_setValue(x, def);
	const int place = u.UK5B_getPlace();

	auto box = new UK5Q_box(nullptr);
	map[place]->addWidget(box);		
	box->setObjectName("UK5Q_BOX_"+s);
	p[place] += {0, box->height()+2};
	map_box.insert(s,box);
	connect(box, SIGNAL(UK5Q_edit(QString)), this, SLOT(UK5Q_newtext(QString)));
	box->UK5Q_setLabel(lmap[s]);
	box->UK5Q_setValue(u.UK5B_getValue());
	box->UK5Q_setMode((place !=1 ) ? false : true);
	box->UK5Q_state((place == 2) ? 0 : 2);
//	box->show();
	if 	(place == 1)
	{
		auto box2 = new UK5Q_box(nullptr);
		map[2]->addWidget(box2);		
		box2->setObjectName("UK5Q_BOX_"+s+"_eval");
		p[2] += {0, box->height()+2};
		map_box.insert(s+"_eval",box2);
		box2->UK5Q_setLabel(lmap[s]+QStringLiteral(u" (расч.)"));
		box2->UK5Q_setValue(u.UK5B_getValue2());
		box2->UK5Q_setMode(false);
		box2->UK5Q_state(0);
//		box2->show();
	}  
	return u;
}

UK5B_varI UK5Q_form::UK5Q_init(const QString& s, const bool x, const int def)
{
	UK5B_varI u;
	u.UK5B_setName(s.toStdString());
	u.UK5B_setValue(x, def);
	const int place = u.UK5B_getPlace();

	auto box = new UK5Q_box(nullptr);
	map[place]->addWidget(box);		
	box->setObjectName("UK5Q_BOX_"+s);
	p[place] += {0, box->height()+2};
	map_box.insert(s,box);
	connect(box, SIGNAL(UK5Q_edit(QString)), this, SLOT(UK5Q_newtext(QString)));
	box->UK5Q_setLabel(lmap[s]);
	box->UK5Q_setValueI(u.UK5B_getValue());
	box->UK5Q_setMode((place !=1 ) ? false : true);
	box->UK5Q_state((place == 2) ? 0 : 2);
//	box->show();
	return u;
}

UK5B_varVD UK5Q_form::UK5Q_init(const QString& s, const bool x, const std::vector<double> def)
{
	UK5B_varVD u;
	u.UK5B_setName(s.toStdString());
	u.UK5B_setValue(x, def);
	const int place = u.UK5B_getPlace();

	auto box = new UK5Q_box(nullptr);
	map[place]->addWidget(box);		
	box->setObjectName("UK5Q_BOX_"+s);
	p[place] += {0, box->height()+2};
	map_box.insert(s,box);
	connect(box, SIGNAL(UK5Q_edit(QString)), this, SLOT(UK5Q_newtext(QString)));
	box->UK5Q_setLabel(lmap[s]);
	box->UK5Q_setVector(u.UK5B_getValue());
	box->UK5Q_setMode((place !=1 ) ? false : true);
	box->UK5Q_state((place == 2) ? 0 : 2);
//	box->show();
	return u;
}

UK5B_varVI UK5Q_form::UK5Q_init(const QString& s, const bool x, const std::vector<int> def)
{
	UK5B_varVI u;
	u.UK5B_setName(s.toStdString());
	u.UK5B_setValue(x, def);
	const int place = u.UK5B_getPlace();

	auto box = new UK5Q_box(nullptr);
	map[place]->addWidget(box);		
	box->setObjectName("UK5Q_BOX_"+s);
	p[place] += {0, box->height()+2};
	map_box.insert(s,box);
	connect(box, SIGNAL(UK5Q_edit(QString)), this, SLOT(UK5Q_newtext(QString)));
	box->UK5Q_setLabel(lmap[s]);
	box->UK5Q_setVectorI(u.UK5B_getValue());
	box->UK5Q_setMode((place !=1 ) ? false : true);
	box->UK5Q_state((place == 2) ? 0 : 2);
//	box->show();
	return u;

}

std::pair<UK5B_varD, int> UK5Q_form::UK5Q_init(const QString& s, const bool x, std::pair<double, int> def)
{
	UK5B_varD u1;
	int u2 = def.second;
	auto u = std::make_pair(u1, u2);
	u.first.UK5B_setName(s.toStdString());
	u.first.UK5B_setValue(x, def.first);
	const int place = u.first.UK5B_getPlace();

	auto box = new UK5Q_box(nullptr);
	map[place]->addWidget(box);		
	box->setObjectName("UK5Q_BOX_"+s);
	p[place] += {0, box->height()+2};
	map_box.insert(s,box);
	connect(box, SIGNAL(UK5Q_edit(QString)), this, SLOT(UK5Q_newtext(QString)));
	box->UK5Q_setLabel(lmap[s]);
	box->UK5Q_setValue(u.first.UK5B_getValue());
	box->UK5Q_setMode((place !=1 ) ? false : true);
	box->UK5Q_state((place == 2) ? 0 : 2);
//	box->show();
	return u;
}

std::pair<UK5B_varVD, std::vector<int>> UK5Q_form::UK5Q_init(const QString& s, const bool x, std::pair<std::vector<double>, std::vector<int>> def)
{
	UK5B_varVD u1;
	std::vector<int> u2 = def.second;
	auto u = std::make_pair(u1, u2);
	u.first.UK5B_setName(s.toStdString());
	u.first.UK5B_setValue(x, def.first);
	const int place = u.first.UK5B_getPlace();

	auto box = new UK5Q_box(nullptr);
	map[place]->addWidget(box);		
	box->setObjectName("UK5Q_BOX_"+s);
	p[place] += {0, box->height()+2};
	map_box.insert(s,box);
	connect(box, SIGNAL(UK5Q_edit(QString)), this, SLOT(UK5Q_newtext(QString)));
	box->UK5Q_setLabel(lmap[s]);
	box->UK5Q_setVector(u.first.UK5B_getValue());
	box->UK5Q_setMode((place !=1 ) ? false : true);
	box->UK5Q_state((place == 2) ? 0 : 2);
//	box->show();
	return u;
	
}

UK5B_varD UK5Q_form::UK5Q_recount(const QString& s, const double def) const
{
	UK5B_varD u;
	const auto box = map_box[s];
		
	u.UK5B_setName(s.toStdString());
	u.UK5B_setValue(false, def);
	u.UK5B_setPlace(map.key((box->UK5Q_getPlace())));

	box->UK5Q_setValue(u.UK5B_getValue());
//	box->show();

	return u;
}

UK5B_varI UK5Q_form::UK5Q_recount(const QString& s, int def) const
{
	UK5B_varI u;
	const auto box = map_box[s];
		
	u.UK5B_setName(s.toStdString());
	u.UK5B_setValue(false, def);
	u.UK5B_setPlace(map.key((box->UK5Q_getPlace())));

	box->UK5Q_setValueI(u.UK5B_getValue());
//	box->show();

	return u;
}

UK5B_varVD UK5Q_form::UK5Q_recount(const QString& s, const std::vector<double> def) const
{
	UK5B_varVD u;
	const auto box = map_box[s];
		
	u.UK5B_setName(s.toStdString());
	u.UK5B_setValue(false, def);
	u.UK5B_setPlace(map.key((box->UK5Q_getPlace())));

	box->UK5Q_setVector(u.UK5B_getValue());
//	box->show();

	return u;
}

UK5B_varVI UK5Q_form::UK5Q_recount(const QString& s, const std::vector<int> def) const
{
	UK5B_varVI u;
	const auto box = map_box[s];
		
	u.UK5B_setName(s.toStdString());
	u.UK5B_setValue(false, def);
	u.UK5B_setPlace(map.key((box->UK5Q_getPlace())));

	box->UK5Q_setVectorI(u.UK5B_getValue());
//	box->show();

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
	u.first.UK5B_setPlace(map.key((box->UK5Q_getPlace())));
	
	box->UK5Q_setValue(u.first.UK5B_getValue());
//	box->show();

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
	u.first.UK5B_setPlace(map.key((box->UK5Q_getPlace())));
	
	box->UK5Q_setVector(u.first.UK5B_getValue());
//	box->show();

	return u;
}

void UK5Q_form::UK5Q_read(const QString& s, UK5B_varD& u, UK5Q_box* box) const
{
	u.UK5B_setName(s.toStdString());
	u.UK5B_setValue(false, box->UK5Q_getValue());
	u.UK5B_setPlace(map.key((box->UK5Q_getPlace())));
}

void UK5Q_form::UK5Q_read(const QString& s, UK5B_varI& u, UK5Q_box* box) const
{
	u.UK5B_setName(s.toStdString());
	u.UK5B_setValue(false, box->UK5Q_getValueI());
	u.UK5B_setPlace(map.key((box->UK5Q_getPlace())));
}

void UK5Q_form::UK5Q_read(const QString& s, UK5B_varVD& u, UK5Q_box* box) const
{
	u.UK5B_setName(s.toStdString());
	u.UK5B_setValue(false, box->UK5Q_getVector());
	u.UK5B_setPlace(map.key((box->UK5Q_getPlace())));
}

void UK5Q_form::UK5Q_read(const QString& s, UK5B_varVI& u, UK5Q_box* box) const
{
	u.UK5B_setName(s.toStdString());
	u.UK5B_setValue(false, box->UK5Q_getVectorI());
	u.UK5B_setPlace(map.key((box->UK5Q_getPlace())));
}

void UK5Q_form::UK5Q_newtext(QString s)
{
	UK5Q_rewrite(s.remove(0,9));
}
