#pragma once

#include "game/entities/entity_player.h"
#include "entity_enemy.h"
#include "graphics/mesh.h"
#include "graphics/texture.h"
#include "graphics/shader.h"

#include "framework/camera.h"
#include "framework/input.h"

#include "game/game.h"
#include "game/world.h"
#include <memory>

#include <random>

#define RENDER_DEBUG

EntityEnemy::EntityEnemy(Mesh* mesh, const std::string& name) : EntityCollider(mesh, material, name)
{
	Material enemy_material;
	Material health_bar_mat;

	health_bar_mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/health_bar.fs");
	//int bar_width = 240;
	
	health_bar = new EntityUI(Vector2(50, 8), health_bar_mat);
	health_bar->is3D = true;

	if (name == "zombie_1") {

		//enemy_material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
		enemy_material.shader = Shader::Get("data/shaders/skinning.vs", "data/shaders/texture.fs");
		enemy_material.diffuse = new Texture();
		enemy_material.diffuse->load("data/textures/zombie/zombie1.png");

		this->fov = 100.0f; //Es pot jugar amb l'angle de visio
		this->max_sight_distance = 10.0f; //La ditancia fins a on et detecta

		this->healthbar = 100;

	}
	else {
		//Material enemy_material;
		//enemy_material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
		enemy_material.shader = Shader::Get("data/shaders/skinning.vs", "data/shaders/texture.fs");
		enemy_material.diffuse = new Texture();
		enemy_material.diffuse->load("data/textures/zombie/zombie2.png");

		this->fov = 120.0f; //Es pot jugar amb l'angle de visio
		this->max_sight_distance = 15.0f;

		this->healthbar = 150;
	}

	this->mesh = mesh;
	this->material = enemy_material;
	this->name = name;

	//animation stuff
	animator.playAnimation("data/animations/zombie_idle.skanim"); //Quan crea el enemy posa l'animació d'IDLE


	animator.addCallback("data/animations/zombie_attack1.skanim", [&](float t) { //Aplicar el mal quan fa un atac

		//PlayAudio
		HCHANNEL channel = Audio::Play("data/audio/damaged.mp3", 0.5);

		(World::get_instance()->player)->apply_damage(10.0f);

		}, 1.3f);

	animator.addCallback("data/animations/zombie_attack2.skanim", [&](float t) {

		//PlayAudio

		HCHANNEL channel = Audio::Play("data/audio/damaged.mp3", 0.5);

		(World::get_instance()->player)->apply_damage(10.0f);

		}, 1.3f);

}

EntityEnemy::~EntityEnemy()
{

}

void EntityEnemy::render(Camera* camera)
{
#ifdef RENDER_DEBUG

	//Render mesh
	Entity* target = (Entity*)World::get_instance()->player;
	bool in_sight = inLineOfSight(target->getGlobalMatrix().getTranslation());

	std::vector<Vector3> vertices;

	Vector3 pos = model.getTranslation();
	Vector3 front = model.frontVector();

	vertices.push_back(pos);
	vertices.push_back(pos + front);

	Matrix44 m;
	m.setRotation(fov * 0.5f * DEG2RAD, Vector3::UP);  //La mitat del fov cap a un canto

	vertices.push_back(pos);
	vertices.push_back(pos + m.rotateVector(front));

	m.setRotation(-fov * 0.5f * DEG2RAD, Vector3::UP); //La mitat del fov cap l'altre canto

	vertices.push_back(pos);
	vertices.push_back(pos + m.rotateVector(front));

	//Add to_targeer line
	Vector3 to_target = target->model.getTranslation() - model.getTranslation();
	vertices.push_back(pos);
	vertices.push_back(pos + to_target);

	Mesh mesh;
	mesh.vertices = vertices;

	Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/flat.fs");

	shader->enable();

	//Upload uniforms
	shader->setUniform("u_color", in_sight ? Vector4(1.f, 0.f, 0.f, 1.f) : Vector4(0.f, 1.f, 0.f, 1.f));
	shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	shader->setUniform("u_model", Matrix44());

	mesh.render(GL_LINE_STRIP);

	//Disable shader
	shader->disable();

#endif RENDER_DEBUG

	health_bar->render(World::get_instance()->camera2D);

	EntityMesh::render(camera);

}

