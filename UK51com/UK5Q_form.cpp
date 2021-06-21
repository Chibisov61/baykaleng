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
	connect(ui_->UK5Q_Eval, SIGNAL(clicked()), this, SLOT(eval_cut()));

}

void uk5_q_form::view_charts(QChartView* chw, std::vector<double> m_r, const double t_min, const double t_max, const int tx, const int ty)
{
	const auto [fst, snd] = minmax_element(m_r.begin(), m_r.end());

	auto low = *fst;
	auto high = *snd;

	const auto cct = std::stod(r.river.at(r.search("cct")).get_value(1));

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
	auto* axis_x = new QValueAxis;
	axis_x->setRange(t_min, t_max);			// диапазон значений на оси X
	axis_x->setTickCount(tx);				// число линий сетки
	axis_x->setLabelFormat("%g");			// формат отображения чисел на оси X
	axis_x->setGridLineVisible(true);

	// ось Y
	auto* axis_y = new QValueAxis;
	axis_y->setRange(low, high);		// диапазон значений на оси Y
	axis_y->setTickCount(ty);				// число линий сетки
	axis_y->setLabelFormat("%g");			// формат отображения чисел на оси Y
	axis_y->setGridLineVisible(true);

	ch->addSeries(series);
	ch->addAxis(axis_x, Qt::AlignBottom);
	series->attachAxis(axis_x);
	ch->addAxis(axis_y, Qt::AlignLeft);
	series->attachAxis(axis_y);
}

void uk5_q_form::init(const uk5_b_set& u)
{
	const QString	 name	= QString::fromStdString(u.get_name());
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
	box->uk5_q_set_mode((place !=1 ) ? false : true);
	box->uk5_q_set_label(label);
	box->uk5_q_set_value(QString::fromStdString(u.get_value(c)));
	box->uk5_q_set_state((place >= 2) ? 0 : 2);
	connect(box, SIGNAL(edit_signal(QString)), this, SLOT(edit_slot(QString)));
	connect(box, SIGNAL(check_signal(QString)), this, SLOT(check_slot(QString)));
	if 	(place == 1)
	{
		auto box_second = new uk5_q_box(nullptr);
		map[2]->addWidget(box_second);		
		box_second->setObjectName("UK5Q_BOX_"+name+"_eval");
		p[2] += {0, box_second->height()+2};
		map_box.insert(name+"_eval",box_second);
		box_second->uk5_q_set_mode(false);
		box_second->uk5_q_set_label(label+QStringLiteral(u" (расч.)"));
		box_second->uk5_q_set_value(QString::fromStdString(u.get_value(c+4)));
		box_second->uk5_q_set_state(0);
	}  
	if 	(type > 1)
	{
		auto box_geometry = new uk5_q_box(nullptr);
		map[2]->addWidget(box_geometry);		
		box_geometry->setObjectName("UK5Q_BOX_"+name+"_geometry");
		p[2] += {0, box_geometry->height()+2};
		map_box.insert(name+"_geometry",box_geometry);
		box_geometry->uk5_q_set_mode(false);
		box_geometry->uk5_q_set_label(label+QStringLiteral(u" (в ячейках)"));
		box_geometry->uk5_q_set_value(QString::fromStdString(u.get_value(c-1)));
		box_geometry->uk5_q_set_state(0);
	}  
}

void uk5_q_form::read(const QString& s)
{
	const auto box			= map_box[s];
	const QString val		= box->uk5_q_get_value();
	const auto num	= r.search(s.toStdString());
	const int  place		= r.river.at(num).get_place();
	const auto type = r.river.at(num).get_type();
	const auto c		= (type == 2) ? 1 : type;
	r.river.at(num).set_value(val.toStdString(), c);
	if 	(place == 1)
		map_box[s+"_eval"]->uk5_q_set_value(QString::fromStdString(r.river.at(num).get_value(c+4)));
	if (type > 1) 
		map_box[s+"_geometry"]->uk5_q_set_value(QString::fromStdString(r.river.at(num).get_value(c-1)));
	
	rewrite(s);
}

