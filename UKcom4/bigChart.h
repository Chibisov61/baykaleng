#pragma once

#include "ui_bigChart.h"

//namespace Ui { class bigChart; };

class bigChart final : public QWidget
{
	Q_OBJECT

public:

	Ui::bigChart* ui;

	explicit bigChart(QWidget *parent = Q_NULLPTR)	: QWidget(parent)
	{
		ui = new Ui::bigChart();
		ui->setupUi(this);

		connect(ui->close, SIGNAL(clicked()), this, SLOT(close()));

	}

	~bigChart()
	{
		delete ui;
	}

	bigChart(const bigChart& other) : ui(other.ui)	{}

	bigChart(bigChart&& other) noexcept : ui(other.ui) 	{}

	bigChart& operator=(const bigChart& other)
	{
		if (this == &other)
			return *this;
		ui = other.ui;
		return *this;
	}

	bigChart& operator=(bigChart&& other) noexcept
	{
		if (this == &other)
			return *this;
		ui = other.ui;
		return *this;
	}
};

