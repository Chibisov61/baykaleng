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
	map.insert(3, ui->UK5Q_verticalLayout_CHART);

	river = {};

	const std::vector<double> dnul = {0.};

	p[0] = { ui->groupBox_IN->width(),0};
	p[1] = { ui->groupBox_IN_OUT->width(),0};
	p[2] = { ui->groupBox_OUT->width(),0};
	p[3] = { ui->groupBox_CHART->width(),0 };
//in
	river.vr	=	UK5Q_init("vr" ,x,0.);																				// �������� ����
	river.br	=	UK5Q_init("br" ,x,0.);																				// ���������� �� ������� ������
	river.bl	=	UK5Q_init("bl" ,x,0.);																				// ���������� �� ������ ������
	river.nog	=	UK5Q_init("nog",x,0);																				// ���������� ���������
	river.b		=	UK5Q_init("b"	 ,x,dnul);																				// ���������� ����� ����������
	river.h		=	UK5Q_init("h"	 ,x,0.);																				// �������
	river.hog	=	UK5Q_init("hog",x,dnul);																				// ������ ���������
	river.nl	=	UK5Q_init("nl" ,x,0);																				// ���������� ������. �������
	river.l		=	UK5Q_init("l"	 ,x,dnul);																				// ���������� �� ������. �������
	river.qst	=	UK5Q_init("qst",x,0.);																				// ������ ������� ��� �� ��������
	river.cct	=	UK5Q_init("cct",x,0.);																				// �������� ������������� ��������
	river.n		=	UK5Q_init("n"	 ,x,0);																				// ������� ���������� �������� � �������
	river.psh	=	UK5Q_init("psh",x,0.);																				// ����������� �������������
//in-out � out
	river.ll	=	UK5Q_init("ll"  ,false	,500.);																	// ����� ������� ����
	river.dog	=	UK5Q_init("dog" ,x        ,UK5B_river::UK5B_eval_dog(river.qst));									// ������� ��������
	river.vst	=	UK5Q_init("vst" ,false  ,UK5B_river::UK5B_eval_vst(river.qst, river.dog));						// ������� ������� ��� �� ��������
	river.dt	=	UK5Q_init("dt"  ,false  ,UK5B_river::UK5B_eval_dt(river.vr, river.vst));							// ������������� �������			
	river.dzz	=	UK5Q_init("dzz" ,false  ,UK5B_river::UK5B_eval_dzz(river.vr, river.qst, river.dt, river.vst));	// ������� ����� �����������
	river.nn 	=	UK5Q_init("nn"  ,x		,UK5B_river::UK5B_eval_nn(river.vr,river.vst,river.dt));					// ��������� �����������
	river.xn 	=	UK5Q_init("xn"  ,x		,UK5B_river::UK5B_eval_xn(river.vr,river.vst,river.dzz));				// ���������� �� ������ �������
	river.dz	=	UK5Q_init("dz"  ,false	,UK5B_river::UK5B_eval_dydz(river.n,river.dzz));							// dz
	river.dy	=	UK5Q_init("dy"  ,false	,UK5B_river::UK5B_eval_dydz(river.n,river.dzz));							// dy
	river.pc	=	UK5Q_init("pc"  ,false	,UK5B_river::UK5B_eval_pc(river.h,river.psh));							// ����������� ����
	river.pd	=	UK5Q_init("pd"  ,x		,UK5B_river::UK5B_eval_pd(river.vr,river.h,river.pc));					// ����������� ��������
	river.dx	=	UK5Q_init("dx"  ,false	,UK5B_river::UK5B_eval_dx(river.vr,river.pd,river.dy));					// dx
	river.rbr	=	UK5Q_init("rbr" ,false	,UK5B_river::UK5B_eval_rbr(river.br,river.dy));							// ���������� �� ������� ������ (����.)
	river.rbl	=	UK5Q_init("rbl" ,false	,UK5B_river::UK5B_eval_rbl(river.bl,river.dy));							// ���������� �� ������ ������ (����.)
	river.rb	=	UK5Q_init("rb"  ,false	,UK5B_river::UK5B_eval_rb(river.b,river.nog,river.dy));					// ���������� ����� ���������� (����.)
	river.rw	=	UK5Q_init("rw"  ,false	,UK5B_river::UK5B_eval_rw(river.rbr,river.rb,river.rbl));				// ����� ������ (����.) 
	river.rhog	=	UK5Q_init("rhog",false	,UK5B_river::UK5B_eval_rhog(river.hog,river.nog,river.dy));				// ������ ��������� (����.)
	river.rh	=	UK5Q_init("rh"  ,false	,UK5B_river::UK5B_eval_rh(river.h,river.dy));							// ������� (����.)
	river.rl	=	UK5Q_init("rl"  ,false	,UK5B_river::UK5B_eval_rl(river.nl,river.l,river.dx,river.xn));			// ���������� �� ������. ������� (����.)
	river.rll	=	UK5Q_init("rll" ,false	,UK5B_river::UK5B_eval_rll(river.ll,river.dx,river.xn));					// ����� ������ ���� (����.)

	river.UK5B_init_cut();

	river.mx	=	UK5Q_init("mx"  ,x	,0.001);
	river.mm	=	UK5Q_init("mm"  ,x	,0.);

	ui->verticalLayoutWidget->setGeometry({ QPoint(0,0),p[0] });
	ui->scrollAreaWidgetContents_IN->setGeometry({ QPoint(0,0),p[0] });
	ui->UK5Q_scrollArea_IN->resize(ui->groupBox_IN->size());
	ui->UK5Q_scrollArea_IN->setWidgetResizable(false);

	ui->verticalLayoutWidget_2->setGeometry({ QPoint(0,0),p[1] });
	ui->scrollAreaWidgetContents_IN_OUT->setGeometry({ QPoint(0,0),p[1] });
	ui->UK5Q_scrollArea_IN_OUT->resize(ui->groupBox_IN_OUT->size());
	ui->UK5Q_scrollArea_IN_OUT->setWidgetResizable(false);
	
	ui->verticalLayoutWidget_3->setGeometry({ QPoint(0,0),p[2] });
	ui->scrollAreaWidgetContents_OUT->setGeometry({ QPoint(0,0),p[2] });
	ui->UK5Q_scrollArea_OUT->resize(ui->groupBox_OUT->size());
	ui->UK5Q_scrollArea_OUT->setWidgetResizable(false);

	ui->UK5Q_verticalLayout_CHART->setGeometry({ QPoint(0,0),p[3] });

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

	std::vector<double> m = { river.max };

