#pragma once

#include "framework/entities/entity_mesh.h"
#include "framework/animation.h"
#include "framework/extra/pathfinder/PathFinder.h"
#include "framework/extra/pathfinder/Dijkstra.h"
#include "graphics/material.h"
#include "game/world.h"
#include "framework/input.h"
#include "game/entities/entity_player.h"
#include "graphics/texture.h"
#include "graphics/shader.h"
#include "framework/entities/entity_bullet.h"

class EntityGun : public EntityMesh {
	
	int ammo = 100;
	float max_range = 200.0;

	EntityGun();
	EntityGun(Mesh* mesh, const std::string& name = "");
	~EntityGun() {};

	void fire(Vector3 origin, Vector3 direction);
};