void EntityEnemy::update(float seconds_elapsed) {

	Entity* target = (Entity*)World::get_instance()->player;
	Vector3 player_pos = target->getGlobalMatrix().getTranslation();
	bool in_sight = inLineOfSight(player_pos);

	if (state == eFSMStates::PATROL) {

		followPath(seconds_elapsed);

		if (this->healthbar == 0.0) {
			state = eFSMStates::DIE;  //Si no t? vida, mor

			//die animation
			animator.playAnimation("data/animations/zombie_die.skanim", false, 0.5f);
		}
		else if (in_sight) {
			state = eFSMStates::SEARCH_PLAYER;  //Mentre patrulla tamb? va buscant el player (dins del seu angle de visio)

			//run animation
			animator.playAnimation("data/animations/zombie_run.skanim", true, 0.5f);
		}

		//Rarely it starts dancing
		std::random_device rd;   // Obtain a random number from hardware
		std::mt19937 gen(rd());  // Seed the generator
		std::uniform_int_distribution<> distr(0, 10000000);  // Define the range

		if (distr(gen) == 0) {
			state = eFSMStates::DANCE;
			
			//dance animation
			animator.playAnimation("data/animations/zombie_dance.skanim", false, 0.05f);
		}

	}
	else if (state == eFSMStates::SEARCH_PLAYER)
	{
		
		Vector3 origin = model.getTranslation();
		lookAtTarget(player_pos, seconds_elapsed);
		model.translate(0.f, 0.f, seconds_elapsed);

		if (this->healthbar == 0.0) {	
			state = eFSMStates::DIE;  //Si no t? vida, mor

			//die animation
			animator.playAnimation("data/animations/zombie_die.skanim", false, 0.5f);

		}
		if (!in_sight) {
			state = eFSMStates::PATROL;  //Si ja no el detecta torna a patrullar o idle

			//animation idle
			animator.playAnimation("data/animations/zombie_idle.skanim", true, 0.5f);
		}

		if (distance(World::get_instance()->player) < ar) {
			state = eFSMStates::ATTACK;  //Si est? a prop ataca

			// Randomly select one attack animation
			std::random_device rd;   // Obtain a random number from hardware
			std::mt19937 gen(rd());  // Seed the generator
			std::uniform_int_distribution<> distr(0, 1);  // Define the range

			if (distr(gen) == 0) {
				animator.playAnimation("data/animations/zombie_attack1.skanim", true, 0.5f);
				//aplicar el mal amb els callbacks
			}
			else {
				animator.playAnimation("data/animations/zombie_attack2.skanim", true, 0.5f);
				//aplicar el mal amb els callbacks
			}
		}
	}
	else if (state == eFSMStates::ATTACK)
	{
		if (this->healthbar == 0.0) {	
			state = eFSMStates::DIE;  //Si no te vida, mor

			//die animation
			animator.playAnimation("data/animations/zombie_die.skanim", false, 0.5f);

		}
		else if (!in_sight) {
			state = eFSMStates::PATROL;  //Si ja no el detecta torna a patrullar

			//animation idle
			animator.playAnimation("data/animations/zombie_idle.skanim", true, 0.5f);
		}
		else if (distance(World::get_instance()->player) > ar) {
			state = eFSMStates::SEARCH_PLAYER;  //Si ja no est? a prop deixa d'atacar

			//walk animation
			animator.playAnimation("data/animations/zombie_run.skanim", true, 0.5f);
		}
	}
	else if (state == eFSMStates::DIE) {
		World::get_instance()->removeEntity(World::get_instance()->enemy); //Destroy the entity

		//Give some drop
		spawn_drop();

	}
	else if (state == eFSMStates::DANCE) {
		
		if (this->healthbar == 0.0) {
			state = eFSMStates::DIE;  //Si no t? vida, mor

			//die animation
			animator.playAnimation("data/animations/zombie_die.skanim", false, 0.5f);

		}
		else if (in_sight) {
			state = eFSMStates::SEARCH_PLAYER;  //Si detecta al player deixa de ballar i el segueix

			//run animation
			animator.playAnimation("data/animations/zombie_run.skanim", true, 0.5f);
		}
	}

	//Si es prem la lletra K el zombies comencen a ballar
	if (Input::wasKeyPressed(SDL_SCANCODE_K)) {
		state = eFSMStates::DANCE;

		animator.playAnimation("data/animations/zombie_dance.skanim", false, 0.5f);
	}

	//Si té velocitat i està al patrol, canvio la animació per WALK
	if (eFSMStates::PATROL && velocity.length() > .5f) {
		animator.playAnimation("data/animations/zombie_walk_patrol.skanim", true, 0.5f); //in case of some velocity = patrol, walk
	}


	health_bar->update3D(model.getTranslation() + Vector3(0, 0.2, 0)); //li passo la posició de l'enemic (una mica més a dalt)

	EntityCollider::update(seconds_elapsed);
}

