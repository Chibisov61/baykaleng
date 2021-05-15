// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "UK5Q_form.h"
#include "UK5Q_box.h"
#include "UK5B_river.h"
#include <QFile>
#include <QMap>

UK5Q_form::UK5Q_form(QWidget *parent)
	: QMainWindow(parent)
{
	
	bool x = QFile::exists("config.ini");

	ui = new Ui::MainWindow();
	ui->setupUi(this);

	map.insert(0, ui->UK5Q_scrollAreaWidgetContents_IN);
	map.insert(1, ui->UK5Q_scrollAreaWidgetContents_IN_OUT);
	map.insert(2, ui->UK5Q_scrollAreaWidgetContents_OUT);

	const std::vector<double> dnul = {0.};
//in
	river->vr	=  UK5Q_init("vr"	,x			,0.																	,map,QStringLiteral(u"Скорость реки"),map_box);
	river->bb	=  UK5Q_init("bb"	,x			,0.																	,map,QStringLiteral(u"Расстояние до берега"),map_box); 
	river->nog	=  UK5Q_init("nog",x			,0																	,map,QStringLiteral(u"Количество оголовков"),map_box);
	river->b	=  UK5Q_init("b"  ,x			,dnul																	,map,QStringLiteral(u"Расстояния между оголовками"), map_box);
	river->h	=  UK5Q_init("h"	,x			,0.																	,map,QStringLiteral(u"Глубина"),map_box);
	river->hog	=  UK5Q_init("hog",x			,dnul																	,map,QStringLiteral(u"Высоты оголовков"),map_box);
	river->nl	=  UK5Q_init("nl"	,x			,0																	,map,QStringLiteral(u"Количество промежуточных сечений"),map_box);
	river->l	=  UK5Q_init("l"	,x			,dnul																	,map,QStringLiteral(u"Промежуточные сечения"),map_box);
	river->qst	=  UK5Q_init("qst",x			,0.																	,map,QStringLiteral(u"Расход сточных вод на оголовок"),map_box);
	river->cct	=  UK5Q_init("cct",x			,0.																	,map,QStringLiteral(u"Величина загрязняющего вещества"),map_box);
	river->n	=  UK5Q_init("n"	,x			,0																	,map,QStringLiteral(u"Сторона расчетного квадрата"),map_box);
	river->psh	=  UK5Q_init("psh",x			,0.																	,map,QStringLiteral(u"Коэффициент шероховатости"),map_box);
//in-out
	river->dog	=  UK5Q_init("dog",x			,UK5B_river::UK5B_eval_dog(river->qst)								,map,QStringLiteral(u"Диаметр оголовка"),map_box);
	river->nn 	=  UK5Q_init("nn"	,x			,UK5B_river::UK5B_eval_nn(river->vr,river->qst,river->dog)			,map,QStringLiteral(u"Начальное разбавление"),map_box);
	river->xn 	=  UK5Q_init("xn"	,x			,UK5B_river::UK5B_eval_xn(river->vr,river->qst,river->dog)			,map,QStringLiteral(u"Расстояние от створа выпуска"),map_box);
//out	
	river->ll	=  UK5Q_init("ll"  ,false	,500.																,map,QStringLiteral(u"Расчетный участок реки"),map_box);
	river->pc	=  UK5Q_init("pc"  ,false	,UK5B_river::UK5B_eval_pc(river->h,river->psh)						,map,QStringLiteral(u"Коэффициент Шези"),map_box);
	river->pd	=  UK5Q_init("pd"  ,false	,UK5B_river::UK5B_eval_pd(river->vr,river->h,river->pc)				,map,QStringLiteral(u"Коэффициент диффузии"),map_box);
	river->dz	=  UK5Q_init("dz"  ,false	,UK5B_river::UK5B_eval_dydz(river->vr,river->qst,river->n,river->nn)	,map,QStringLiteral(u"dz"),map_box);
	river->dy	=  UK5Q_init("dy"  ,false	,UK5B_river::UK5B_eval_dydz(river->vr,river->qst,river->n,river->nn)	,map,QStringLiteral(u"dy"),map_box);
	river->dx	=  UK5Q_init("dx"  ,false	,UK5B_river::UK5B_eval_dx(river->vr,river->pd,river->dy)				,map,QStringLiteral(u"dx"),map_box);
	river->rbb  =  UK5Q_init("rbb" ,false	,UK5B_river::UK5B_eval_rbb(river->bb,river->dy)						,map,QStringLiteral(u"Расстояние до берега (расч.)"), map_box);
	river->rb	=  UK5Q_init("rb"  ,false	,UK5B_river::UK5B_eval_rb(river->b,river->nog,river->dy)				,map,QStringLiteral(u"Расстояния между оголовками (расч.)"),map_box);
	river->rw	=  UK5Q_init("rw"  ,false	,UK5B_river::UK5B_eval_rw(river->rbb,river->rb,river->nog)			,map,QStringLiteral(u"Общая ширина (расч.)"),map_box);
	river->rhog	=  UK5Q_init("rhog",false	,UK5B_river::UK5B_eval_rhog(river->hog,river->dy)					,map,QStringLiteral(u"Высоты оголовков (расч.)"),map_box);
	river->rh	=  UK5Q_init("rh"  ,false	,UK5B_river::UK5B_eval_rh(river->h,river->dy)						,map,QStringLiteral(u"Глубина (расч.)"),map_box);
	river->rl	=  UK5Q_init("rl"  ,false	,UK5B_river::UK5B_eval_rl(river->l,river->dx,river->xn)				,map,QStringLiteral(u"Помежуточные сечения (расч.)"),map_box);
	river->rll	=  UK5Q_init("rll" ,false	,UK5B_river::UK5B_eval_rll(river->ll,river->dx,river->xn)			,map,QStringLiteral(u"Участок реки (расч.)"),map_box);
		
	connect(ui->UK5Q_Exit, SIGNAL(clicked()), this, SLOT(UK5Q_exit()));
	connect(ui->UK5Q_Eval, SIGNAL(clicked()), this, SLOT(UK5Q_eval()));

}

