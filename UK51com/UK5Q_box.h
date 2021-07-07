#pragma once

#include "ui_UK5Q_box.h"

class uk5_q_box final : public QWidget
{
	Q_OBJECT

public:
				explicit				uk5_q_box(QWidget* parent);
	
					void				set_label(const QString&) const;
					void				set_mode(bool) const;
	[[nodiscard]]	int					get_state() const;
	             	void				set_state(int) const;
					void				set_value(const QString&) const;
	[[nodiscard]]	QString				get_value() const;

public slots:	
					void				box_state(int);
					void				box_slot();

private:
			Ui::UK5Q_box				*ui_;

signals:
					void				check_signal(QString);
					void				edit_signal(QString);
};
