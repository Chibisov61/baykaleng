// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "UK5Q_form.h"
#include "UK5Q_box.h"
#include "UK5B_river.h"
#include "UK5B_out.h"
#include <QWidget>

uk5_q_form::uk5_q_form(QWidget *parent)
	: QMainWindow(parent)
{
	ui_ = new Ui::MainWindow();
	ui_->setupUi(this);

	ui_->UK5Q_progressBar->setValue(0);
	
	map.insert(0, ui_->UK5Q_scrollLayout_IN);
	map.insert(1, ui_->UK5Q_scrollLayout_IN_OUT);
	map.insert(2, ui_->UK5Q_scrollLayout_OUT);
	map.insert(3, ui_->UK5Q_verticalLayout_CHART);

	p[0] = { ui_->groupBox_IN->width(),0};
	p[1] = { ui_->groupBox_IN_OUT->width(),0};
	p[2] = { ui_->groupBox_OUT->width(),0};
	p[3] = { ui_->groupBox_CHART->width(),0 };

	for (auto& itr : r.river)
		init(itr);

	ui_->verticalLayoutWidget->setGeometry({ QPoint(0,0),p[0] });
	ui_->scrollAreaWidgetContents_IN->setGeometry({ QPoint(0,0),p[0] });
	ui_->UK5Q_scrollArea_IN->resize(ui_->groupBox_IN->size());
	ui_->UK5Q_scrollArea_IN->setWidgetResizable(false);

	ui_->verticalLayoutWidget_2->setGeometry({ QPoint(0,0),p[1] });
	ui_->scrollAreaWidgetContents_IN_OUT->setGeometry({ QPoint(0,0),p[1] });
	ui_->UK5Q_scrollArea_IN_OUT->resize(ui_->groupBox_IN_OUT->size());
	ui_->UK5Q_scrollArea_IN_OUT->setWidgetResizable(false);
	
	ui_->verticalLayoutWidget_3->setGeometry({ QPoint(0,0),p[2] });
	ui_->scrollAreaWidgetContents_OUT->setGeometry({ QPoint(0,0),p[2] });
	ui_->UK5Q_scrollArea_OUT->resize(ui_->groupBox_OUT->size());
	ui_->UK5Q_scrollArea_OUT->setWidgetResizable(false);

	ui_->UK5Q_verticalLayout_CHART->setGeometry({ QPoint(0,0),p[3] });

	connect(ui_->UK5Q_Exit, SIGNAL(clicked()), this, SLOT(exit()));
	connect(ui_->UK5Q_Eval, SIGNAL(clicked()), this, SLOT(eval()));

}

void uk5_q_form::exit()
{
	QApplication::quit();
}

void uk5_q_form::eval()
{
	const QString f = "UK5." + QDateTime::currentDateTime().toString("yyyyMMddTHHmmss") + ".csv";
	UK5B_out print(f.toStdString());

	std::vector m = { r.max_r };

// вывод заголовка и нулевого среза	
//	print.UK5B_header_print(river);
//	print.UK5B_body_print(0, river);

// расчет по караушеву с выводом промежуточных и конечного срезов
	ui_->UK5Q_progressBar->setValue(0);
	const int lll = std::stoi(r.search("ll").get_value(0));
	for(int i = 1; i < lll + 1; ++i)
	{
		r.cut = r.karaush(r.cut);
		m.push_back(r.max_r);
//		if ((std::binary_search(river.rl.second.begin(), river.rl.second.end(), i)) || (i == lll))
//			print.UK5B_body_print(i,river);
		const auto ii = static_cast<double>(i) * 100. / (static_cast<double>(lll) - 1.);
		ui_->UK5Q_progressBar->setValue(static_cast<int>(ii));
	}

// вывод графика, максимального загрязнения на 500 м. и конечного разбавления
	view_charts(ui_->UK5_chart, m, 0., std::stod(r.search("ll").get_value(1)), 11, 5);
	
	recount("mx",r.max_r, m.back());
	recount("mm", r.mm, (std::stoi(r.search("cct").get_value(1)) / m.back()));
	
// обнуление рабочего поля для следующего расчета
	r.init_cut();
}

