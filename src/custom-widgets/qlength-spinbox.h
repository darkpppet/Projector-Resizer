#pragma once

#include <QSpinBox>

class QLengthSpinBox : public QSpinBox
{
	Q_OBJECT

public:
	explicit QLengthSpinBox(QWidget *parent = nullptr);
	~QLengthSpinBox() override = default;
};
