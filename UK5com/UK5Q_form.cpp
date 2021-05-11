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

	auto river = new UK5B_river();
//in
	river->vr   =  UK5Q_Dinit("vr" ,x,0.,map,QStringLiteral(u"�������� ����"));
	river->bb   =  UK5Q_Dinit("bb" ,x,0.,map,QStringLiteral(u"���������� �� ������")); 
	river->h    =  UK5Q_Dinit("h"  ,x,0.,map,QStringLiteral(u"�������"));
	river->nog  =  UK5Q_Iinit("nog",x,0 ,map,QStringLiteral(u"���������� ���������"));
	river->dog  =  UK5Q_Dinit("dog",x,0.,map,QStringLiteral(u"������� ��������"));
	river->qst  =  UK5Q_Dinit("qst",x,0.,map,QStringLiteral(u"������ ������� ��� �� ��������"));
	river->cct  =  UK5Q_Dinit("cct",x,0.,map,QStringLiteral(u"�������� ������������� ��������"));
	river->n    =  UK5Q_Iinit("n"  ,x,0 ,map,QStringLiteral(u"������� ���������� ��������"));
	river->psh  =  UK5Q_Dinit("psh",x,0.,map,QStringLiteral(u"����������� �������������"));
	river->l	= UK5Q_VDinit("l"	 ,x,{},map,QStringLiteral(u"������������� �������"));
	river->b	= UK5Q_VDinit("b"  ,x,{},map,QStringLiteral(u"���������� ����� ����������"));
	river->hog	= UK5Q_VDinit("hog",x,{},map,QStringLiteral(u"������ ���������"));
//in-out
	river->nn   =  UK5Q_Dinit("nn" ,x,0.,map,QStringLiteral(u"��������� �����������"));
	river->xn   =  UK5Q_Dinit("xn" ,x,0.,map,QStringLiteral(u"���������� �� ������ �������"));
//out	
	river->ll   =  UK5Q_Dinit("ll"  ,false,500.,map,QStringLiteral(u"��������� ������� ����"));
	river->c	=  UK5Q_Dinit("c"   ,false,0.6 ,map,QStringLiteral(u"����������� ����"));
	river->d	=  UK5Q_Dinit("d"   ,false,1.0 ,map,QStringLiteral(u"����������� ��������"));
	river->dx	=  UK5Q_Dinit("dx"  ,false,0.11,map,QStringLiteral(u"dx"));
	river->dy	=  UK5Q_Dinit("dy"  ,false,0.1 ,map,QStringLiteral(u"dy"));
	river->dz	=  UK5Q_Dinit("dz"  ,false,0.1 ,map,QStringLiteral(u"dz"));
	river->rbb  =  UK5Q_Dinit("rbb" ,false,0.1 ,map,QStringLiteral(u"���������� �� ������ (����.)"));
	river->rb   = UK5Q_VDinit("rb"  ,false,{}  ,map,QStringLiteral(u"���������� ����� ���������� (����.)"));
	river->rw	=  UK5Q_Dinit("rw"  ,false,0.1 ,map,QStringLiteral(u"����� ������ (����.)"));
	river->rh	=  UK5Q_Dinit("rh"  ,false,0.1 ,map,QStringLiteral(u"������� (����.)"));
	river->rll  =  UK5Q_Dinit("rll" ,false,0.1 ,map,QStringLiteral(u"������� ���� (����.)"));
	river->rl   = UK5Q_VDinit("rl"  ,false,{}  ,map,QStringLiteral(u"������������ ������� (����)."));
	river->rhog = UK5Q_VDinit("rhog",false,{}  ,map,QStringLiteral(u"������ ��������� (����)."));
	

	
	connect(ui->UK5Q_Exit, SIGNAL(clicked()), this, SLOT(UK5Q_exit()));
	connect(ui->UK5Q_Eval, SIGNAL(clicked()), this, SLOT(UK5B_eval()));
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

