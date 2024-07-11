#include "defeat_stage.h"

#include "framework/camera.h"
#include "framework/input.h"
#include "framework/entities/entity_ui.h"

#include "graphics/texture.h"

#include "game/game.h"
#include "game/world.h"

DefeatStage::DefeatStage()
{
    int width = Game::instance->window_width;
    int height = Game::instance->window_height;

    Material background_mat;
    background_mat.diffuse = Texture::Get("data/textures/ui/game_over.png");
    background = new EntityUI(Vector2(width * 0.5, height * 0.5), Vector2(width, height), background_mat);

    Material exit_mat;
    exit_mat.diffuse = Texture::Get("data/textures/ui/exitButton.png");
    exit_button = new EntityUI(Vector2(width * 0.5, 500), Vector2(250, 250), exit_mat, eButtonId::ExitButton);

    background->addChild(exit_button);
}

void DefeatStage::onEnter(Stage* previousStage)
{
    Game::instance->setMouseLocked(false);
}

void DefeatStage::render()
{
    background->render(World::get_instance()->camera2D);
}

void DefeatStage::update(float seconds_elapsed)
{
    background->update(seconds_elapsed);
}

void DefeatStage::onButtonPressed(eButtonId buttonId)
{
    switch (buttonId) {
    case ExitButton:
        exit(0);
        break;
    }

}

void DefeatStage::onKeyDown(SDL_KeyboardEvent event)
{
    switch (event.keysym.sym)
    {
    case SDLK_INSERT:
    {}
    }
}

void DefeatStage::onMouseButtonUp(SDL_MouseButtonEvent event)
{
    switch (event.button)
    {
    case SDL_BUTTON_LEFT:
        break;
    }
}

void DefeatStage::onResize(int width, int height)
{
    World::get_instance()->camera->aspect = width / (float)height;
}