void uk5_q_form::view_charts(QChartView* chw, std::vector<double> m_r, const double t_min, const double t_max, const int tx, const int ty)
{
	const auto [fst, snd] = minmax_element(m_r.begin(), m_r.end());

	auto low = *fst;
	auto high = *snd;

	const auto cct = std::stod(r.search("cct").get_value(1));

	low = static_cast<int>(cct / low / 10.) * 10.;
	high = static_cast<int>((cct / high / 10.) + 1) * 10.;

	auto ch = new QtCharts::QChart;
	ch->layout()->setContentsMargins(0, 0, 0, 0);
	ch->setBackgroundRoundness(0);
	ch->setMargins(QMargins(0, 0, 0, 0));
	ch->legend()->hide();
	chw->setRenderHint(QPainter::Antialiasing);
	chw->setChart(ch);

	const auto cit = m_r.size();
	const auto step = (t_max - t_min) / static_cast<double>(cit - 1);

	QPen pen;
	pen.setStyle(Qt::SolidLine);
	pen.setWidth(2);
	pen.setColor(Qt::red);
	auto* series = new QLineSeries();
	for (size_t j = 1; j < cit; j++)
		series->append(step * static_cast<double>(j - 1) + t_min, cct / m_r[j]);
	series->setPen(pen);

	// ось Х
	auto* axisX = new QValueAxis;
	axisX->setRange(t_min, t_max);		// диапазон значений на оси X
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

void uk5_q_form::init(const uk5_b_set& u)
{
	const QString	 name	= u.get_name().c_str();
	if (name == "cut") return;
	const int		place	= u.get_place();
	const int		 type	= u.get_type();
	const QString	label	= QString::fromUtf8(u.desc.c_str());

	const auto c = (type == 2) ? 1 : type;

	auto box = new uk5_q_box(nullptr);
	map[place]->addWidget(box);		
	box->setObjectName("UK5Q_BOX_"+name);
	p[place] += {0, box->height()+2};
	map_box.insert(name,box);
	connect(box, SIGNAL(edit_signal(QString)), this, SLOT(new_text_slot(QString)));
	connect(box, SIGNAL(check_signal(QString)), this, SLOT(check_slot(QString)));
	box->uk5_q_set_label(label);
	box->uk5_q_set_value(u.get_value(c).c_str());
	box->uk5_q_set_mode((place !=1 ) ? false : true);
	box->uk5_q_state((place >= 2) ? 0 : 2);
	if 	(place == 1)
	{
		auto box_second = new uk5_q_box(nullptr);
		map[2]->addWidget(box_second);		
		box_second->setObjectName("UK5Q_BOX_"+name+"_eval");
		p[2] += {0, box_second->height()+2};
		map_box.insert(name+"_eval",box_second);
		box_second->uk5_q_set_label(label+QStringLiteral(u" (расч.)"));
		box_second->uk5_q_set_value(u.get_value(c+4).c_str());
		box_second->uk5_q_set_mode(false);
		box_second->uk5_q_state(0);
	}  
	if 	(type > 1)
	{
		auto box_geometry = new uk5_q_box(nullptr);
		map[2]->addWidget(box_geometry);		
		box_geometry->setObjectName("UK5Q_BOX_"+name+"_geometry");
		p[2] += {0, box_geometry->height()+2};
		map_box.insert(name+"_geometry",box_geometry);
		box_geometry->uk5_q_set_label(label+QStringLiteral(u" (расч.)"));
		box_geometry->uk5_q_set_value(u.get_value(c-1).c_str());
		box_geometry->uk5_q_set_mode(false);
		box_geometry->uk5_q_state(0);
	}  
}

void uk5_q_form::new_text_slot(QString s)
{
	rewrite(s.remove(0,9));
}

void uk5_q_form::check_slot(QString s)
{
	const QString ss = s.remove(0, 9);
	const auto box = map_box[ss];
	switch(b_map[s])
	{
	case 14:
		if (river.dog.UK5B_isInit())
		{

			river.dog.UK5B_swap();
			box->uk5_q_set_value(river.dog.UK5B_getValue());
			UK5Q_rewrite("dog");
		}
		break;
	case 15:
		if (river.nn.UK5B_isInit())
		{
			river.nn.UK5B_swap();
			box->uk5_q_set_value(river.nn.UK5B_getValue());
			UK5Q_rewrite("nn");
		}
		break;
	case 16:
		if (river.xn.UK5B_isInit())
		{
			river.xn.UK5B_swap();
			box->uk5_q_set_value(river.xn.UK5B_getValue());
			UK5Q_rewrite("xn");
		}
		break;
	case 19:
		if (river.pd.UK5B_isInit())
		{
			river.pd.UK5B_swap();
			box->uk5_q_set_value(river.pd.UK5B_getValue());
			UK5Q_rewrite("pd");
		}
		break;
	default: ;
	}
}
