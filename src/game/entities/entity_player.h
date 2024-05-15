#pragma once

#include "framework/entities/entity_mesh.h"
#include "framework/animation.h"
#include "graphics/material.h"

class EntityPlayer : public EntityMesh {

public:
	EntityPlayer();
	~EntityPlayer() {};

	// Vel in move the player
	Vector3 velocity;

	float walk_speed = 2.f;

	// Methods overwritten from base class
	void render(Camera* camera);
	void update(float seconds_elapsed);
};
