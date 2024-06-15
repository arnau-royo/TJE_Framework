#ifndef ENTITY_PLAYER_H
#define ENTITY_PLAYER_H

#include "graphics/mesh.h"
#include "graphics/texture.h"
#include "graphics/shader.h"
#include "graphics/material.h"

#include "framework/input.h"
#include "framework/camera.h"
#include "framework/entities/entity_enemy.h"
#include "framework/entities/entity_mesh.h"
#include "framework/animation.h"


class EntityPlayer : public EntityMesh {

public:
	EntityPlayer();
	EntityPlayer(Mesh* mesh, const Material& material, const std::string& name = "");
	~EntityPlayer() {};

	// Vel in move the player
	Vector3 velocity;
	float healthbar = 100.0;
	float walk_speed = 0.5f;

	// Methods overwritten from base class
	void render(Camera* camera);
	void update(float seconds_elapsed);
};
#endif // !entity_player.h
