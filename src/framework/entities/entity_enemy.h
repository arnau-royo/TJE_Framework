#pragma once

#include "framework/entities/entity_mesh.h"
#include "framework/animation.h"
#include "graphics/material.h"
#include "game/world.h"
#include "framework/input.h"
#include "game/entities/entity_player.h"

class EntityEnemy : public EntityMesh {

	enum eActions {SPAWN, CHASE, ATTACK, DIE, DANCE};

	float mod = 1.0;
	float ar = 15.0;
	float healthbar;
	float damage = 15.0 * mod;

	eActions state;

public:
	EntityEnemy();
	EntityEnemy(Mesh* mesh, const Material& material, const std::string& name = "");
	~EntityEnemy() {};

	// Vel in move the player
	Vector3 velocity;

	float walk_speed = 0.2f;

	// Methods overwritten from base class
	void render(Camera* camera);
	void update(float seconds_elapsed);
	void dif_mod(int difficulty);
};