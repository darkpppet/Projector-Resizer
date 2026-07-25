#pragma once

#include "qlength-spinbox.h"

QLengthSpinBox::QLengthSpinBox(QWidget* parent)	: QSpinBox(parent)
{
	setRange(LENGTH_MIN, LENGTH_MAX);
	setButtonSymbols(NoButtons);			// 화살표 제거
	setAlignment(Qt::AlignCenter);	// 중앙 정렬
}