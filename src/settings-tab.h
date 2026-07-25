#pragma once

#include <QWidget>

class QLengthSpinBox;
class QComboBox;
class QSpinBox;

class ProjectorSettingsTab : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectorSettingsTab(QWidget* parent = nullptr);
    ~ProjectorSettingsTab() override = default;

    void ApplySettings() const;
    void ResetSettings() const;

signals:
    // ★ 확인 버튼이 눌렸을 때 다이얼로그에게 "나 닫아줘!" 라고 보낼 신호
    void SettingsApplied();

private:
    void OnApplyButtonClicked();

    QLengthSpinBox* m_widthSpin {nullptr};
    QLengthSpinBox* m_heightSpin {nullptr};
};
