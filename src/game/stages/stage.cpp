#include "stage.h"
#include "game/game.h"
#include "graphics/shader.h"
#include "framework/input.h"
#include "framework/camera.h"
#include "graphics/texture.h"
#include "framework/utils.h"
#include "game/entities/entity_player.h"
#include "game/world.h"

#include <iomanip>
#include <sstream>

StageManager* StageManager::instance = nullptr;

void StageManager::render() { 
	if (current) 
		current->render(); 
}

//PLAY STAGE
PlayStage::PlayStage()
{
	int width = Game::instance->window_width;
	int height = Game::instance->window_height;
}

void PlayStage::onEnter(Stage* previousStage)
{
	// lock mouse
	SDL_ShowCursor(false);
	SDL_SetRelativeMouseMode(SDL_TRUE);
	Game::instance->mouse_locked = true;
}

void PlayStage::render()
{
	World::get_instance()->render();
}

void PlayStage::update(float seconds_elapsed)
{
	World::get_instance()->update(seconds_elapsed);
}

void PlayStage::onKeyDown(SDL_KeyboardEvent event)
{
	
}

void PlayStage::onResize(int width, int height)
{
	World::get_instance()->camera->aspect = width / (float)height;
	std::cout << "window resized: " << width << "," << height << std::endl;
	glViewport(0, 0, width, height);
	Game::instance->window_width = width;
	Game::instance->window_height = height;
}