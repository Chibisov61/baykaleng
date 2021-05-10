#pragma once

#include "ui_UK5Q_form.h"

class UK5Q_form : public QMainWindow
{
    Q_OBJECT

public:
    explicit UK5Q_form(QWidget *parent = Q_NULLPTR);

private:
    Ui::MainWindow *ui;
};
