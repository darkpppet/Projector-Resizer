#include "ui-integration.h"
#include "projector-resizer.h"

#include <obs-frontend-api.h>
#include <obs-module.h>

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE("projector-resizer", "en-US")

// 프론트엔드 이벤트 콜백
static void OnFrontendEvent(enum obs_frontend_event event, void* private_data)
{
	if (event == OBS_FRONTEND_EVENT_FINISHED_LOADING)
	{
		ProjectorResizerUIIntegration::InjectButtonToControlsDock();
		ProjectorResizerUIIntegration::InjectMenuToMenuBar();
	}
}

bool obs_module_load()
{
	obs_module_set_locale(obs_get_locale());

	ProjectorResizer::LoadConfig(); // OBS가 켜질 때 저장된 설정 파일 읽어오기

	obs_frontend_add_event_callback(OnFrontendEvent, nullptr);
	return true;
}

void obs_module_unload()
{
	obs_frontend_remove_event_callback(OnFrontendEvent, nullptr);
}