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
	
	QMap<int, QWidget*> map;
	bool x = QFile::exists("config.ini");

	ui = new Ui::MainWindow();
	ui->setupUi(this);

	map.insert(0, ui->UK5Q_scrollAreaWidgetContents_IN);
	map.insert(1, ui->UK5Q_scrollAreaWidgetContents_IN_OUT);
	map.insert(2, ui->UK5Q_scrollAreaWidgetContents_OUT);

	auto river = new UK5B_river();
//in
	std::make_pair(river->vr,box_vr)		=  UK5Q_Dinit("vr"	,x		,0.,map,QStringLiteral(u"Скорость реки"));
	std::make_pair(river->bb,box_bb)		=  UK5Q_Dinit("bb"	,x		,0.,map,QStringLiteral(u"Расстояние до берега")); 
	std::make_pair(river->nog,box_nog)		=  UK5Q_Iinit("nog"	,x		,0 ,map,QStringLiteral(u"Количество оголовков"));
	std::make_pair(river->b,box_b)			= UK5Q_VDinit("b"		,x		,{},map,QStringLiteral(u"Расстояния между оголовками"));
	std::make_pair(river->h,box_h)			=  UK5Q_Dinit("h"		,x		,0.,map,QStringLiteral(u"Глубина"));
	std::make_pair(river->hog,box_hog)		= UK5Q_VDinit("hog"	,x		,{},map,QStringLiteral(u"Высоты оголовков"));
	std::make_pair(river->nl,box_nl)		=  UK5Q_Iinit("nl"	,x		,0 ,map,QStringLiteral(u"Количество промежуточных сечений"));
	std::make_pair(river->l,box_l)			= UK5Q_VDinit("l"		,x		,{},map,QStringLiteral(u"Промежуточные сечения"));
	std::make_pair(river->qst,box_qst)		=  UK5Q_Dinit("qst"	,x		,0.,map,QStringLiteral(u"Расход сточных вод на оголовок"));
	std::make_pair(river->cct,box_cct)		=  UK5Q_Dinit("cct"	,x		,0.,map,QStringLiteral(u"Величина загрязняющего вещества"));
	std::make_pair(river->n,box_n)			=  UK5Q_Iinit("n"		,x		,0 ,map,QStringLiteral(u"Сторона расчетного квадрата"));
	std::make_pair(river->psh,box_psh)		=  UK5Q_Dinit("psh"	,x		,0.,map,QStringLiteral(u"Коэффициент шероховатости"));
//in-out
	std::make_pair(river->dog,box_dog)		=  UK5Q_Dinit("dog"	,x		,UK5B_river::UK5B_eval_dog(river->qst)								,map,QStringLiteral(u"Диаметр оголовка"));
	std::make_pair(river->nn,box_nn)		=  UK5Q_Dinit("nn"	,x		,UK5B_river::UK5B_eval_nn(river->vr,river->qst,river->dog)			,map,QStringLiteral(u"Начальное разбавление"));
	std::make_pair(river->xn,box_xn)		=  UK5Q_Dinit("xn"	,x		,UK5B_river::UK5B_eval_xn(river->vr,river->qst,river->dog)			,map,QStringLiteral(u"Расстояние от створа выпуска"));
