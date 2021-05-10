#pragma once

//#include <QtWidgets/QMainWindow>
#include "ui_UK5Q_box.h"
#include <vector>

using namespace std;

class UK5Q_box final : public QWidget
{
    Q_OBJECT

public:
    explicit UK5Q_box(QWidget* parent);
	
	void UK5Q_setLabel(const QString&) const;
	void UK5Q_setMode(bool) const;
	double UK5Q_getValue() const;
	vector<double> UK5Q_getVector() const;
	void UK5Q_setValue(double) const;
    void UK5Q_setVector(const vector<double>&) const;

public slots:	
    void UK5Q_state(int) const;
private:
    Ui::UK5Q_box *ui;
};
