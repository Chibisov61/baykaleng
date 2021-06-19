#pragma once

#include "ui_UK5Q_box.h"

class uk5_q_box final : public QWidget
{
	Q_OBJECT

public:
				explicit				uk5_q_box(QWidget* parent);
	
					void				uk5_q_set_label(const QString&) const;
					void				uk5_q_set_mode(bool) const;
	[[nodiscard]]	int					uk5_q_get_state() const;
	             	void				uk5_q_set_state(int) const;
					void				uk5_q_set_value(const QString&) const;
	[[nodiscard]]	QString				uk5_q_get_value() const;
	[[nodiscard]]	QVBoxLayout*		uk5_q_get_place() const;

public slots:	
					void				uk5_q_state(int);
					void				uk5_q_slot();

private:
			Ui::UK5Q_box				*ui_;

signals:
					void				check_signal(QString);
					void				edit_signal(QString);
};