UK5B_varD UK5Q_form::UK5Q_init(const QString& s, const bool x, const double def, const QMap<int, QWidget*>& mp, const QString& label,QMap<QString, UK5Q_box*>& mapbox) const
{
	UK5B_varD u;
	u.UK5B_setName(s.toStdString());
	u.UK5B_setValue(x, def);
	const int place = u.UK5B_getPlace();

	auto box = new UK5Q_box(mp[place]);
	box->setObjectName("UK5Q_BOX_"+s);
	mapbox.insert(s, box);
	connect(box, SIGNAL(UK5Q_edit(QString)), this, SLOT(UK5Q_newtext(QString)));
	
	box->UK5Q_setLabel(label);
	box->UK5Q_setValue(u.UK5B_getValue());

	switch(place)
	{
		case 0:
			box->UK5Q_setMode(false);                   
			box->UK5Q_state(2);                   
			break;
		case 1:
			box->UK5Q_setMode(true);                   
			box->UK5Q_state(0);                   
			break;
		case 2:
			box->UK5Q_setMode(false);                   
			box->UK5Q_state(0);                   
			break;
		default: ;
	}
	box->show();

	return u;
}

UK5B_varI UK5Q_form::UK5Q_init(const QString& s, const bool x, const int def, const QMap<int, QWidget*>& mp,  const QString& label,QMap<QString, UK5Q_box*>& mapbox) const
{
	UK5B_varI u;
	u.UK5B_setName(s.toStdString());
	u.UK5B_setValue(x, def);
	const int place = u.UK5B_getPlace();

	auto box = new UK5Q_box(mp[place]);
	box->setObjectName("UK5Q_BOX_"+s);
	mapbox.insert(s, box);
	connect(box, SIGNAL(UK5Q_edit(QString)), this, SLOT(UK5Q_newtext(QString)));
	
	box->UK5Q_setLabel(label);
	box->UK5Q_setValueI(u.UK5B_getValue());
	
	switch(place)
	{
		case 0:
			box->UK5Q_setMode(false);                   
			box->UK5Q_state(2);                   
			break;
		case 1:
			box->UK5Q_setMode(true);                   
			box->UK5Q_state(0);                   
			break;
		case 2:
			box->UK5Q_setMode(false);                   
			box->UK5Q_state(0);                   
			break;
		default: ;
	}
	box->show();
	
	return u;
}

