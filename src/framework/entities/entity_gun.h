#pragma once

#include "framework/animation.h"
#include "framework/extra/pathfinder/PathFinder.h"
#include "framework/extra/pathfinder/Dijkstra.h"
#include "graphics/material.h"
#include "game/world.h"
#include "game/entities/entity_player.h"
#include "graphics/texture.h"
#include "graphics/shader.h"
#include "entity.h"
#include "entity_mesh.h"
#include "framework/extra/coldet/coldet.h"

class EntityGun : public EntityMesh {
	
	int ammo = 100;
	float max_range = 200.0;

	EntityGun();
	EntityGun(Mesh* mesh, Material* mat, const std::string& name = ""):
		EntityMesh(mesh, material, name) {};
	~EntityGun() {};

	void fire(Vector3 origin, Vector3 direction);
};