//out	
	std::make_pair(river->ll,box_ll)		=  UK5Q_Dinit("ll"  ,false	,500.																,map,QStringLiteral(u"Расчетный участок реки"));
	std::make_pair(river->pc,box_pc)		=  UK5Q_Dinit("pc"  ,false	,UK5B_river::UK5B_eval_pc(river->h,river->psh)						,map,QStringLiteral(u"Коэффициент Шези"));
	std::make_pair(river->pd,box_pd)		=  UK5Q_Dinit("pd"  ,false	,UK5B_river::UK5B_eval_pd(river->vr,river->h,river->pc)				,map,QStringLiteral(u"Коэффициент диффузии"));
	std::make_pair(river->dz,box_dz)		=  UK5Q_Dinit("dz"  ,false	,UK5B_river::UK5B_eval_dydz(river->vr,river->qst,river->n,river->nn)	,map,QStringLiteral(u"dz"));
	std::make_pair(river->dy,box_dy)		=  UK5Q_Dinit("dy"  ,false	,UK5B_river::UK5B_eval_dydz(river->vr,river->qst,river->n,river->nn)	,map,QStringLiteral(u"dy"));
	std::make_pair(river->dx,box_dx)		=  UK5Q_Dinit("dx"  ,false	,UK5B_river::UK5B_eval_dx(river->vr,river->pd,river->dy)				,map,QStringLiteral(u"dx"));
	std::make_pair(river->rbb,box_rbb)		=  UK5Q_Dinit("rbb" ,false	,0.,map,QStringLiteral(u"Расстояние до берега (расч.)"));
	std::make_pair(river->rb,box_rb)		= UK5Q_VDinit("rb"  ,false	,{},map,QStringLiteral(u"Расстояния между оголовками (расч.)"));
	std::make_pair(river->rw,box_rw)		=  UK5Q_Dinit("rw"  ,false	,0.,map,QStringLiteral(u"Общая ширина (расч.)"));
	std::make_pair(river->rh,box_rh)		=  UK5Q_Dinit("rh"  ,false	,0.,map,QStringLiteral(u"Глубина (расч.)"));
	std::make_pair(river->rhog,box_rhog)	= UK5Q_VDinit("rhog",false	,{},map,QStringLiteral(u"Высоты оголовков (расч.)"));
	std::make_pair(river->rll,box_rll)		=  UK5Q_Dinit("rll" ,false	,0.,map,QStringLiteral(u"Участок реки (расч.)"));
	std::make_pair(river->rl,box_rl)		= UK5Q_VDinit("rl"  ,false	,{},map,QStringLiteral(u"Помежуточные сечения (расч.)"));
	

	
	connect(ui->UK5Q_Exit, SIGNAL(clicked()), this, SLOT(UK5Q_exit()));
	connect(ui->UK5Q_Eval, SIGNAL(clicked()), this, SLOT(UK5Q_eval()));

//	connect(box_vr., SIGNAL(UK5Q_box::UK5Q_text()), this, SLOT(UK5Q_newtext()));

	auto xx = box_vr.UK5Q_getValue();
	auto yy = box_vr.UK5Q_getMode();
	
}

std::pair<UK5B_varD,UK5Q_box> UK5Q_form::UK5Q_Dinit(const QString& s, const bool x, const double def, const QMap<int, QWidget*>& map, const QString& label)
{
	UK5B_varD u;
	u.UK5B_setName(s.toStdString());
	u.UK5B_setValue(x, def);
	const int place = u.UK5B_getPlace();

	auto box = new UK5Q_box(map[place]);
//	connect(box, SIGNAL(UK5Q_box::UK5Q_text()), this, SLOT(UK5Q_newtext()));
	
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

	return std::make_pair(u, box);
}

std::pair<UK5B_varI,UK5Q_box> UK5Q_form::UK5Q_Iinit(const QString& s, const bool x, const int def, const QMap<int, QWidget*>& map,  const QString& label)
{
	UK5B_varI u;
	u.UK5B_setName(s.toStdString());
	u.UK5B_setValue(x, def);
	const int place = u.UK5B_getPlace();

	auto box = new UK5Q_box(map[place]);
//	connect(box, SIGNAL(UK5Q_box::UK5Q_text()), this, SLOT(UK5Q_newtext()));
	
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
	
	return std::make_pair(u, box);
}

std::pair<UK5B_varVD,UK5Q_box> UK5Q_form::UK5Q_VDinit(const QString& s, const bool x, const std::vector<double> def, const QMap<int, QWidget*>& map,  const QString& label)
{
	UK5B_varVD u;
	u.UK5B_setName(s.toStdString());
	u.UK5B_setValue(x, def);
	const int place = u.UK5B_getPlace();

	auto box = new UK5Q_box(map[place]);
//	connect(box, SIGNAL(UK5Q_box::UK5Q_text()), this, SLOT(UK5Q_newtext()));
	
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
	
	return std::make_pair(u, box);

}

std::pair<UK5B_varVI,UK5Q_box> UK5Q_form::UK5Q_VIinit(const QString& s, const bool x, const std::vector<int> def, const QMap<int, QWidget*>& map,  const QString& label)
{
	UK5B_varVI u;
	u.UK5B_setName(s.toStdString());
	u.UK5B_setValue(x, def);
	const int place = u.UK5B_getPlace();

	auto box = new UK5Q_box(map[place]);
//	connect(box, SIGNAL(UK5Q_box::UK5Q_text()), this, SLOT(UK5Q_newtext()));
	
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
	
	return std::make_pair(u, box);

}

void UK5Q_form::UK5Q_exit()
{
	QApplication::quit();
}

void UK5Q_form::UK5Q_eval()
{
	UK5B_river::UK5B_eval();
}

void UK5Q_form::UK5Q_newtext()
{
	const QString& ss = "";
}


