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

#define RENDER_DEBUG

EntityEnemy::EntityEnemy(Mesh* mesh, const std::string& name) : EntityCollider(mesh, material, name)
{
	Material enemy_material;

	if (name == "zombie_1") {

		enemy_material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
		enemy_material.diffuse = new Texture();
		enemy_material.diffuse->load("data/textures/zombie/zombie1.png");

		this->fov = 100.0f; //Es pot jugar amb l'angle de visió
		this->max_sight_distance = 10.0f; //La ditancia fins a on et detecta

		this->healthbar = 90;

	}
	else {
		enemy_material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
		enemy_material.diffuse = new Texture();
		enemy_material.diffuse->load("data/textures/zombie/zombie2.png");

		this->fov = 120.0f; //Es pot jugar amb l'angle de visió
		this->max_sight_distance = 15.0f;

		this->healthbar = 150;
	}
	
	this->mesh = mesh;
	this->material = enemy_material;
	this->name = name;
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
	m.setRotation(fov * 0.5f * DEG2RAD, Vector3::UP);  //La mitat del fov cap a un cantó

	vertices.push_back(pos);
	vertices.push_back(pos + m.rotateVector(front));

	m.setRotation(-fov * 0.5f * DEG2RAD, Vector3::UP); //La mitat del fov cap l'altre cantó

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

	EntityMesh::render(camera);

}

void EntityEnemy::update(float seconds_elapsed) {
	
	Entity* target = (Entity*)World::get_instance()->player;
	Vector3 player_pos = target->getGlobalMatrix().getTranslation();
	bool in_sight = inLineOfSight(player_pos);

	if (state == PATROL) {
		
		//walk animation
		
		followPath(seconds_elapsed);
		
		if (this->healthbar == 0.0) {
			state = DIE;  //Si no té vida, mor
		}
		if (in_sight) {
			state = SEARCH_PLAYER;  //Mentre patrulla també va buscant el player (dins del seu angle de visió)
		}
	}
	else if (state == SEARCH_PLAYER)
	{
		Vector3 origin = model.getTranslation();
		Matrix44 rotation = lookAtTarget(player_pos, seconds_elapsed);

		Vector3 enemy_velocity = rotation.rotateVector(Vector3(0, 0, -1));

		//pegar lo que tenia
		
		Vector3 position = model.getTranslation();
		
		//Check collisions with world entities

		std::vector<sCollisionData> collisions;
		std::vector<sCollisionData> ground_collisions;

		for (auto entity : World::get_instance()->root.children) {

			EntityCollider* ec = dynamic_cast<EntityCollider*>(entity);
			if (ec != nullptr)
				ec->getCollisions(position + velocity * seconds_elapsed, collisions, ground_collisions, static_cast<eCollisionFilter> (ec->getLayer()));
		}

		//Enviornment collisions
		for (const sCollisionData& collision : collisions) {
			//Move along wall when colliding
			Vector3 newDir = velocity.dot(collision.colNormal) * collision.colNormal;
			velocity.x -= newDir.x;
			velocity.y -= newDir.y;
			velocity.z -= newDir.z;
		}

		//Ground collisions
		bool is_grounded = false;

		for (const sCollisionData& collision : ground_collisions) {
			//If normal is pointing upwards, it means it's a floor collision
			float up_factor = fabsf(collision.colNormal.dot(Vector3::UP));
			if (up_factor > 0.8) {
				is_grounded = true;
			}
			if (collision.colPoint.y > (position.y * velocity.y * seconds_elapsed)) {
				position.y = collision.colPoint.y;
			}
		}

		/*
		//Gravity for falling
		if (!is_grounded) {
			velocity.y -= 0.9f * seconds_elapsed;
		}
		else if (Input::wasKeyPressed(SDL_SCANCODE_SPACE)) {
			velocity.y = 2.0f;
		}
		*/


		model.setTranslation(position);
		model = model * rotation; // cambiar orden si no va
		
		if (this->healthbar == 0.0) {
			state = DIE;  //Si no té vida, mor
		}
		if (!in_sight) {
			state = PATROL;  //Si ja no el detecta torna a patrullar
		}

		if (distance(World::get_instance()->player) < ar) {
			state = ATTACK;  //Si està a prop ataca
		}
	}
	else if (state == ATTACK)
	{
		//play.animation
	
		//TODO: Aplicar el mal a la salut del player (ho fa a un video del final) (jo he pensat que amb el metode per agafar player de world i una funció dins de player per modificar la vida)
		//(World::get_instance()->player);

		if (this->healthbar == 0.0) {
			state = DIE;  //Si no té vida, mor
		}
		else if (distance(World::get_instance()->player) > ar) {
			state = SEARCH_PLAYER;  //Si ja no està a prop deixa d'atacar
		}
		else if (!in_sight) {
			state = PATROL;  //Si ja no el detecta torna a patrullar
		}
	}
	else if (state == DIE) {
		
		//play animació
		World::get_instance()->removeEntity(World::get_instance()->enemy);
		spawn_drop();
	}
	else if (state == DANCE) {
		//play animació

		if (this->healthbar == 0.0) {
			state = DIE;  //Si no té vida, mor
		}
		else if (distance(World::get_instance()->player) > ar) {
			state = SEARCH_PLAYER;  //Si ja no està a prop deixa d'atacar
		}
		/*
		else if (!in_sight) {
			state = PATROL;  //Si ja no el detecta torna a patrullar
		}
		*/
	}

	EntityCollider::update(seconds_elapsed);
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

Matrix44 EntityEnemy::lookAtTarget(const Vector3 position, float seconds_elapsed)
{
	//Rotate model to look at position
	float angle = model.getYawRotationToAimTo(position);
	float rotation_speed = 4.0f * seconds_elapsed;		//Velocitat a la que rota l'enemic
	Matrix44 rotation_mat;
	rotation_mat.rotate(angle * rotation_speed, Vector3::UP);
	return rotation_mat;
	//model.rotate(angle * rotation_speed, Vector3::UP);
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

		// “points” is a vector3 array storing our waypoints

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
			// Path found! Iterate through “solution” and
		// store the path in your AI data structure
			for (const auto& waypoint : solution) {
				path.push_back(waypoint->position);
			}
		}

	}
}

