#pragma once

#include "ui_UK5Q_form.h"
#include "UK5B_river.h"
#include "UK5Q_box.h"
#include <vector>
#include <array>
#include <QMap>


class uk5_q_form final : public QMainWindow
{
	Q_OBJECT

public:
	explicit					uk5_q_form(QWidget* parent = Q_NULLPTR);
	uk5_b_river					r;
	std::array<QSize, 4>		p;
	QMap<QString, uk5_q_box*>	map_box;
	QMap<int, QVBoxLayout*>		map;
	
	void						view_charts(QChartView*, std::vector<double>, double, double, int, int);
	void						init(const uk5_b_set& );
	static std::vector<double>			dis(QString);

public slots:
	void new_text_slot(QString);
	void check_slot(QString);
private:
	Ui::MainWindow* ui_;
private slots:
	static void exit();
	void eval();
	void rewrite(QString);
signals:
	void check_signal(QString);
	void edit_signal(QString);

};


