#pragma once

#include "ui_UK5Q_form.h"
#include "UK5B_river.h"
#include <vector>


class UK5Q_form : public QMainWindow
{
    Q_OBJECT

public:
    explicit UK5Q_form(QWidget *parent = Q_NULLPTR);
    static UK5B_varD  UK5Q_Dinit( const QString&, const bool, const double,              const QMap<int, QWidget*>&, const QString&);
    static UK5B_varI  UK5Q_Iinit( const QString&, const bool, const int,                 const QMap<int, QWidget*>&, const QString&);
    static UK5B_varVD UK5Q_VDinit(const QString&, const bool, const std::vector<double>, const QMap<int, QWidget*>&, const QString&);
    static UK5B_varVI UK5Q_VIinit(const QString&, const bool, const std::vector<int>,    const QMap<int, QWidget*>&, const QString&);

private:
    Ui::MainWindow *ui;
private slots:
    static void UK5Q_exit();
};
