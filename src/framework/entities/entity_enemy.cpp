#include "entity_enemy.h"
#include "graphics/mesh.h"
#include "graphics/texture.h"
#include "graphics/shader.h"
#include "game/entities/entity_player.h"

#include "framework/camera.h"
#include "framework/input.h"

#include "game/game.h"
#include "game/world.h"
#include <memory>

EntityEnemy::EntityEnemy()
{
}

EntityEnemy::EntityEnemy(Mesh* mesh, const std::string& name) //TODO com fer diferents tipus d'enemic
{
	this->mesh = mesh;

	if (name == "zombie_1") {

		Material enemy_material;
		enemy_material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
		enemy_material.diffuse = new Texture();
		enemy_material.diffuse->load("data/textures/zombie/zombie1.png");

		this->material = enemy_material;
		this->name = name;
		this->healthbar = 100;
	}
	else {
		Material enemy_material;
		enemy_material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
		enemy_material.diffuse = new Texture();
		enemy_material.diffuse->load("data/textures/zombie/zombie2.png");

		this->material = enemy_material;
		this->name = name;
		this->healthbar = 150;
	}
	
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
			render(World::get_instance()->camera);
			healthbar = 100.0;
			state = CHASE;
		case CHASE:
			chase_pl();
			if (distance(World::get_instance()->player) < ar)
				state = ATTACK;
		case ATTACK:
			attack(World::get_instance()->player);
			if (distance(World::get_instance()->player) > ar)
				state = CHASE;
		case DIE:
			die();
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

void EntityEnemy::chase(EntityPlayer player)
{
	//walk animation
}

void EntityEnemy::attack(EntityPlayer player)
{
	//play.animation
	player.healthbar -= this->damage;
}

void EntityEnemy::die(EntityEnemy enemy)
{
	//play animaci�
	//World::get_instance()->entities_to_destroy.insert(enemy);
	enemy.~EntityEnemy();
}

bool EntityEnemy::isdead(EntityEnemy enemy) {
	if (enemy.healthbar == 0.0)
		return true;
	else
		return false;
}
void EntityEnemy::dance(EntityEnemy enemy)
{
}

void EntityEnemy::followPath(float seconds_elapsed)
{
	if (path.size())
	{
		Vector3 origin = model.getTranslation();
		Vector3 target = path[waypoint_index].position;

		lookAtTarget(target, seconds_elapsed);

		model.translate(0.f, 0.f, seconds_elapsed);

		float distance_to_target = (target - origin).length();

		//Point reached
		if (distance_to_target < 0.1f) {

			if (walk_forwards && waypoint_index + 1 == path.size() || !walk_forwards && waypoint_index - 1 < 0) {
				walk_forwards = !walk_forwards;
			}

			waypoint_index += (walk_forwards ? 1 : -1);
		}
	}
	else {

		if (World::get_instance()->waypoints.size() < 2)
			return;

		path.clear();

		const std::vector<Vector3>& points = World::get_instance()->waypoints;

		// �points� is a vector3 array storing our waypoints

		std::vector<WayPoint> wp_nodes;
		wp_nodes.resize(points.size());

		// Create a LINEAR path (1 connection per node)
		for (int i = 0; i < points.size() - 1; ++i) {
			wp_nodes[i].position = points[i];
			wp_nodes[i + 1].position = points[i + 1];
			wp_nodes[i].addLink(&wp_nodes[i + 1], (points[i] - points[i + 1]).length()
			);
		}

		// Create the PathFinder and PathAlgorithm stuff
		PathFinder<WayPoint> p;
		std::vector<WayPoint*> solution;
		p.setStart(wp_nodes[0]);
		p.setGoal(wp_nodes[wp_nodes.size() - 1]);

		bool r = p.findPath<Dijkstra>(solution);

		if (r) {
			// Path found! Iterate through �solution� and
		// store the path in your AI data structure
			for (const auto& waypoint : solution) {
				path.push_back(waypoint->position);
			}
		}

	}
}