// ����� ��������� � �������� �����	
	print.UK5B_header_print(river);
	print.UK5B_body_print(0, river);

// ������ �� ��������� � ������� ������������� � ��������� ������
	ui->UK5Q_progressBar->setValue(0);
	const int lll = river.rll.second;
	for(int i = 1; i < lll + 1; ++i)
	{
		river.cut = river.UK5B_karaush(river.cut);
		m.push_back(river.max);
		if ((std::binary_search(river.rl.second.begin(), river.rl.second.end(), i)) || (i == lll))
			print.UK5B_body_print(i,river);
		const auto ii = static_cast<double>(i) * 100. / (static_cast<double>(lll) - 1.);
		ui->UK5Q_progressBar->setValue(static_cast<int>(ii));
	}

// ����� �������, ������������� ����������� �� 500 �. � ��������� �����������
	viewCharts(ui->UK5_chart, m, 0., river.ll.UK5B_getValue(), 11, 5);
	river.mx = UK5Q_recount("mx", m.back());
	river.mm = UK5Q_recount("mm", river.cct.UK5B_getValue()/m.back());
// ��������� �������� ���� ��� ���������� �������
	river.UK5B_init_cut();
}

void UK5Q_form::viewCharts(QChartView* chw, std::vector<double> r, const double tmin, const double tmax, const int tx, const int ty) const
{
	const auto res = minmax_element(r.begin(), r.end());

	auto low = *res.first;
	auto high = *res.second;

	const auto cct = river.cct.UK5B_getValue();

	low = static_cast<int>(cct / low / 10.) * 10.;
	high = static_cast<int>((cct / high / 10.) + 1) * 10.;

	auto ch = new QtCharts::QChart;
	ch->layout()->setContentsMargins(0, 0, 0, 0);
	ch->setBackgroundRoundness(0);
	ch->setMargins(QMargins(0, 0, 0, 0));
	ch->legend()->hide();
	chw->setRenderHint(QPainter::Antialiasing);
	chw->setChart(ch);

	const auto cit = r.size();
	const auto step = (tmax - tmin) / static_cast<double>(cit - 1);

	QPen pen;
	pen.setStyle(Qt::SolidLine);
	pen.setWidth(2);
	pen.setColor(Qt::red);
	auto* series = new QLineSeries();
	for (size_t j = 1; j < cit; j++)
		series->append(step * static_cast<double>(j - 1) + tmin, cct / r[j]);
	series->setPen(pen);

	// ��� �
	auto* axisX = new QValueAxis;
	axisX->setRange(tmin, tmax);		// �������� �������� �� ��� X
	axisX->setTickCount(tx);				// ����� ����� �����
	axisX->setLabelFormat("%g");			// ������ ����������� ����� �� ��� X
	axisX->setGridLineVisible(true);

	// ��� Y
	auto* axisY = new QValueAxis;
	axisY->setRange(low, high);		// �������� �������� �� ��� Y
	axisY->setTickCount(ty);				// ����� ����� �����
	axisY->setLabelFormat("%g");			// ������ ����������� ����� �� ��� Y
	axisY->setGridLineVisible(true);

	ch->addSeries(series);
	ch->addAxis(axisX, Qt::AlignBottom);
	series->attachAxis(axisX);
	ch->addAxis(axisY, Qt::AlignLeft);
	series->attachAxis(axisY);
}

