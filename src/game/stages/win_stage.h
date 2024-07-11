#pragma once

#include "stage.h"
#include "framework/entities/entity_ui.h"
#include "framework/audio.h"

class WinStage : public Stage {
	eStages type = WIN_STAGE;


public:
	WinStage();

	EntityUI* background = nullptr;

	EntityUI* exit_button = nullptr;

	//Methods overwritten from base class
	void render();
	void update(float seconds_elapsed);

	void onButtonPressed(eButtonId buttonId);

	void onEnter(Stage* previousStage);
	void onKeyDown(SDL_KeyboardEvent event);
	void onMouseButtonUp(SDL_MouseButtonEvent event);
	void onResize(int width, int height);
};