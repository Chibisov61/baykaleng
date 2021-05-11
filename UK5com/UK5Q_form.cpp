// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "UK5Q_form.h"
#include "UK5Q_box.h"		
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
//in
	UK5B_varD  vr   =  UK5Q_Dinit("vr" ,x,0.,map,QStringLiteral(u"Скорость реки"));
	UK5B_varD  bb   =  UK5Q_Dinit("bb" ,x,0.,map,QStringLiteral(u"Расстояние до берега")); 
	UK5B_varD  h    =  UK5Q_Dinit("h"  ,x,0.,map,QStringLiteral(u"Глубина"));
	UK5B_varI  nog  =  UK5Q_Iinit("nog",x,0 ,map,QStringLiteral(u"Количество оголовков"));
	UK5B_varD  dog  =  UK5Q_Dinit("dog",x,0.,map,QStringLiteral(u"Диаметр оголовка"));
	UK5B_varD  qst  =  UK5Q_Dinit("qst",x,0.,map,QStringLiteral(u"Расход сточных вод на оголовок"));
	UK5B_varD  cct  =  UK5Q_Dinit("cct",x,0.,map,QStringLiteral(u"Величина загрязняющего вещества"));
	UK5B_varI  n    =  UK5Q_Iinit("n"  ,x,0 ,map,QStringLiteral(u"Сторона расчетного квадрата"));
	UK5B_varD  psh  =  UK5Q_Dinit("psh",x,0.,map,QStringLiteral(u"Коэффициент шероховатости"));
	UK5B_varVD l	= UK5Q_VDinit("l"	 ,x,{},map,QStringLiteral(u"Промежуточные сечения"));
	UK5B_varVD b	= UK5Q_VDinit("b"  ,x,{},map,QStringLiteral(u"Расстояния между оголовками"));
	UK5B_varVD hog	= UK5Q_VDinit("hog",x,{},map,QStringLiteral(u"Высоты оголовков"));
//in-out
	UK5B_varD  nn   =  UK5Q_Dinit("nn" ,x,0.,map,QStringLiteral(u"Начальное разбавление"));
	UK5B_varD  xn   =  UK5Q_Dinit("xn" ,x,0.,map,QStringLiteral(u"Расстояние от створа выпуска"));
//out	
	UK5B_varD  ll   =  UK5Q_Dinit("ll",false,500.,map,QStringLiteral(u"Расчетный участок реки"));

	
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

