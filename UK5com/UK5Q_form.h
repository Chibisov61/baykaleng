#pragma once

#include "ui_UK5Q_form.h"
#include "UK5B_river.h"
#include "UK5Q_box.h"
#include <vector>


class UK5Q_form : public QMainWindow
{
	Q_OBJECT

public:
	explicit UK5Q_form(QWidget* parent = Q_NULLPTR);
	UK5B_varD								UK5Q_init(const QString&, bool, double,												const QMap<int, QWidget*>&, const QString&,QMap<QString, UK5Q_box*>&) const;
	UK5B_varI								UK5Q_init(const QString&, bool, int,												const QMap<int, QWidget*>&, const QString&,QMap<QString, UK5Q_box*>&) const;
	UK5B_varVD								UK5Q_init(const QString&, bool, std::vector<double>,								const QMap<int, QWidget*>&, const QString&,QMap<QString, UK5Q_box*>&) const;
	UK5B_varVI								UK5Q_init(const QString&, bool, std::vector<int>,									const QMap<int, QWidget*>&, const QString&,QMap<QString, UK5Q_box*>&) const;
	std::pair<UK5B_varD,int>				UK5Q_init(const QString&, bool, std::pair<double,int>,								const QMap<int, QWidget*>&, const QString&,QMap<QString, UK5Q_box*>&) const;
	std::pair<UK5B_varVD,std::vector<int>>	UK5Q_init(const QString&, bool, std::pair<std::vector<double>,std::vector<int>>,	const QMap<int, QWidget*>&, const QString&,QMap<QString, UK5Q_box*>&) const;
	
	QMap<QString, UK5Q_box*>map_box;

public slots:
	static void UK5Q_newtext(QString);
private:
	Ui::MainWindow* ui;
private slots:
	static void UK5Q_exit();
	static void UK5Q_eval();
signals:
	void UK5Q_edit(QString);
};