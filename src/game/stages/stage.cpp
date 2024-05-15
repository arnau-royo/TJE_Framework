#include "stage.h"
//#include "game/game.h"
//#include "graphics/shader.h"
//#include "framework/input.h"
//#include "framework/camera.h"
//#include "graphics/texture.h"
//#include "framework/utils.h"
//#include "game/entities/entity_player.h"

//#include <iomanip>
//#include <sstream>

StageManager* StageManager::instance = nullptr;

/*
	MenuStage
*/

//MenuStage::MenuStage()
//{
//
//}
//
//void MenuStage::render()
//{
//
//}
//
//void MenuStage::onEnter(Stage* previousStage)
//{
//
//}



/*
	PlayStage
*/

PlayStage::PlayStage()
{

}

void PlayStage::onEnter(Stage* previousStage)
{
	SDL_SetRelativeMouseMode(SDL_TRUE);
	Game::instance->mouse_locked = true;
}

void PlayStage::render()
{
	World* world = World::get_instance();
	float camera_yaw = world->camera_yaw;
	float camera_pitch = world->camera_pitch;

	Camera* camera = world->camera;

	Vector4 bgColor = world->bgColor;
	glClearColor(bgColor.x, bgColor.y, bgColor.z, bgColor.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	camera->enable();

	world->render();
}

void PlayStage::update(float delta_time)
{
	World* world = World::get_instance();
	world->update(delta_time);
}

void PlayStage::onKeyDown(SDL_KeyboardEvent event)
{
	std::cout << "window resized: " << width << "," << height << std::endl;
	glViewport(0, 0, width, height);
	camera->aspect = width / (float)height;
	window_width = width;
	window_height = height;
}

void PlayStage::onResize(int width, int height)
{
	World::get_instance()->camera->aspect = width / (float)height;
}

/*
	WinStage
*/
//MenuStage::MenuStage()
//{
//
//}
//
//void MenuStage::render()
//{
//
//}
//
//void MenuStage::onEnter(Stage* previousStage)
//{
//
//}
