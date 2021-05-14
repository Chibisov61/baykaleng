// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "UK5Q_box.h"
#include "UK5Q_form.h"

using namespace std;

// виджет ввода параметра
UK5Q_box::UK5Q_box(QWidget* parent) : QWidget(parent)
{
		ui = new Ui::UK5Q_box();
		ui->setupUi(this);

		this->setEnabled(true);

		UK5Q_setMode(false);
		UK5Q_state(false);

		const int n = parent->children().count() - 1;
		QRect g = this->geometry();
		const auto h = g.height();
		const QPoint p = { g.x(), g.y() + h * n };
		if (n) this->move(p);

		connect(ui->UK5Q_check, SIGNAL(stateChanged(int)), this, SLOT(UK5Q_state(int)));
		connect(ui->UK5Q_input, SIGNAL(editingFinished()), this, SLOT(UK5Q_slot()));
}

void UK5Q_box::UK5Q_setLabel(const QString& label) const
{
	ui->UK5Q_label->setText(label);
}

void UK5Q_box::UK5Q_setMode(const bool mode) const
{
	ui->UK5Q_check->setEnabled(mode);
}

int UK5Q_box::UK5Q_getMode() const
{
	return ui->UK5Q_check->checkState();
}

auto UK5Q_box::UK5Q_state(const int state) const -> void
{
	switch(state)
	{
	case 0:
		ui->UK5Q_input->setEnabled(false);
		ui->UK5Q_input->setReadOnly(true);
		break;
	case 1:
		ui->UK5Q_input->setEnabled(true);
		ui->UK5Q_input->setReadOnly(true);
		break;
	case 2:
		ui->UK5Q_input->setEnabled(true);
		ui->UK5Q_input->setReadOnly(false);
		break;
	default: ;
	}
}

void UK5Q_box::UK5Q_slot()
{
	emit UK5Q_edit(this->objectName());	
}

double UK5Q_box::UK5Q_getValue() const
{
		bool ok;
		const double x = ui->UK5Q_input->text().toDouble(&ok);
		return((ok) ? x : -1.);
}

std::vector<double> UK5Q_box::UK5Q_getVector() const
{
	vector<double> ret = {};
		bool ok;
		const QString str = ui->UK5Q_input->text();
		const QStringList list = str.split(";");
		for (QStringList::const_iterator itr = list.constBegin(); itr != list.constEnd(); ++itr)
		{
			if ((*itr).isEmpty())
			{
				ret.push_back(0);
				ok = true;
			}
			else
			{
				if (!(*itr).contains(":",Qt::CaseInsensitive)) 
					ret.push_back((*itr).toDouble(&ok));
				else
				{
					const QStringList list2 = (*itr).split(":");
					const double f = list2[0].toDouble(&ok);
					const int c = list2[1].toInt(&ok);
					const double s = (list2.size() == 2) ? 1 : list2[2].toDouble(&ok);
					for (int i = 1; i <= c; i += 1) ret.push_back(f+((i-1)*s));
				}
			}
			
			if (!ok)
			{
				ret = {-1.};
				break;
			}
		}
	return(ret);
}

int UK5Q_box::UK5Q_getValueI() const
{
		bool ok;
		const int x = ui->UK5Q_input->text().toInt(&ok);
		return((ok) ? x : -1);
}

std::vector<int> UK5Q_box::UK5Q_getVectorI() const
{
	vector<int> ret = {};
		bool ok;
		const QString str = ui->UK5Q_input->text();
		const QStringList list = str.split(";");
		for (QStringList::const_iterator itr = list.constBegin(); itr != list.constEnd(); ++itr)
		{
			if ((*itr).isEmpty())
			{
				ret.push_back(0);
				ok = true;
			}
			else
			{
				if (!(*itr).contains(":",Qt::CaseInsensitive)) 
					ret.push_back((*itr).toInt(&ok));
				else
				{
					const QStringList list2 = (*itr).split(":");
					const int f = list2[0].toInt(&ok);
					const int c = list2[1].toInt(&ok);
					const int s = (list2.size() == 2) ? 1 : list2[2].toInt(&ok);
					for (int i = 1; i <= c; i += 1) ret.push_back(f+((i-1)*s));
				}
			}
			
			if (!ok)
			{
				ret = {-1};
				break;
			}
		}
	return(ret);
}

void UK5Q_box::UK5Q_setValue(const double x) const
{
	ui->UK5Q_input->setText(QString::number(x));
}

void UK5Q_box::UK5Q_setVector(const vector<double>& v) const
{
	QString s = "";
	for (double it : v)
		s.append(QString::number(it)).append(";");
			
	ui->UK5Q_input->setText(s);
}

void UK5Q_box::UK5Q_setValueI(const int x) const
{
	ui->UK5Q_input->setText(QString::number(x));
}

void UK5Q_box::UK5Q_setVectorI(const std::vector<int>& v) const
{
	QString s = "";
	for (int it : v)
		s.append(QString::number(it)).append(";");
			
	ui->UK5Q_input->setText(s);
}

