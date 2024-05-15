#pragma once

#include "framework/entities/entity.h"
#include "framework/animation.h"
#include "graphics/material.h"

class EntityPlayer : public Entity {

public:
	EntityPlayer();
	~EntityPlayer() {};

	// Vel in move the player
	Vector3 velocity;

	float walk_speed = 2.f;

	// Methods overwritten from base class
	void render(Camera* camera);
	void update(float delta_time);
};
