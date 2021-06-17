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

	ui = new Ui::MainWindow();
	ui->setupUi(this);

	ui->UK5Q_progressBar->setValue(0);
	
	map.insert(0, ui->UK5Q_scrollLayout_IN);
	map.insert(1, ui->UK5Q_scrollLayout_IN_OUT);
	map.insert(2, ui->UK5Q_scrollLayout_OUT);
	map.insert(3, ui->UK5Q_verticalLayout_CHART);

	p[0] = { ui->groupBox_IN->width(),0};
	p[1] = { ui->groupBox_IN_OUT->width(),0};
	p[2] = { ui->groupBox_OUT->width(),0};
	p[3] = { ui->groupBox_CHART->width(),0 };

//	river.mx	=	UK5Q_init("mx"  ,x	,0.001);
//	river.mm	=	UK5Q_init("mm"  ,x	,0.);

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

// вывод заголовка и нулевого среза	
	print.UK5B_header_print(river);
	print.UK5B_body_print(0, river);

// расчет по караушеву с выводом промежуточных и конечного срезов
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

// вывод графика, максимального загрязнения на 500 м. и конечного разбавления
	viewCharts(ui->UK5_chart, m, 0., river.ll.UK5B_getValue(), 11, 5);
	UK5Q_recount("mx",river.mx, m.back());
	UK5Q_recount("mm", river.mm,river.cct.UK5B_getValue()/m.back());
// обнуление рабочего поля для следующего расчета
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

	// ось Х
	auto* axisX = new QValueAxis;
	axisX->setRange(tmin, tmax);		// диапазон значений на оси X
	axisX->setTickCount(tx);				// число линий сетки
	axisX->setLabelFormat("%g");			// формат отображения чисел на оси X
	axisX->setGridLineVisible(true);

	// ось Y
	auto* axisY = new QValueAxis;
	axisY->setRange(low, high);		// диапазон значений на оси Y
	axisY->setTickCount(ty);				// число линий сетки
	axisY->setLabelFormat("%g");			// формат отображения чисел на оси Y
	axisY->setGridLineVisible(true);

	ch->addSeries(series);
	ch->addAxis(axisX, Qt::AlignBottom);
	series->attachAxis(axisX);
	ch->addAxis(axisY, Qt::AlignLeft);
	series->attachAxis(axisY);
}

UK5B_varD UK5Q_form::UK5Q_init(const QString& s, const bool x, const double def)
{
	UK5B_varD u;
	u.UK5B_setName(s.toStdString());
	u.UK5B_setValue(x, def);
	u.UK5B_setInit(true);
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
	}  
	return u;
}

void UK5Q_form::UK5Q_newtext(QString s)
{
	UK5Q_rewrite(s.remove(0,9));
}

void UK5Q_form::UK5Q_chck(QString s)
{
	const QString ss = s.remove(0, 9);
	const auto box = map_box[ss];
	switch(bmap[s])
	{
	case 14:
		if (river.dog.UK5B_isInit())
		{

			river.dog.UK5B_swap();
			box->UK5Q_setValue(river.dog.UK5B_getValue());
			UK5Q_rewrite("dog");
		}
		break;
	case 15:
		if (river.nn.UK5B_isInit())
		{
			river.nn.UK5B_swap();
			box->UK5Q_setValue(river.nn.UK5B_getValue());
			UK5Q_rewrite("nn");
		}
		break;
	case 16:
		if (river.xn.UK5B_isInit())
		{
			river.xn.UK5B_swap();
			box->UK5Q_setValue(river.xn.UK5B_getValue());
			UK5Q_rewrite("xn");
		}
		break;
	case 19:
		if (river.pd.UK5B_isInit())
		{
			river.pd.UK5B_swap();
			box->UK5Q_setValue(river.pd.UK5B_getValue());
			UK5Q_rewrite("pd");
		}
		break;
	default: ;
	}
}
