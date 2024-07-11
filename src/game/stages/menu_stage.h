#pragma once

#include "stage.h"
#include "framework/entities/entity_ui.h"

class MenuStage : public Stage {
	eStages type = MENU_STAGE;
public: 
	MenuStage();

	EntityUI* background = nullptr;

	EntityUI* play_button = nullptr;
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