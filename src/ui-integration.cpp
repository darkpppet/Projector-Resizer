#include "ui-integration.h"

#include "projector-resizer.h"
#include "settings-tab.h"

#include <obs-frontend-api.h>
#include <obs-module.h>

#include <QDialog>
#include <QDockWidget>
#include <QMainWindow>
#include <QMenuBar>
#include <QPushButton>
#include <QShortcut>
#include <QVBoxLayout>

namespace ProjectorResizerUIIntegration
{
void InjectButtonToControlsDock()
{
	const QMainWindow* main_window = static_cast<QMainWindow*>(obs_frontend_get_main_window());
	if (!main_window)
		return;

	const QDockWidget* controls_dock = main_window->findChild<QDockWidget*>("controlsDock");
	if (!controls_dock || !controls_dock->widget())
		return;

	const QWidget* dock_content = controls_dock->widget();
	QList<QPushButton*> native_buttons = dock_content->findChildren<QPushButton*>();
	if (native_buttons.isEmpty())
		return;

	const QPushButton* target_btn = native_buttons.last();
	QWidget* button_container = target_btn->parentWidget();
	QBoxLayout* layout = qobject_cast<QBoxLayout*>(button_container->layout());
	if (!layout)
		return;

	// 리사이즈 실행 버튼 생성
	QPushButton* btn_resize = new QPushButton(QString::fromUtf8(obs_module_text("Button.Resize")), button_container);
	btn_resize->setSizePolicy(target_btn->sizePolicy());
	btn_resize->setMinimumSize(target_btn->minimumSize());
	btn_resize->setMaximumSize(target_btn->maximumSize());
	btn_resize->setFont(target_btn->font());

	// 스페이서(투명 스프링) 바로 앞 위치 계산해서 삽입
	int target_index = layout->count();
	for (int i = 0; i < layout->count(); ++i)
	{
		QLayoutItem* item = layout->itemAt(i);
		if (item && item->spacerItem())
		{
			target_index = i;
			break;
		}
	}
	if (target_index == layout->count() && target_btn)
	{
		const int last_btn_idx = layout->indexOf(target_btn);
		if (last_btn_idx != -1)
			target_index = last_btn_idx + 1;
	}

	layout->insertWidget(target_index, btn_resize);

	// 클릭 시 리사이즈 실행 연결
	QObject::connect(btn_resize, &QPushButton::clicked, &ProjectorResizer::Execute);
}

void InjectMenuToMenuBar()
{
	const QMainWindow* main_window = static_cast<QMainWindow*>(obs_frontend_get_main_window());
	if (!main_window)
		return;

	QMenuBar* menu_bar = main_window->menuBar();
	if (!menu_bar)
		return;

	// 최상단 메뉴 생성
	QMenu* my_menu = new QMenu(QString::fromUtf8(obs_module_text("Plugin.Name")), menu_bar);
	menu_bar->addMenu(my_menu);

	// 그 아래 설정 항목 추가
	QAction* settings_action = new QAction(QString::fromUtf8(obs_module_text("Menu.Settings")), my_menu);
	my_menu->addAction(settings_action);

	QObject::connect(settings_action, &QAction::triggered, &ProjectorResizerUIIntegration::OpenSettingsDialog);
}

void OpenSettingsDialog()
{
	QMainWindow* main_window = static_cast<QMainWindow*>(obs_frontend_get_main_window());
	if (!main_window)
		return;

	// 모달(Modal) 팝업 창 생성
	QDialog* dialog = new QDialog(main_window);
	dialog->setWindowTitle(QString::fromUtf8(obs_module_text("Settings.Title")));

	// 다이얼로그 기본 여백을 최소화해서 탭 위젯이랑 자연스럽게 이어지도록 함
	QVBoxLayout* layout = new QVBoxLayout(dialog);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSizeConstraint(QLayout::SetFixedSize);

	// 우리가 만든 설정 탭 위젯 부착
	ProjectorSettingsTab* settings_tab = new ProjectorSettingsTab(dialog);
	layout->addWidget(settings_tab);

	// 탭 위젯에서 저장 완료 신호가 오면 다이얼로그 닫기
	QObject::connect(settings_tab, &ProjectorSettingsTab::SettingsApplied, dialog, &QDialog::accept);

	const QShortcut* enter_shortcut = new QShortcut(QKeySequence(Qt::Key_Return), dialog);
	const QShortcut* enter_shortcut_numpad = new QShortcut(QKeySequence(Qt::Key_Enter), dialog);

	// 키보드 Enter / Numpad Enter 누르면 저장 후 닫음
	auto handle_enter = [settings_tab, dialog]()
	{
		settings_tab->ApplySettings();
		dialog->accept();
	};

	QObject::connect(enter_shortcut, &QShortcut::activated, dialog, handle_enter);
	QObject::connect(enter_shortcut_numpad, &QShortcut::activated, dialog, handle_enter);

	// 창 띄우고 유저 입력 대기
	dialog->exec();
}
} // namespace ProjectorResizerUIIntegration