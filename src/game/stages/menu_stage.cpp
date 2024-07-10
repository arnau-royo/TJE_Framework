#include "menu_stage.h"

#include "framework/camera.h"
#include "framework/input.h"
#include "framework/entities/entity_ui.h"

#include "graphics/texture.h"

#include "game/game.h"
#include "game/world.h"

MenuStage::MenuStage()
{
    int width = Game::instance->window_width;
    int height = Game::instance->window_height;

    Material background_mat;
    background_mat.diffuse = Texture::Get("data/textures/ui/menu.png");
    background = new EntityUI(Vector2(width * 0.5, height * 0.5), Vector2(width, height), background_mat);

    Material play_mat;
    play_mat.diffuse = Texture::Get("data/textures/ui/playButton.png");
    play_button = new EntityUI(Vector2(width * 0.35, 400), Vector2(250, 250), play_mat, eButtonId::PlayButton);

    Material exit_mat;
    exit_mat.diffuse = Texture::Get("data/textures/ui/exitButton.png");
    exit_button = new EntityUI(Vector2(width * 0.65, 400), Vector2(250, 250), exit_mat, eButtonId::ExitButton);

    background->addChild(play_button);
    background->addChild(exit_button);
}

void MenuStage::onEnter(Stage* previousStage)
{
    Game::instance->setMouseLocked(false);
}

void MenuStage::render()
{
    background->render(World::get_instance()->camera2D);
}

void MenuStage::update(float seconds_elapsed)
{
    background->update(seconds_elapsed);
}

void MenuStage::onButtonPressed(eButtonId buttonId)
{
    switch (buttonId) {
    case PlayButton:
        StageManager::get_instance()->goTo("playStage");
        break;
    case ExitButton:
        exit(0);
        break;
    }

}

void MenuStage::onKeyDown(SDL_KeyboardEvent event)
{
    switch (event.keysym.sym)
    {
    case SDLK_INSERT:
    {}
    }
}

void MenuStage::onMouseButtonUp(SDL_MouseButtonEvent event)
{
    switch (event.button)
    {
    case SDL_BUTTON_LEFT:
        break;
    }
}

void MenuStage::onResize(int width, int height)
{
    World::get_instance()->camera->aspect = width / (float)height;
}