UK5B_varVD UK5Q_form::UK5Q_init(const QString& s, const bool x, std::vector<double> def, const QMap<int, QWidget*>& mp,  const QString& label,QMap<QString, UK5Q_box*>& mapbox) const
{
	UK5B_varVD u;
	u.UK5B_setName(s.toStdString());
	u.UK5B_setValue(x, def);
	const int place = u.UK5B_getPlace();

	auto box = new UK5Q_box(mp[place]);
	box->setObjectName("UK5Q_BOX_"+s);
	mapbox.insert(s, box);
	connect(box, SIGNAL(UK5Q_edit(QString)), this, SLOT(UK5Q_newtext(QString)));
	
	box->UK5Q_setLabel(label);
	box->UK5Q_setVector(u.UK5B_getValue());
	
	switch(place)
	{
		case 0:
			box->UK5Q_setMode(false);                   
			box->UK5Q_state(2);                   
			break;
		case 1:
			box->UK5Q_setMode(true);                   
			box->UK5Q_state(0);                   
			break;
		case 2:
			box->UK5Q_setMode(false);                   
			box->UK5Q_state(0);                   
			break;
		default: ;
	}
	box->show();
	
	return u;

}

UK5B_varVI UK5Q_form::UK5Q_init(const QString& s, const bool x, std::vector<int> def, const QMap<int, QWidget*>& mp,  const QString& label,QMap<QString, UK5Q_box*>& mapbox) const
{
	UK5B_varVI u;
	u.UK5B_setName(s.toStdString());
	u.UK5B_setValue(x, def);
	const int place = u.UK5B_getPlace();

	auto box = new UK5Q_box(mp[place]);
	box->setObjectName("UK5Q_BOX_"+s);
	mapbox.insert(s, box);
	connect(box, SIGNAL(UK5Q_edit(QString)), this, SLOT(UK5Q_newtext(QString)));
	
	box->UK5Q_setLabel(label);
	box->UK5Q_setVectorI(u.UK5B_getValue());
	
	switch(place)
	{
		case 0:
			box->UK5Q_setMode(false);                   
			box->UK5Q_state(2);                   
			break;
		case 1:
			box->UK5Q_setMode(true);                   
			box->UK5Q_state(0);                   
			break;
		case 2:
			box->UK5Q_setMode(false);                   
			box->UK5Q_state(0);                   
			break;
		default: ;
	}
	box->show();
	
	return u;

}

std::pair<UK5B_varD, int> UK5Q_form::UK5Q_init(const QString& s, const bool x, std::pair<double, int> def, const QMap<int, QWidget*>& mp, const QString& label, QMap<QString, UK5Q_box*>& mapbox) const
{
	UK5B_varD u1;
	int u2 = def.second;
	auto u = std::make_pair(u1, u2);
	u.first.UK5B_setName(s.toStdString());
	u.first.UK5B_setValue(x, def.first);
	const int place = u.first.UK5B_getPlace();

	auto box = new UK5Q_box(mp[place]);
	box->setObjectName("UK5Q_BOX_"+s);
	mapbox.insert(s, box);
	connect(box, SIGNAL(UK5Q_edit(QString)), this, SLOT(UK5Q_newtext(QString)));
	
	box->UK5Q_setLabel(label);
	box->UK5Q_setValue(u.first.UK5B_getValue());

	switch(place)
	{
		case 0:
			box->UK5Q_setMode(false);                   
			box->UK5Q_state(2);                   
			break;
		case 1:
			box->UK5Q_setMode(true);                   
			box->UK5Q_state(0);                   
			break;
		case 2:
			box->UK5Q_setMode(false);                   
			box->UK5Q_state(0);                   
			break;
		default: ;
	}
	box->show();

	return u;
}

