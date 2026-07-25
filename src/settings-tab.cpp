#include "settings-tab.h"
#include "projector-resizer.h"

#include <obs-module.h>

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include "custom-widgets/qlength-spinbox.h"

ProjectorSettingsTab::ProjectorSettingsTab(QWidget* parent) : QWidget(parent)
{
	// 0. 최상위 메인 레이아웃
	QVBoxLayout* main_layout = new QVBoxLayout(this);
	main_layout->setContentsMargins(16, 16, 16, 16); // OBS 표준 다이얼로그 여백
	main_layout->setSpacing(12);

	// 1. "설정" 타이틀 라벨
	QLabel* title_label = new QLabel(QString::fromUtf8(obs_module_text("Settings.Settings")), this);
	title_label->setAlignment(Qt::AlignCenter);

	QFont title_font = title_label->font();
	title_font.setPointSize(title_font.pointSize() + 2);
	title_font.setBold(true);
	title_label->setFont(title_font);

	main_layout->addWidget(title_label);

	// 2. 스핀박스 영역
	QHBoxLayout* spin_layout = new QHBoxLayout();
	spin_layout->setSpacing(8);

	m_widthSpin = new QLengthSpinBox(this);
	m_widthSpin->setValue(ProjectorResizer::GetTargetWidth());

	m_heightSpin = new QLengthSpinBox(this);
	m_heightSpin->setValue(ProjectorResizer::GetTargetHeight());

	QLabel* multiply_label = new QLabel(" × ", this);
	multiply_label->setAlignment(Qt::AlignCenter);

	QFont mul_font = multiply_label->font();
	mul_font.setBold(true);
	multiply_label->setFont(mul_font);

	spin_layout->addStretch();
	spin_layout->addWidget(m_widthSpin);
	spin_layout->addWidget(multiply_label);
	spin_layout->addWidget(m_heightSpin);
	spin_layout->addStretch();

	main_layout->addLayout(spin_layout);
	main_layout->addStretch();

	// 3. 가로 구분선
	QFrame* line = new QFrame(this);
	line->setFrameShape(QFrame::HLine);
	line->setFrameShadow(QFrame::Sunken);

	main_layout->addWidget(line);

	// 4. 버튼 영역
	QHBoxLayout* btn_layout = new QHBoxLayout();
	btn_layout->setContentsMargins(0, 4, 0, 0);
	btn_layout->setSpacing(8);

	QPushButton* btnApply = new QPushButton(QString::fromUtf8(obs_module_text("Settings.OK")), this);
	QPushButton* btnReset = new QPushButton(QString::fromUtf8(obs_module_text("Settings.Reset")), this);

	btnReset->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	btnApply->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	btnReset->setMinimumWidth(75);
	btnApply->setMinimumWidth(75);

	btn_layout->addStretch();
	btn_layout->addWidget(btnApply);
	btn_layout->addWidget(btnReset);
	btn_layout->addStretch();

	main_layout->addLayout(btn_layout);

	// 5. 시그널 / 슬롯 바인딩
	connect(btnApply, &QPushButton::clicked, this, &ProjectorSettingsTab::OnApplyButtonClicked);
	connect(btnReset, &QPushButton::clicked, this, &ProjectorSettingsTab::ResetSettings);
}

void ProjectorSettingsTab::ApplySettings() const
{
	ProjectorResizer::SetTargetSize(m_widthSpin->value(), m_heightSpin->value());
}

void ProjectorSettingsTab::ResetSettings() const
{
	m_widthSpin->setValue(DEFAULT_WIDTH);
	m_heightSpin->setValue(DEFAULT_HEIGHT);
}

void ProjectorSettingsTab::OnApplyButtonClicked()
{
	ApplySettings();
	emit SettingsApplied();
}