#pragma once
#include "stage.h"
#include "framework/audio.h"


#include <array>

class EntityMissile;
class EntityHealth;
class EntityPower;
class EntityMesh;

class PlayStage : public Stage {
    eStages type = PLAY_STAGE;

public:
    PlayStage();

    // Methods overwritten from base class
    void render();
    void update(float delta_time);

   // void onKeyDown(SDL_KeyboardEvent event);

    void onEnter(Stage* previousStage);
    void onLeave(Stage* nextStage) {};
};
