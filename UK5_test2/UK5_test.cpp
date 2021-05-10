#include "UK5_test.h"

void UK5_test::UK5_click() const
{
	box21->UK5_setVector(box12->UK5_getVector());
}

UK5_test::UK5_test(QWidget *parent) : QMainWindow(parent)
{
    ui = new Ui::UK5_testClass();
    ui->setupUi(this);

	const QString label11 = "Test11";
    box11 = new UK5_box(ui->scrollAreaWidgetContents_1);
	box11->UK5_setLabel(label11);
    box11->UK5_setValue(12.3);
    box11->UK5_setMode(true);
    box11->show();
	
    const int h = box11->geometry().height();
    QRect g = box11->geometry();

    const QString label12 = "Test12";
  	box12 = new UK5_box(ui->scrollAreaWidgetContents_1);
    box12->UK5_setLabel(label12);
    g.moveBottom(2*h);
    box12->setGeometry(g);
    box12->UK5_setVector({ 45.6, 78.9 });
    box12->UK5_setMode(true);
    box12->show();

    const QString label21 = "Test21";
    box21 = new UK5_box(ui->scrollAreaWidgetContents_2);
    box21->UK5_setLabel(label21);
    box21->UK5_setValue(0.00);
    box21->UK5_state(1);
    box21->show();

	connect(ui->UK5_OK, SIGNAL(clicked()), this, SLOT(UK5_click()));
    
}


