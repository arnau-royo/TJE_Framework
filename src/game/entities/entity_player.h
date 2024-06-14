#pragma once

#include "graphics/mesh.h"
#include "graphics/texture.h"
#include "graphics/shader.h"
#include "graphics/material.h"

#include "framework/input.h"
#include "framework/camera.h"
#include "framework/input.h"
#include "framework/entities/entity_enemy.h"
#include "framework/entities/entity_collider.h"
#include "framework/animation.h"

#include "entity_player.h"

#include "game/game.h"
#include "game/world.h"


class EntityPlayer : public EntityCollider {

public:
	EntityPlayer();
	EntityPlayer(Mesh* mesh, Material* material, const std::string& name = "");
	~EntityPlayer() {};

	// Vel in move the player
	Vector3 velocity;
	float healthbar = 100.0;
	float walk_speed = 0.5f;

	// Methods overwritten from base class
	void render(Camera* camera);
	void update(float seconds_elapsed);
};
