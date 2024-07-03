#pragma once

#include "framework/entities/entity_collider.h"
#include "framework/entities/entity_enemy.h"

#include "graphics/mesh.h"
#include "graphics/texture.h"
#include "graphics/shader.h"
#include "graphics/material.h"

#include "framework/input.h"
#include "framework/camera.h"
#include "framework/input.h"
#include "framework/animation.h"

#include "game/game.h"
#include "game/world.h"


class EntityPlayer : public EntityCollider {

public:
	EntityPlayer();
	EntityPlayer(Mesh* mesh, const Material& material, const std::string& name = "");
	~EntityPlayer() {};

	// Vel in move the player
	Vector3 velocity = Vector3(0.0f);
	float healthbar = 100.0;
	float walk_speed = 1.0f;

	// Methods overwritten from base class
	void render(Camera* camera) override;
	void update(float seconds_elapsed) override;

};