void UK5Q_form::UK5Q_rewrite(const QString& s)
{
	const auto box = map_box[s];

	switch (bmap[s])
	{
	case 1:			//vr
		UK5Q_read(s, river.vr, box);
		if (!map_box["nn"]->UK5Q_getMode()) river.nn = UK5Q_recount("nn", UK5B_river::UK5B_eval_nn(river.vr, river.qst, river.dog));		                       
		if (!map_box["xn"]->UK5Q_getMode()) river.xn = UK5Q_recount("xn", UK5B_river::UK5B_eval_xn(river.vr, river.qst, river.dog));		                        
		river.pd	= UK5Q_recount("pd" ,UK5B_river::UK5B_eval_pd(river.vr,river.h,river.pc));												                      
		river.dt	= UK5Q_recount("dt" ,UK5B_river::UK5B_eval_dt(river.vr, river.dzz));														                       
		river.dzz	= UK5Q_recount("dzz",UK5B_river::UK5B_eval_dzz(river.vr,river.qst,river.dt,river.vst));								                      			                         
		river.dz	= UK5Q_recount("dz" ,UK5B_river::UK5B_eval_dydz(river.n,river.dzz));						
		river.dy	= UK5Q_recount("dy" ,UK5B_river::UK5B_eval_dydz(river.n,river.dzz));					UK5Q_rewrite("dy");
		break;
	case 2:			//br
		UK5Q_read(s, river.br, box);
		river.rbr = UK5Q_recount("rbr",UK5B_river::UK5B_eval_rbr(river.br,river.dy));						UK5Q_rewrite("rbr");
		break;
	case 3:			//bl
		UK5Q_read(s, river.bl, box);
		river.rbl = UK5Q_recount("rbl",UK5B_river::UK5B_eval_rbl(river.bl,river.dy));						UK5Q_rewrite("rbl");
		break;
	case 4:			//nog
		UK5Q_read(s, river.nog, box);
		river.rb	= UK5Q_recount("rb",UK5B_river::UK5B_eval_rb(river.b,river.nog,river.dy));				
		river.rhog	= UK5Q_recount("rhog",UK5B_river::UK5B_eval_rhog(river.hog,river.nog,river.dy));		UK5Q_rewrite("rb");
		break;
	case 5:			//b
		UK5Q_read(s, river.b, box);
		river.rb = UK5Q_recount("rb",UK5B_river::UK5B_eval_rb(river.b,river.nog,river.dy));				UK5Q_rewrite("rb");
		break;
	case 6:			//h
		UK5Q_read(s, river.h, box);
		river.rh	= UK5Q_recount("rh",UK5B_river::UK5B_eval_rh(river.h,river.dy));						
		river.pc	= UK5Q_recount("pc",UK5B_river::UK5B_eval_pc(river.h,river.psh));															                      
		if (!map_box["pd"]->UK5Q_getMode()) river.pd = UK5Q_recount("pd", UK5B_river::UK5B_eval_pd(river.vr, river.h, river.pc));		                        
		river.dx	= UK5Q_recount("dx",UK5B_river::UK5B_eval_dx(river.vr,river.pd,river.dy));												                         
		river.rl	= UK5Q_recount("rl",UK5B_river::UK5B_eval_rl(river.nl,river.l,river.dx,river.xn));		
		river.rll	= UK5Q_recount("rll",UK5B_river::UK5B_eval_rll(river.ll,river.dx,river.xn));			river.UK5B_init_cut();
		break;
	case 7:			//hog
		UK5Q_read(s, river.hog, box);
		river.rhog = UK5Q_recount("rhog",UK5B_river::UK5B_eval_rhog(river.hog,river.nog,river.dy));		river.UK5B_init_cut();		
		break;
	case 8:			//nl
		UK5Q_read(s, river.nl, box);
		river.rl = UK5Q_recount("rl",UK5B_river::UK5B_eval_rl(river.nl,river.l,river.dx,river.xn));		UK5Q_rewrite("rl");
		break;
	case 9:			//l
		UK5Q_read(s, river.l, box);
		river.rl = UK5Q_recount("rl",UK5B_river::UK5B_eval_rl(river.nl,river.l,river.dx,river.xn));		UK5Q_rewrite("rl");	
		break;
	case 10:		//qst
		UK5Q_read(s, river.qst, box);
		if (!map_box["dog"]->UK5Q_getMode()) { river.dog = UK5Q_recount("dog", UK5B_river::UK5B_eval_dog(river.qst));						UK5Q_rewrite("dog");}
		break;
	case 11:		//cct
		UK5Q_read(s, river.cct, box);		river.UK5B_init_cut();
		break;
	case 12:		//n
		UK5Q_read(s, river.n, box);
		river.dz = UK5Q_recount("dz",UK5B_river::UK5B_eval_dydz(river.n,river.dzz));														                      
		river.dy = UK5Q_recount("dy",UK5B_river::UK5B_eval_dydz(river.n,river.dzz));														UK5Q_rewrite("dy");
		break;
	case 13:		//psh
		UK5Q_read(s, river.psh, box);
		river.pc = UK5Q_recount("pc",UK5B_river::UK5B_eval_pc(river.h,river.psh));															UK5Q_rewrite("pc");
		break;
	case 14:		//dog
		UK5Q_read(s, river.dog, box);
		if (!map_box["nn"]->UK5Q_getMode()) river.nn = UK5Q_recount("nn", UK5B_river::UK5B_eval_nn(river.vr, river.qst, river.dog));		                         
		if (!map_box["xn"]->UK5Q_getMode()) river.xn = UK5Q_recount("xn", UK5B_river::UK5B_eval_xn(river.vr, river.qst, river.dog));		                        
		river.rl	= UK5Q_recount("rl",UK5B_river::UK5B_eval_rl(river.nl,river.l,river.dx,river.xn));										
		river.rll	= UK5Q_recount("rll",UK5B_river::UK5B_eval_rll(river.ll,river.dx,river.xn));											                      
		river.vst	= UK5Q_recount("vst", UK5B_river::UK5B_eval_vst(river.qst,river.dog));													UK5Q_rewrite("vst");
		break;
	case 15:
		UK5Q_read(s, river.nn, box);		river.UK5B_init_cut();
		break;
	case 16:		//xn
		UK5Q_read(s, river.xn, box);
		river.rl	= UK5Q_recount("rl",UK5B_river::UK5B_eval_rl(river.nl,river.l,river.dx,river.xn));										
		river.rll	= UK5Q_recount("rll",UK5B_river::UK5B_eval_rll(river.ll,river.dx,river.xn));											river.UK5B_init_cut();
		break;
	case 18:		//pc
		if (!map_box["pd"]->UK5Q_getMode()) { river.pd = UK5Q_recount("pd", UK5B_river::UK5B_eval_pd(river.vr, river.h, river.pc));		UK5Q_rewrite("pd"); }
		break;
	case 19:		//pd
		UK5Q_read(s, river.pd, box);
		river.dx	= UK5Q_recount("dx",UK5B_river::UK5B_eval_dx(river.vr,river.pd,river.dy));				UK5Q_rewrite("dx");
		break;
	case 20:		//dz
	case 21:		//dy
		river.dx	= UK5Q_recount("dx",UK5B_river::UK5B_eval_dx(river.vr,river.pd,river.dy));				                          
		river.rl	= UK5Q_recount("rl",UK5B_river::UK5B_eval_rl(river.nl,river.l,river.dx,river.xn));		
		river.rll	= UK5Q_recount("rll",UK5B_river::UK5B_eval_rll(river.ll,river.dx,river.xn));			                      
		river.rbr	= UK5Q_recount("rbr",UK5B_river::UK5B_eval_rbr(river.br,river.dy));					                      
		river.rb	= UK5Q_recount("rb",UK5B_river::UK5B_eval_rb(river.b,river.nog,river.dy));				                    
		river.rbl	= UK5Q_recount("rbl",UK5B_river::UK5B_eval_rbl(river.bl,river.dy));					                        
		river.rw	= UK5Q_recount("rw",UK5B_river::UK5B_eval_rw(river.rbr,river.rb,river.rbl));				                       
		river.rhog	= UK5Q_recount("rhog",UK5B_river::UK5B_eval_rhog(river.hog,river.nog,river.dy));		                       	
		river.rh	= UK5Q_recount("rh",UK5B_river::UK5B_eval_rh(river.h,river.dy));						river.UK5B_init_cut();					
		break;
	case 22:		//dx
		river.rl	= UK5Q_recount("rl",UK5B_river::UK5B_eval_rl(river.nl,river.l,river.dx,river.xn));		
		river.rll	= UK5Q_recount("rll",UK5B_river::UK5B_eval_rll(river.ll,river.dx,river.xn));			river.UK5B_init_cut();
		break;
	case 23:		//rbr
	case 24:		//rbl
	case 25:		//rb
		river.rw = UK5Q_recount("rw",UK5B_river::UK5B_eval_rw(river.rbr,river.rb,river.rbl));				river.UK5B_init_cut();
		break;
	case 31:		//vst
		river.dt  = UK5Q_recount("dt", UK5B_river::UK5B_eval_dt(river.vr,river.vst));						UK5Q_rewrite("dt");	                         
		break;
	case 32:		//dt
		river.dzz = UK5Q_recount("dzz", UK5B_river::UK5B_eval_dzz(river.vr,river.qst,river.dt,river.vst));	UK5Q_rewrite("dzz");					                         
		break;
	case 33:		//dzz
		river.dz = UK5Q_recount("dz",UK5B_river::UK5B_eval_dydz(river.n,river.dzz));						
		river.dy = UK5Q_recount("dy",UK5B_river::UK5B_eval_dydz(river.n,river.dzz));						UK5Q_rewrite("dy");
		break;
	default: ;
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
	connect(box, SIGNAL(UK5Q_check(QString)), this, SLOT(UK5Q_chck(QString)));
	box->UK5Q_setLabel(lmap[s]);
	box->UK5Q_setValue(u.UK5B_getValue());
	box->UK5Q_setMode((place !=1 ) ? false : true);
	box->UK5Q_state((place >= 2) ? 0 : 2);
//	box->show();
	if 	(place == 1)
	{
		auto box2 = new UK5Q_box(nullptr);
		map[2]->addWidget(box2);		
		box2->setObjectName("UK5Q_BOX_"+s+"_eval");
		p[2] += {0, box->height()+2};
		map_box.insert(s+"_eval",box2);
		box2->UK5Q_setLabel(lmap[s]+QStringLiteral(u" (����.)"));
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
	connect(box, SIGNAL(UK5Q_check(QString)), this, SLOT(UK5Q_chck(QString)));
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
	connect(box, SIGNAL(UK5Q_check(QString)), this, SLOT(UK5Q_chck(QString)));
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
	connect(box, SIGNAL(UK5Q_check(QString)), this, SLOT(UK5Q_chck(QString)));
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
	connect(box, SIGNAL(UK5Q_check(QString)), this, SLOT(UK5Q_chck(QString)));
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
	connect(box, SIGNAL(UK5Q_check(QString)), this, SLOT(UK5Q_chck(QString)));
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

void UK5Q_form::UK5Q_chck(QString s)
{
	const QString ss = s.remove(0, 9);
	const auto box = map_box[ss];
	const auto mod = box->UK5Q_getMode();
	switch(bmap[s])
	{
	case 14:
		box->UK5Q_setValue((mod == Qt::Unchecked) ? river.dog.UK5B_getValue2() : river.dog.UK5B_getValue());
		break;
	case 15:
		box->UK5Q_setValue((mod == Qt::Unchecked) ? river.nn.UK5B_getValue2() : river.nn.UK5B_getValue());
		break;
	case 16:
		box->UK5Q_setValue((mod == Qt::Unchecked) ? river.xn.UK5B_getValue2() : river.xn.UK5B_getValue());
		break;
	case 19:
		box->UK5Q_setValue((mod == Qt::Unchecked) ? river.pd.UK5B_getValue2() : river.pd.UK5B_getValue());
		break;
	default: ;
	}
}
