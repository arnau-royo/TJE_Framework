#pragma once

#include "framework/entities/entity_collider.h"
#include "framework/animation.h"
#include "framework/extra/pathfinder/PathFinder.h"
#include "framework/extra/pathfinder/Dijkstra.h"
#include "graphics/material.h"
#include "framework/input.h"

enum eFSMStates {
	PATROL, // o IDLE
	SEARCH_PLAYER,
	ATTACK,
	DIE,
	DANCE
};

struct WayPoint : public DijkstraNode
{
	WayPoint() {}
	WayPoint(const Vector3& position) : position(position) { }

	void addLink(WayPoint* waypoint, float distance) {
		addChild(waypoint, distance);
		waypoint->addChild(this, distance);
	}

	Vector3 position;

};

class EntityEnemy : public EntityCollider {

	float fov = 0.0f;
	float max_sight_distance = 0.0f;

	eFSMStates state = PATROL;

	//Waypoints
	bool walk_forwards = true;
	int waypoint_index = 0;
	std::vector<WayPoint> path;


	float mod = 1.0;
	float ar = 0.2; //Distancia a la que l'enemic fa mal (ataca)
	float healthbar;
	float damage = 15.0 * mod; //It increases with the diffculty mode

	std::vector<EntityEnemy> enemies;

public:

	EntityEnemy() {};
	EntityEnemy(Mesh* mesh, const std::string& name = "");
	~EntityEnemy();

	// Vel in move the enemy
	Vector3 velocity;

	float walk_speed = 0.2f;

	// Methods overwritten from base class
	void render(Camera* camera);
	void update(float seconds_elapsed);

	//Methods from class
	void dif_mod(int difficulty);
	void choosedrop();
	void spawn_drop();

	bool inLineOfSight(const Vector3& position);
	void lookAtTarget(Vector3 target, float seconds_elapsed);
	void followPath(float seconds_elpased);
};