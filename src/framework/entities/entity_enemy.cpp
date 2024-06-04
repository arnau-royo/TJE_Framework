#include "entity_enemy.h"
#include "graphics/mesh.h"
#include "graphics/texture.h"
#include "graphics/shader.h"
#include "game/entities/entity_player.h"

#include "framework/camera.h"
#include "framework/input.h"

#include "game/game.h"
#include "game/world.h"

EntityEnemy::EntityEnemy()
{
}

EntityEnemy::EntityEnemy(Mesh* mesh, const Material& material, const std::string& name) //TODO com fer diferents tipus d'enemic
{
	this->mesh = mesh;
	this->material = material;
	this->name = name;
}

void EntityEnemy::render(Camera* camera)
{
	//Render mesh
	EntityMesh::render(camera);
}

void EntityEnemy::update(float seconds_elapsed) {
	/*
	switch (state) {
		case SPAWN:
			render(camera);
			healthbar = 100.0;
			state = CHASE;
		case CHASE:
			chase_pl();
			if (distance(player) < ar)
				state = ATTACK;
		case ATTACK:
			attack();
			if (distance(player) > ar)
				state = CHASE;
		case DIE:
			~EntityEnemy();
		case DANCE:
			dance();
		}
		*/
}

void EntityEnemy::dif_mod(int difficulty)
{
	switch (difficulty) {
		case 0: //easy mode, no change
		case 1:	//normal mode
			mod = 1.25;
		case 2:
			mod = 1.5;
	}
}