std::pair<UK5B_varVD, std::vector<int>> UK5Q_form::UK5Q_init(const QString& s, const bool x, std::pair<std::vector<double>, std::vector<int>> def, const QMap<int, QWidget*>& mp, const QString& label, QMap<QString, UK5Q_box*>& mapbox) const
{
	UK5B_varVD u1;
	std::vector<int> u2 = def.second;
	auto u = std::make_pair(u1, u2);
	u.first.UK5B_setName(s.toStdString());
	u.first.UK5B_setValue(x, def.first);
	const int place = u.first.UK5B_getPlace();

	auto box = new UK5Q_box(mp[place]);
	box->setObjectName("UK5Q_BOX_"+s);
	mapbox.insert(s, box);
	connect(box, SIGNAL(UK5Q_edit(QString)), this, SLOT(UK5Q_newtext(QString)));
	
	box->UK5Q_setLabel(label);
	box->UK5Q_setVector(u.first.UK5B_getValue());
	
	switch(place)
	{
		case 0:
			box->UK5Q_setMode(false);                   
			box->UK5Q_state(2);                   
			break;
		case 1:
			box->UK5Q_setMode(true);                   
			box->UK5Q_state(0);                   
			break;
		case 2:
			box->UK5Q_setMode(false);                   
			box->UK5Q_state(0);                   
			break;
		default: ;
	}
	box->show();
	
	return u;
	
}

void UK5Q_form::UK5Q_exit()
{
	QApplication::quit();
}

void UK5Q_form::UK5Q_eval()
{
	UK5B_river::UK5B_eval();
}

void UK5Q_form::UK5Q_newtext(QString s)
{
	UK5Q_rewrite(s.remove(0,9));
}

