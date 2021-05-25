#pragma once

#include "ui_UK5Q_form.h"
#include "UK5B_river.h"
#include "UK5Q_box.h"
#include <vector>
#include <array>
#include <QMap>


class UK5Q_form : public QMainWindow
{
	Q_OBJECT

public:
	UK5B_river					river;
	std::array<QSize, 3>		p;
	QMap<QString, UK5Q_box*>	map_box;
	QMap<int, QVBoxLayout*>		map;
	QMap<QString,int>			bmap = {{"vr",1}, {"br",2},	{"bl",3}, {"nog",4},{"b",5},  {"h",6},  {"hog",7},{"nl",8},  {"l",9},	{"qst",10},{"cct",11},{"n",12},	  {"psh",13},{"dog",14},{"nn",15},
										{"xn",16},{"ll",17},{"pc",18},{"pd",19},{"dz",20},{"dy",21},{"dx",22},{"rbr",23},{"rbl",24},{"rb",25}, {"rw",26}, {"rhog",27},{"rh",28}, {"rl",29},	{"rll",30}
										{"vst",31},{"dt",32},{"dzz",33}};
	QMap<QString, QString>		lmap = {{"vr",QStringLiteral(u"�������� ����")},
										{"br",QStringLiteral(u"���������� �� ������� ������")},
										{"bl",QStringLiteral(u"���������� �� ������ ������")},
										{"nog",QStringLiteral(u"���������� ���������")},				
										{"b",QStringLiteral(u"���������� ����� ����������")},
										{"h",QStringLiteral(u"�������")},
										{"hog",QStringLiteral(u"������ ���������")},
										{"nl",QStringLiteral(u"���������� ������. �������")},
										{"l",QStringLiteral(u"������������� ������� (���.)")},
										{"qst",QStringLiteral(u"������ ������� ��� �� ��������")},
										{"cct",QStringLiteral(u"�������� ������������� ��������")},		
										{"n",QStringLiteral(u"������� ���������� ��������")},
										{"psh",QStringLiteral(u"����������� �������������")},
										{"dog",QStringLiteral(u"������� ��������")},
										{"vst",QStringLiteral(u"�������� ������� ��� (�� ��������)")},
										{"dt",QStringLiteral(u"������������� �������")},
										{"dzz",QStringLiteral(u"������� ����� �����������")},
										{"nn",QStringLiteral(u"��������� �����������")},
										{"xn",QStringLiteral(u"���������� �� ������ �������")},
										{"ll",QStringLiteral(u"��������� ������� ����")},
										{"pc",QStringLiteral(u"����������� ����")},
										{"pd",QStringLiteral(u"����������� ��������")},
										{"dz",QStringLiteral(u"dz")},
										{"dy",QStringLiteral(u"dy")},							
										{"dx",QStringLiteral(u"dx")},							
										{"rbr",QStringLiteral(u"���������� �� ������� ������ (����.)")},
										{"rbl",QStringLiteral(u"���������� �� ������ ������ (����.)")},
										{"rb",QStringLiteral(u"���������� ����� ���������� (����.)")},
										{"rw",QStringLiteral(u"����� ������ (����.)")},
										{"rhog",QStringLiteral(u"������ ��������� (����.)")},
										{"rh",QStringLiteral(u"������� (����.)")},
										{"rl",QStringLiteral(u"������������ ������� (����.)")},
										{"rll",QStringLiteral(u"������� ���� (����.)")}};
								explicit	UK5Q_form(QWidget* parent = Q_NULLPTR);

									void	UK5Q_rewrite(const QString&);
	
	UK5B_varD								UK5Q_init(const QString&, bool, double);
	UK5B_varI								UK5Q_init(const QString&, bool, int);
	UK5B_varVD								UK5Q_init(const QString&, bool, std::vector<double>);
	UK5B_varVI								UK5Q_init(const QString&, bool, std::vector<int>);
	std::pair<UK5B_varD,int>				UK5Q_init(const QString&, bool, std::pair<double,int>);
	std::pair<UK5B_varVD,std::vector<int>>	UK5Q_init(const QString&, bool, std::pair<std::vector<double>,std::vector<int>>);
	
	UK5B_varD								UK5Q_recount(const QString&, double) const;
	UK5B_varI								UK5Q_recount(const QString&, int) const;
	UK5B_varVD								UK5Q_recount(const QString&, std::vector<double>) const;
	UK5B_varVI								UK5Q_recount(const QString&, std::vector<int>) const;
	std::pair<UK5B_varD,int>				UK5Q_recount(const QString&, std::pair<double,int>) const;
	std::pair<UK5B_varVD,std::vector<int>>	UK5Q_recount(const QString&, std::pair<std::vector<double>,std::vector<int>>) const;
	
									void	UK5Q_read(const QString&,	UK5B_varD&,	UK5Q_box*) const;
									void	UK5Q_read(const QString&,	UK5B_varI&,	UK5Q_box*) const;
									void	UK5Q_read(const QString&,	UK5B_varVD&,UK5Q_box*) const;
									void	UK5Q_read(const QString&,	UK5B_varVI&,UK5Q_box*) const;

public slots:
	void UK5Q_newtext(QString);
private:
	Ui::MainWindow* ui;
private slots:
	static void UK5Q_exit();
	void UK5Q_eval();
signals:
	void UK5Q_edit(QString);
};

