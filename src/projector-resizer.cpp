#include "projector-resizer.h"

#include <obs-frontend-api.h>
#include <obs-module.h>
#include <obs.hpp>

#include <util/platform.h>
#include <util/util.hpp>

#include <QApplication>
#include <QMainWindow>

int ProjectorResizer::m_width = DEFAULT_WIDTH;
int ProjectorResizer::m_height = DEFAULT_HEIGHT;

void ProjectorResizer::SetTargetSize(const int width, const int height)
{
	m_width = std::clamp(width, LENGTH_MIN, LENGTH_MAX);
	m_height = std::clamp(height, LENGTH_MIN, LENGTH_MAX);
	SaveConfig();
}

void ProjectorResizer::LoadConfig()
{
	// 1. AppData 영역의 우리 플러그인 전용 config 절대 경로를 가져옴
	// (예: .../Roaming/obs-studio/plugin_config/projector-resizer/config.json)
	BPtr<char> config_path = obs_module_config_path(CONFIG_FILE);
	if (!config_path)
		return;

	// 2. 해당 경로에서 JSON 로드
	const OBSData settings = obs_data_create_from_json_file(config_path);
	if (!settings)
	{
		m_width = DEFAULT_WIDTH;
		m_height = DEFAULT_HEIGHT;

		SaveConfig();
		return;
	}

	const int saved_w = static_cast<int>(obs_data_get_int(settings, "width"));
	const int saved_h = static_cast<int>(obs_data_get_int(settings, "height"));

	m_width = std::clamp(saved_w, LENGTH_MIN, LENGTH_MAX);
	m_height = std::clamp(saved_h, LENGTH_MIN, LENGTH_MAX);
}

void ProjectorResizer::SaveConfig()
{
	// 1. 폴더 경로 가져오기
	BPtr<char> config_dir = obs_module_config_path("");
	if (!config_dir)
		return;

	// 해당 경로의 폴더가 없으면 상위 폴더까지 생성
	if (os_mkdirs(config_dir) != MKDIR_SUCCESS)
		return;

	// 2. 실제 저장할 파일의 절대 경로 가져오기
	BPtr<char> config_path = obs_module_config_path(CONFIG_FILE);
	if (!config_path)
		return;

	// 3. JSON 데이터 생성 및 안전 저장
	const OBSData settings = obs_data_create();
	if (settings)
	{
		obs_data_set_int(settings, "width", m_width);
		obs_data_set_int(settings, "height", m_height);

		// 임시 파일(tmp)로 먼저 쓰고 원본을 교체(bak)하는 safe 세이브
		obs_data_save_json_safe(settings, config_path, "tmp", "bak");
	}
}

void ProjectorResizer::Execute()
{
	ResizeActiveProjectors(m_width, m_height);
}

void ProjectorResizer::ResizeActiveProjectors(const int width, const int height)
{
	QWidgetList top_level_widgets = QApplication::topLevelWidgets();
	const QMainWindow* main_window = static_cast<QMainWindow*>(obs_frontend_get_main_window());

	for (QWidget* widget : top_level_widgets)
	{
		if (!widget->isVisible() || widget == main_window)
			continue;

		if (!widget->inherits("OBSProjector"))
			continue;

		if (widget->isFullScreen())
			widget->showNormal();

		widget->resize(width, height);
	}
}