void UK5Q_form::UK5Q_rewrite(const QString& s)
{
	const auto box = map_box[s];

	switch(bmap[s])
	{
	case 1:			//vr
		UK5Q_read(s, river->vr, box);
		river->nn = UK5Q_recount(s,UK5B_river::UK5B_eval_nn(river->vr,river->qst,river->dog),	map_box);		UK5Q_rewrite("nn");
		river->xn = UK5Q_recount(s,UK5B_river::UK5B_eval_xn(river->vr,river->qst,river->dog),	map_box);		UK5Q_rewrite("xn");
		river->pd = UK5Q_recount(s,UK5B_river::UK5B_eval_pd(river->vr,river->h,river->pc),		map_box);		UK5Q_rewrite("pd");
		break;
	case 2:			//bb
		UK5Q_read(s, river->bb, box);
		river->rbb = UK5Q_recount(s,UK5B_river::UK5B_eval_rbb(river->bb,river->dy), map_box);					UK5Q_rewrite("rbb");
		break;
	case 4:			//b
		UK5Q_read(s, river->b, box);
		river->rb = UK5Q_recount(s,UK5B_river::UK5B_eval_rb(river->b,river->nog,river->dy), map_box);			UK5Q_rewrite("rb");
		break;
	case 5:			//h
		UK5Q_read(s, river->h, box);
		river->pc = UK5Q_recount(s,UK5B_river::UK5B_eval_pc(river->h,river->psh), map_box);					UK5Q_rewrite("pc");
		river->rh = UK5Q_recount(s,UK5B_river::UK5B_eval_rh(river->h,river->dy), map_box);					
		break;
	case 6:			//hog
		UK5Q_read(s, river->hog, box);
		river->rhog = UK5Q_recount(s,UK5B_river::UK5B_eval_rhog(river->hog,river->dy), map_box);				
		break;
	case 8:			//l
		UK5Q_read(s, river->l, box);
		river->rl = UK5Q_recount(s,UK5B_river::UK5B_eval_rl(river->l,river->dx,river->xn), map_box);			
		break;
	case 9:			//qst
		UK5Q_read(s, river->qst, box);
		river->nn = UK5Q_recount(s,UK5B_river::UK5B_eval_nn(river->vr,river->qst,river->dog),map_box);		UK5Q_rewrite("nn");
		river->xn = UK5Q_recount(s,UK5B_river::UK5B_eval_xn(river->vr,river->qst,river->dog),map_box);		UK5Q_rewrite("xn");
		break;
	case 11:		//n
		UK5Q_read(s, river->n, box);
		river->dz = UK5Q_recount(s,UK5B_river::UK5B_eval_dydz(river->vr,river->qst,river->n,river->nn),map_box);	UK5Q_rewrite("dz");
		river->dy = UK5Q_recount(s,UK5B_river::UK5B_eval_dydz(river->vr,river->qst,river->n,river->nn),map_box);	UK5Q_rewrite("dy");
		break;
	case 12:		//psh
		UK5Q_read(s, river->psh, box);
		river->pc = UK5Q_recount(s,UK5B_river::UK5B_eval_pc(river->h,river->psh), map_box);					UK5Q_rewrite("pc");
		break;
	case 13:		//dog
		UK5Q_read(s, river->dog, box);
		river->nn = UK5Q_recount(s,UK5B_river::UK5B_eval_nn(river->vr,river->qst,river->dog),	map_box);		UK5Q_rewrite("nn");
		river->xn = UK5Q_recount(s,UK5B_river::UK5B_eval_xn(river->vr,river->qst,river->dog),	map_box);		UK5Q_rewrite("xn");
		break;
	case 14:		//nn
		UK5Q_read(s, river->nn, box);
		river->dz = UK5Q_recount(s,UK5B_river::UK5B_eval_dydz(river->vr,river->qst,river->n,river->nn),map_box);	UK5Q_rewrite("dz");
		river->dy = UK5Q_recount(s,UK5B_river::UK5B_eval_dydz(river->vr,river->qst,river->n,river->nn),map_box);	UK5Q_rewrite("dy");
		break;
	case 15:		//xn
		UK5Q_read(s, river->xn, box);
		river->rl	= UK5Q_recount(s,UK5B_river::UK5B_eval_rl(river->l,river->dx,river->xn), map_box);		
		river->rll	= UK5Q_recount(s,UK5B_river::UK5B_eval_rll(river->ll,river->dx,river->xn), map_box);		
		break;
	case 17:		//pc
		river->pd = UK5Q_recount(s,UK5B_river::UK5B_eval_pd(river->vr,river->h,river->pc),map_box);			UK5Q_rewrite("pd");
		break;
	case 18:		//pd
		river->dx = UK5Q_recount(s,UK5B_river::UK5B_eval_dx(river->vr,river->pd,river->dy),map_box);			UK5Q_rewrite("dx");
		break;
	case 19:		//dz
	case 20:		//dy
		river->dx	= UK5Q_recount(s,UK5B_river::UK5B_eval_dx(river->vr,river->pd,river->dy),map_box);		UK5Q_rewrite("dx");
		river->rbb	= UK5Q_recount(s,UK5B_river::UK5B_eval_rbb(river->bb,river->dy), map_box);				UK5Q_rewrite("rbb");
		river->rb	= UK5Q_recount(s,UK5B_river::UK5B_eval_rb(river->b,river->nog,river->dy), map_box);		UK5Q_rewrite("rb");
		river->rhog = UK5Q_recount(s,UK5B_river::UK5B_eval_rhog(river->hog,river->dy), map_box);				
		river->rh	= UK5Q_recount(s,UK5B_river::UK5B_eval_rh(river->h,river->dy), map_box);					
		break;
	case 21:		//dx
		river->rl	= UK5Q_recount(s,UK5B_river::UK5B_eval_rl(river->l,river->dx,river->xn), map_box);		
		river->rll	= UK5Q_recount(s,UK5B_river::UK5B_eval_rll(river->ll,river->dx,river->xn), map_box);		UK5Q_rewrite("rll");
		break;
	case 22:		//rbb
	case 23:		//rb
		river->rw = UK5Q_recount(s,UK5B_river::UK5B_eval_rw(river->rbb,river->rb,river->nog), map_box);		
		break;
	default: ;
	}
}

UK5B_varD UK5Q_form::UK5Q_recount(const QString& s, double def, QMap<QString, UK5Q_box*>& mapbox) const
{
	UK5B_varD u;
	const auto box = mapbox[s];
		
	u.UK5B_setName(s.toStdString());
	u.UK5B_setValue(false, def);
	u.UK5B_setPlace(map.key(qobject_cast<UK5Q_box*>(box->UK5Q_getPlace())));

	box->UK5Q_setValue(u.UK5B_getValue());
	box->show();

	return u;
}

UK5B_varI UK5Q_form::UK5Q_recount(const QString& s, int def, QMap<QString, UK5Q_box*>& mapbox) const
{
	UK5B_varI u;
	const auto box = mapbox[s];
		
	u.UK5B_setName(s.toStdString());
	u.UK5B_setValue(false, def);
	u.UK5B_setPlace(map.key(qobject_cast<UK5Q_box*>(box->UK5Q_getPlace())));

	box->UK5Q_setValueI(u.UK5B_getValue());
	box->show();

	return u;
}

