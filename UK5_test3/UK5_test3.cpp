// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "UK5_test3.h"
#include "UK5Q_box.h"


UK5_test3::UK5_test3(QWidget *parent)
	: QMainWindow(parent)
{
	ui = new Ui::UK5_test3Class();
	ui->setupUi(this);

	QMap<int,UK5Q_box*> map;
	
	for(int i=0;  i < 20; ++i)
	{
		const auto t = new UK5Q_box(nullptr);
		map.insert(i, t);
		QString s = "Test box N" + QString::number(i);
		t->UK5Q_setLabel(s);
		ui->UK5_verticalLayout->addWidget(t);
	}
	ui->UK5_scrollArea->resize(451,365	);
	ui->UK5_scrollArea->setWidgetResizable(false);
	
}
