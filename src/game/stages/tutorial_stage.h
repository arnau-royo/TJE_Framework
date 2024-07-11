#pragma once
#include "stage.h"
#include "framework/audio.h"


#include <array>

class EntityMissile;
class EntityHealth;
class EntityPower;
class EntityMesh;

class TutorialStage : public Stage {
    eStages type = TUTORIAL_STAGE;

public:
    TutorialStage();

    // Methods overwritten from base class
    void render();
    void update(float delta_time);

    void onEnter(Stage* previousStage);
    void onLeave(Stage* nextStage) {};
};