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
    static std::pair<UK5B_varD,UK5Q_box>    UK5Q_Dinit(const QString&, const bool, const double, const QMap<int, QWidget*>&, const QString&);
    static std::pair<UK5B_varI,UK5Q_box>    UK5Q_Iinit(const QString&, const bool, const int, const QMap<int, QWidget*>&, const QString&);
    static std::pair<UK5B_varVD,UK5Q_box>   UK5Q_VDinit(const QString&, const bool, const std::vector<double>, const QMap<int, QWidget*>&, const QString&);
    static std::pair<UK5B_varVI,UK5Q_box>   UK5Q_VIinit(const QString&, const bool, const std::vector<int>, const QMap<int, QWidget*>&, const QString&);

    UK5Q_box box_vr,box_bb,box_nog,box_b,box_h,box_hog,box_nl,box_l,box_qst,box_cct,box_n,box_psh;
    UK5Q_box box_dog,box_nn,box_xn;
    UK5Q_box box_ll,box_pc,box_pd,box_dz,box_dy,box_dx,box_rbb,box_rb,box_rw,box_rh,box_rhog,box_rll,box_rl;

public slots:
    static void UK5Q_newtext();
private:
    Ui::MainWindow* ui;
private slots:
    static void UK5Q_exit();
    static void UK5Q_eval();

};