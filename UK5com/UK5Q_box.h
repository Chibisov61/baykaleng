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
	
	double				UK5Q_getValue() const;
	std::vector<double>	UK5Q_getVector() const;
	int					UK5Q_getValueI() const;
	std::vector<int>	UK5Q_getVectorI() const;

	
	void				UK5Q_setValue(const double) const;
	void				UK5Q_setVector(const std::vector<double>&) const;
	void				UK5Q_setValueI(const int) const;
	void				UK5Q_setVectorI(const std::vector<int>&) const;

public slots:	
	void UK5Q_state(int) const;
private:
	Ui::UK5Q_box *ui;
};
