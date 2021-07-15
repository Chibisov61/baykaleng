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
	bool						xls_check = false;
	
	void						view_charts(QChartView*, const std::vector<double>&, double, double, int, int);
	void						init(const uk5_b_set& );
	void						read(const QString&);
	void						rewrite(const QString&);

public slots:
	void edit_slot(QString);
	void check_slot(QString);
private:
	Ui::MainWindow* ui_;
	static std::vector<double>	disassemble(QString);
	static std::vector<int>	disassemble_int(QString);
	QStringList xls_list_ = { "Без вывода","С выводом в xlsx"};
private slots:
	static void exit();
	void eval_cut();
	void xls_out_check(int);
signals:
	void edit_signal(QString);
	void check_signal(QString);

};


