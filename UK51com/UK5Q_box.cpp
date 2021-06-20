// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "UK5Q_box.h"
#include "UK5Q_form.h"

using namespace std;

// виджет ввода параметра
uk5_q_box::uk5_q_box(QWidget* parent = nullptr) : QWidget(parent)
{
		ui_ = new Ui::UK5Q_box();
		ui_->setupUi(this);

		this->setEnabled(true);

		connect(ui_->UK5Q_check, SIGNAL(stateChanged(int)), this, SLOT(uk5_q_state(int)));
		connect(ui_->UK5Q_input, SIGNAL(editingFinished()), this, SLOT(uk5_q_slot()));
}

void uk5_q_box::uk5_q_set_label(const QString& label) const
{
	ui_->UK5Q_label->setText(label);
}

void uk5_q_box::uk5_q_set_mode(const bool mode) const
{
	ui_->UK5Q_check->setChecked(mode);
	ui_->UK5Q_check->setEnabled(mode);
}

int uk5_q_box::uk5_q_get_state() const
{
	return ui_->UK5Q_check->checkState();
}

void uk5_q_box::uk5_q_set_state(const int state) const
{
	const bool st = state;
	ui_->UK5Q_input->setEnabled(st);
	ui_->UK5Q_input->setReadOnly(!st);
}

void uk5_q_box::uk5_q_state(const int state)
{
	uk5_q_set_state(state);
	emit check_signal(this->objectName());	
}


void uk5_q_box::uk5_q_slot()
{
	emit edit_signal(this->objectName());	
}

void uk5_q_box::uk5_q_set_value(const QString& s) const
{
	ui_->UK5Q_input->setText(s);
}

QString uk5_q_box::uk5_q_get_value() const
{
	QString val = ui_->UK5Q_input->text();
	return val;
}

QVBoxLayout* uk5_q_box::uk5_q_get_place() const
{
	return(qobject_cast<QVBoxLayout*>(this->parent()));
}



