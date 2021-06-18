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

		uk5_q_set_mode(false);
		uk5_q_state(0);

		connect(ui_->UK5Q_check, SIGNAL(stateChanged(int)), this, SLOT(UK5Q_state(int)));
		connect(ui_->UK5Q_input, SIGNAL(editingFinished()), this, SLOT(UK5Q_slot()));
}

void uk5_q_box::uk5_q_set_label(const QString& label) const
{
	ui_->UK5Q_label->setText(label);
}

void uk5_q_box::uk5_q_set_mode(const bool mode) const
{
	ui_->UK5Q_check->setEnabled(mode);
	const auto c = (mode) ? Qt::Checked : Qt::Unchecked;
	ui_->UK5Q_check->setCheckState(c);
}

int uk5_q_box::uk5_q_get_mode() const
{
	return ui_->UK5Q_check->checkState();
}

void uk5_q_box::uk5_q_state(const int state)
{
	switch(state)
	{
	case 0:
		ui_->UK5Q_input->setEnabled(false);
		ui_->UK5Q_input->setReadOnly(true);
		break;
	case 1:
		ui_->UK5Q_input->setEnabled(true);
		ui_->UK5Q_input->setReadOnly(true);
		break;
	case 2:
		ui_->UK5Q_input->setEnabled(true);
		ui_->UK5Q_input->setReadOnly(false);
		break;
	default: ;
	}
	emit uk5_q_check(this->objectName());	
}


void uk5_q_box::uk5_q_slot()
{
	emit uk5_q_edit(this->objectName());	
}

void uk5_q_box::uk5_q_set_value(const QString& s) const
{
	ui_->UK5Q_input->setText(s);
}

const QString& uk5_q_box::uk5_q_get_value() const
{
		return ui_->UK5Q_input->text();
}

QVBoxLayout* uk5_q_box::uk5_q_get_place() const
{
	return(qobject_cast<QVBoxLayout*>(this->parent()));
}