void EntityEnemy::apply_damage(float damage)
{
	healthbar -= damage;

	if (healthbar < 0.0f) {
		healthbar = 0.0f;
	}

	if (name == "zombie_1") {
		health_bar->mask = healthbar / 100.0f;  //Divide for the total health
	}
	else {
		health_bar->mask = healthbar / 150.0f;
	}

	


}


bool EntityEnemy::inLineOfSight(const Vector3& position)
{
	Vector3 offset = Vector3(0.f, 0.1f, 0.f);
	Vector3 origin = model.getTranslation() + offset;
	Vector3 target = position + offset;
	Vector3 to_target = target - origin;

	float distance = to_target.length();
	to_target.normalize();
	Vector3 front = model.frontVector();
	front.normalize();

	// First step: Vision Code
	float angle = model.getYawRotationToAimTo(target);
	float half_fov_radians = fov * 0.5f * DEG2RAD;

	if (fabsf(angle) < half_fov_radians && distance < max_sight_distance)
	{
		// Second staep: check obstacles
		sCollisionData data = World::get_instance()->raycast(origin, to_target,
			eCollisionFilter::ALL ^ eCollisionFilter::ENEMY, distance); //excloem l'ENEMY

		return !data.collided;
	}

	return false;
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

void EntityEnemy::choosedrop()
{
	/*if (lastdrop == health) {
		drop = ammo;
	}
	else if (lastdrop == ammo) {
		drop = health;
	}*/
}

void EntityEnemy::spawn_drop()
{
	/*if (last_drop == "supply") {
		EntityMesh* drop = new EntityMesh(heal_mesh, heal_mat, "heal");
		last_drop = drop->name;
	}
	else if(last_drop == "heal") {
		EntityMesh* drop = new EntityMesh(supply_mesh, supply_mat, "supply");
		last_drop = drop->name;
	}*/
}

void EntityEnemy::lookAtTarget(const Vector3 position, float seconds_elapsed)
{
	//Rotate model to look at position
	float angle = model.getYawRotationToAimTo(position);
	float rotation_speed = 4.0f * seconds_elapsed;		//Velocitat a la que rota l'enemic
	model.rotate(angle * rotation_speed, Vector3::UP);
	//float angle_in_rad = acos(clamp(front.dot(target), -1.0f, 1.0f));
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

		// ?points? is a vector3 array storing our waypoints

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
			// Path found! Iterate through ?solution? and
		// store the path in your AI data structure
			for (const auto& waypoint : solution) {
				path.push_back(waypoint->position);
			}
		}

	}
}