UK5B_varVD UK5Q_form::UK5Q_recount(const QString& s, std::vector<double> def, QMap<QString, UK5Q_box*>& mapbox) const
{
	UK5B_varVD u;
	const auto box = mapbox[s];
		
	u.UK5B_setName(s.toStdString());
	u.UK5B_setValue(false, def);
	u.UK5B_setPlace(map.key(qobject_cast<UK5Q_box*>(box->UK5Q_getPlace())));

	box->UK5Q_setVector(u.UK5B_getValue());
	box->show();

	return u;
}

UK5B_varVI UK5Q_form::UK5Q_recount(const QString& s, std::vector<int> def, QMap<QString, UK5Q_box*>& mapbox) const
{
	UK5B_varVI u;
	const auto box = mapbox[s];
		
	u.UK5B_setName(s.toStdString());
	u.UK5B_setValue(false, def);
	u.UK5B_setPlace(map.key(qobject_cast<UK5Q_box*>(box->UK5Q_getPlace())));

	box->UK5Q_setVectorI(u.UK5B_getValue());
	box->show();

	return u;
}

std::pair<UK5B_varD, int> UK5Q_form::UK5Q_recount(const QString& s, std::pair<double, int> def, QMap<QString, UK5Q_box*>& mapbox) const
{
	UK5B_varD	u1;
	int			u2					= def.second;
	auto		u	= std::make_pair(u1, u2);
	const auto	box					= mapbox[s];
	
	u.first.UK5B_setName(s.toStdString());
	u.first.UK5B_setValue(false, def.first);
	u.first.UK5B_setPlace(map.key(qobject_cast<UK5Q_box*>(box->UK5Q_getPlace())));
	
	box->UK5Q_setValue(u.first.UK5B_getValue());
	box->show();

	return u;
}

std::pair<UK5B_varVD, std::vector<int>> UK5Q_form::UK5Q_recount(const QString& s, std::pair<std::vector<double>, std::vector<int>> def, QMap<QString, UK5Q_box*>& mapbox) const
{
	UK5B_varVD			u1;
	std::vector<int>	u2						= def.second;
	auto				u	= std::make_pair(u1, u2);
	const auto	box								= mapbox[s];
	
	u.first.UK5B_setName(s.toStdString());
	u.first.UK5B_setValue(false, def.first);
	u.first.UK5B_setPlace(map.key(qobject_cast<UK5Q_box*>(box->UK5Q_getPlace())));
	
	box->UK5Q_setVector(u.first.UK5B_getValue());
	box->show();

	return u;
}

void UK5Q_form::UK5Q_read(const QString& s, UK5B_varD u, UK5Q_box* box) const
{
	u.UK5B_setName(s.toStdString());
	u.UK5B_setValue(false, box->UK5Q_getValue());
	u.UK5B_setPlace(map.key(qobject_cast<UK5Q_box*>(box->UK5Q_getPlace())));
}

void UK5Q_form::UK5Q_read(const QString& s, UK5B_varI u, UK5Q_box* box) const
{
	u.UK5B_setName(s.toStdString());
	u.UK5B_setValue(false, box->UK5Q_getValueI());
	u.UK5B_setPlace(map.key(qobject_cast<UK5Q_box*>(box->UK5Q_getPlace())));
}

void UK5Q_form::UK5Q_read(const QString& s, UK5B_varVD u, UK5Q_box* box) const
{
	u.UK5B_setName(s.toStdString());
	u.UK5B_setValue(false, box->UK5Q_getVector());
	u.UK5B_setPlace(map.key(qobject_cast<UK5Q_box*>(box->UK5Q_getPlace())));
}

void UK5Q_form::UK5Q_read(const QString& s, UK5B_varVI u, UK5Q_box* box) const
{
	u.UK5B_setName(s.toStdString());
	u.UK5B_setValue(false, box->UK5Q_getVectorI());
	u.UK5B_setPlace(map.key(qobject_cast<UK5Q_box*>(box->UK5Q_getPlace())));
}