void uk5_q_form::rewrite(const QString& s)
{
	const auto n = r.search(s.toStdString());
	int begin, end;

	if (const auto cut = r.search("cut"); n != cut)
	{
		begin = n + 1;
		end   = cut;
	}
	else
	{
		begin = cut + 1;
		end   = cut + 3;
	}

	for (int i = begin; i < end; i++)
	{
		const int		c		= r.recount(r.river.at(i));
		const QString	name	= QString::fromStdString(r.river.at(i).get_name());
		const int		place	= r.river.at(i).get_place();
		const int		type	= r.river.at(i).get_type();
		auto ss = QString::fromStdString(r.river.at(i).get_value(c));
		map_box[name]->uk5_q_set_value(ss);
		if 	(place == 1)
			map_box[name+"_eval"]->uk5_q_set_value(QString::fromStdString(r.river.at(i).get_value(c+4)));
		if (type > 1)
			map_box[name+"_geometry"]->uk5_q_set_value(QString::fromStdString(r.river.at(i).get_value(c-1)));
	}
			
}

void uk5_q_form::edit_slot(QString s)
{
	read(s.remove(0,9));
}

// ReSharper disable once CppParameterMayBeConst
void uk5_q_form::check_slot(QString s)  // NOLINT(performance-unnecessary-value-param)
{
	const QString ss = s.remove(0, 9);
	const auto box = map_box[ss];
	const auto num = r.search(ss.toStdString());

	if (!r.river.at(num).is_init()) return;
	const auto state = box->uk5_q_get_state();
	const auto с = r.re_init(r.river.at(num),state);
	const auto def = QString::fromStdString(r.river.at(num).get_value(с));
	box->uk5_q_set_value(def);

	r.recount(r.river.at(num));
	
	rewrite(ss);
}

std::vector<double> uk5_q_form::disassemble(QString str)
{

	std::vector<double> ret = {};
	bool ok;
	if (str.back() == ";") str.truncate(str.size() - 1);
	const QStringList list = str.split(";");
	for (QStringList::const_iterator itr = list.constBegin(); itr != list.constEnd(); ++itr)
	{
		ret.push_back((*itr).toDouble(&ok));
		if (!ok) return { -1. };
	}
	return ret;
}

std::vector<int> uk5_q_form::disassemble_int(QString str)
{

	std::vector<int> ret = {};
	bool ok;
	if (str.back() == ";") str.truncate(str.size() - 1);
	const QStringList list = str.split(";");
	for (QStringList::const_iterator itr = list.constBegin(); itr != list.constEnd(); ++itr)
	{
		ret.push_back((*itr).toInt(&ok));
		if (!ok) return { -1 };
	}
	return ret;
}

void uk5_q_form::exit()
{
	QApplication::quit();
}

void uk5_q_form::eval_cut()
{
  	const QString f = "UK5." + QDateTime::currentDateTime().toString("yyyyMMddTHHmmss") + ".csv";
  	uk5_b_out print(f.toStdString());
// вывод заголовка и нулевого среза	
  	print.uk5_b_header_print(r);
  	print.uk5_b_body_print(0, r);

// обнуление рабочего поля перед расчетом

	r.recount(r.river.at(r.search("cut")));
		
// расчет по караушеву с выводом промежуточных и конечного срезов
	ui_->UK5Q_progressBar->setValue(0);
	const int lll = std::stoi(r.river.at(r.search("ll")).get_value(0));
	for(int i = 1; i < lll + 1; ++i)
	{
		r.cut = r.karaushev(r.cut);
		const auto l_s = r.river.at(r.search("l")).get_value(2);
		if (const auto l_d = disassemble_int(QString::fromStdString(l_s)); (std::binary_search(l_d.begin(), l_d.end(), i)) || (i == lll))
			print.uk5_b_body_print(i,r);
		const auto ii = static_cast<double>(i) * 100. / (static_cast<double>(lll) - 1.);
		ui_->UK5Q_progressBar->setValue(static_cast<int>(ii));
	}

// вывод графика, максимального загрязнения на 500 м. и конечного разбавления
	const auto m = disassemble(QString::fromStdString(r.river.at(r.search("cut")).get_value(3)));
	view_charts(ui_->UK5_chart, m, 0., std::stod(r.river.at(r.search("ll")).get_value(1)), 11, 5);

	rewrite("cut");
}

