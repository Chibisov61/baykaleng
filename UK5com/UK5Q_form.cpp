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
	double pi = 3.1415;
	double  g = 9.8110;
	
	QMap<int, QWidget*> map;
	bool x = QFile::exists("config.ini");

	ui = new Ui::MainWindow();
	ui->setupUi(this);

	map.insert(0, ui->UK5Q_scrollAreaWidgetContents_IN);
	map.insert(1, ui->UK5Q_scrollAreaWidgetContents_IN_OUT);
	map.insert(2, ui->UK5Q_scrollAreaWidgetContents_OUT);

	auto river = new UK5B_river();
//in
	river->vr   =  UK5Q_Dinit("vr" ,x,0.,map,QStringLiteral(u"Скорость реки"));
	river->bb   =  UK5Q_Dinit("bb" ,x,0.,map,QStringLiteral(u"Расстояние до берега")); 
	river->nog  =  UK5Q_Iinit("nog",x,0 ,map,QStringLiteral(u"Количество оголовков"));
	river->b	= UK5Q_VDinit("b"  ,x,{},map,QStringLiteral(u"Расстояния между оголовками"));
	river->h    =  UK5Q_Dinit("h"  ,x,0.,map,QStringLiteral(u"Глубина"));
	river->hog	= UK5Q_VDinit("hog",x,{},map,QStringLiteral(u"Высоты оголовков"));
	river->nl   =  UK5Q_Iinit("nl" ,x,0 ,map,QStringLiteral(u"Количество промежуточных сечений"));
	river->l	= UK5Q_VDinit("l"	 ,x,{},map,QStringLiteral(u"Промежуточные сечения"));
	river->qst  =  UK5Q_Dinit("qst",x,0.,map,QStringLiteral(u"Расход сточных вод на оголовок"));
	river->cct  =  UK5Q_Dinit("cct",x,0.,map,QStringLiteral(u"Величина загрязняющего вещества"));
	river->n    =  UK5Q_Iinit("n"  ,x,0 ,map,QStringLiteral(u"Сторона расчетного квадрата"));
	river->psh  =  UK5Q_Dinit("psh",x,0.,map,QStringLiteral(u"Коэффициент шероховатости"));
//in-out
	river->dog  =  UK5Q_Dinit("dog",x,0.,map,QStringLiteral(u"Диаметр оголовка"));
	river->nn   =  UK5Q_Dinit("nn" ,x,0.,map,QStringLiteral(u"Начальное разбавление"));
	river->xn   =  UK5Q_Dinit("xn" ,x,0.,map,QStringLiteral(u"Расстояние от створа выпуска"));
//out	
	river->ll   =  UK5Q_Dinit("ll"  ,false,500.,map,QStringLiteral(u"Расчетный участок реки"));
	river->pc	=  UK5Q_Dinit("pc"  ,false,0.  ,map,QStringLiteral(u"Коэффициент Шези"));
	river->pd	=  UK5Q_Dinit("pd"  ,false,0.  ,map,QStringLiteral(u"Коэффициент диффузии"));
	river->dx	=  UK5Q_Dinit("dx"  ,false,0.  ,map,QStringLiteral(u"dx"));
	river->dy	=  UK5Q_Dinit("dy"  ,false,0.  ,map,QStringLiteral(u"dy"));
	river->dz	=  UK5Q_Dinit("dz"  ,false,0.  ,map,QStringLiteral(u"dz"));
	river->rbb  =  UK5Q_Dinit("rbb" ,false,0.  ,map,QStringLiteral(u"Расстояние до берега (расч.)"));
	river->rb   = UK5Q_VDinit("rb"  ,false,{}  ,map,QStringLiteral(u"Расстояния между оголовками (расч.)"));
	river->rw	=  UK5Q_Dinit("rw"  ,false,0.  ,map,QStringLiteral(u"Общая ширина (расч.)"));
	river->rh	=  UK5Q_Dinit("rh"  ,false,0.  ,map,QStringLiteral(u"Глубина (расч.)"));
	river->rhog = UK5Q_VDinit("rhog",false,{}  ,map,QStringLiteral(u"Высоты оголовков (расч.)"));
	river->rll  =  UK5Q_Dinit("rll" ,false,0.  ,map,QStringLiteral(u"Участок реки (расч.)"));
	river->rl   = UK5Q_VDinit("rl"  ,false,{}  ,map,QStringLiteral(u"Помежуточные сечения (расч.)"));
	

	
	connect(ui->UK5Q_Exit, SIGNAL(clicked()), this, SLOT(UK5Q_exit()));
	connect(ui->UK5Q_Eval, SIGNAL(clicked()), this, SLOT(UK5Q_eval()));
}

UK5B_varD UK5Q_form::UK5Q_Dinit(const QString& s, const bool x, const double def, const QMap<int, QWidget*>& map, const QString& label )
{
	UK5B_varD u;
	u.UK5B_setName(s.toStdString());
	u.UK5B_setValue(x, def);
	const int place = u.UK5B_getPlace();

	auto box = new UK5Q_box(map[place]);
	
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

UK5B_varI UK5Q_form::UK5Q_Iinit(const QString& s, const bool x, const int def, const QMap<int, QWidget*>& map,  const QString& label  )
{
	UK5B_varI u;
	u.UK5B_setName(s.toStdString());
	u.UK5B_setValue(x, def);
	const int place = u.UK5B_getPlace();

	auto box = new UK5Q_box(map[place]);
	
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

UK5B_varVD UK5Q_form::UK5Q_VDinit(const QString& s, const bool x, const std::vector<double> def, const QMap<int, QWidget*>& map,  const QString& label  )
{
	UK5B_varVD u;
	u.UK5B_setName(s.toStdString());
	u.UK5B_setValue(x, def);
	const int place = u.UK5B_getPlace();

	auto box = new UK5Q_box(map[place]);
	
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

UK5B_varVI UK5Q_form::UK5Q_VIinit(const QString& s, const bool x, const std::vector<int> def, const QMap<int, QWidget*>& map,  const QString& label  )
{
	UK5B_varVI u;
	u.UK5B_setName(s.toStdString());
	u.UK5B_setValue(x, def);
	const int place = u.UK5B_getPlace();

	auto box = new UK5Q_box(map[place]);
	
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

void UK5Q_form::UK5Q_exit()
{
	QApplication::quit();
}

void UK5Q_form::UK5Q_eval()
{
	UK5B_river::UK5B_eval();
}
