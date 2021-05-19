#pragma once

#include "ui_UK5Q_box.h"
#include <vector>

class UK5Q_box final : public QWidget
{
	Q_OBJECT

public:
	explicit UK5Q_box(QWidget* parent);
	
	void				UK5Q_setLabel(const QString&) const;
	void				UK5Q_setMode(bool) const;
	int					UK5Q_getMode() const;

	bool				UK5Q_isCalculated();
	
	double				UK5Q_getValue() const;
	int					UK5Q_getValueI() const;
	std::vector<double>	UK5Q_getVector() const;
	std::vector<int>	UK5Q_getVectorI() const;

	QObject*			UK5Q_getPlace() const;
	
	void				UK5Q_setValue(double) const;
	void				UK5Q_setValueI(int) const;
	void				UK5Q_setVector(const std::vector<double>&) const;
	void				UK5Q_setVectorI(const std::vector<int>&) const;

public slots:	
	void UK5Q_state(int) const;
	void UK5Q_slot();

private:
	Ui::UK5Q_box *ui;

signals:
	void UK5Q_edit(QString);
};
