#include "win_stage.h"

#include "framework/camera.h"
#include "framework/input.h"
#include "framework/entities/entity_ui.h"

#include "graphics/texture.h"

#include "game/game.h"
#include "game/world.h"

WinStage::WinStage()
{
    int width = Game::instance->window_width;
    int height = Game::instance->window_height;

    Material background_mat;
    background_mat.diffuse = Texture::Get("data/textures/ui/Victory.png");
    background = new EntityUI(Vector2(width * 0.5, height * 0.5), Vector2(width, height), background_mat);

    Material exit_mat;
    exit_mat.diffuse = Texture::Get("data/textures/ui/exitButton.png");
    exit_button = new EntityUI(Vector2(width * 0.5, 500), Vector2(250, 250), exit_mat, eButtonId::ExitButton);

    background->addChild(exit_button);
}

void WinStage::onEnter(Stage* previousStage)
{
    Game::instance->setMouseLocked(false);
    HCHANNEL channel = Audio::Play("data/audio/victory.wav", 0.5);
}

void WinStage::render()
{
    background->render(World::get_instance()->camera2D);
}

void WinStage::update(float seconds_elapsed)
{
    background->update(seconds_elapsed);
}

void WinStage::onButtonPressed(eButtonId buttonId)
{
    switch (buttonId) {
    case ExitButton:
        exit(0);
        break;
    }

}

void WinStage::onKeyDown(SDL_KeyboardEvent event)
{
    switch (event.keysym.sym)
    {
    case SDLK_INSERT:
    {}
    }
}

void WinStage::onMouseButtonUp(SDL_MouseButtonEvent event)
{
    switch (event.button)
    {
    case SDL_BUTTON_LEFT:
        break;
    }
}

void WinStage::onResize(int width, int height)
{
    World::get_instance()->camera->aspect = width / (float)height;
}
