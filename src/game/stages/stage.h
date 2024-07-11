#pragma once

#include "framework/includes.h"
#include "framework/framework.h"
#include <map>

#include "framework/entities/entity_ui.h"



enum eStages {
	UNDEFINED,
	INTRO_STAGE,
	PLAY_STAGE,
	END_STAGE,
};


class Stage {
	
	eStages type = UNDEFINED;

public:

	Stage() {};


	virtual void onEnter(Stage* previousStage) {};
	virtual void onLeave(Stage* nextStage) {};

	virtual void onButtonPressed(eButtonId buttonId) {}

	virtual void onKeyDown(SDL_KeyboardEvent event) {};
	virtual void onMouseWheel(SDL_MouseWheelEvent event) {};
	virtual void onResize(int width, int height) {};
	virtual void onMouseButtonDown(SDL_MouseButtonEvent event) {};
	virtual void onMouseButtonUp(SDL_MouseButtonEvent event) {};

	virtual void render() {};
	virtual void update(float seconds_elapsed) {};

};

class StageManager : public Stage {

	static StageManager* instance;

public:

	static StageManager* get_instance() {
		if (instance != nullptr)
			return instance;
		instance = new StageManager();
		return instance;
	}

	Stage* current = nullptr;
	std::map<std::string, Stage*> stages;

	StageManager() {};

	void goTo(const std::string& stageName)
	{
		Stage* nextStage = stages[stageName];
		if (!nextStage)
			return;

		if (current) {
			current->onLeave(nextStage);
		}

		nextStage->onEnter(current);

		// assign new current
		current = nextStage;
	}

	void render();
	void update(float seconds_elapsed) { if (current) current->update(seconds_elapsed); }
};

