#pragma once

#include "framework/includes.h"
#include "framework/framework.h"
#include "framework/entities/entity_collider.h"

class Camera;

enum eCollisionFilter {
	NONE = 0,
	FLOOR = 1 << 0,
	WALL = 1 << 1,
	PLAYER = 1 << 2,
	ENEMY = 1 << 3,
	SCENARIO = FLOOR | WALL,
	ALL = 0xFF
};

struct sCollisionData {
	Vector3 colPoint;
	Vector3 colNormal;
	float distance = 1e10f;
	bool collided = false;
	EntityCollider* collider = nullptr;
};

class Entity {

public:

	Entity() {}; 			// Constructor
	virtual ~Entity() {}; 	// Destructor

	std::string name;

	Matrix44 model;

	Entity* parent = nullptr;
	std::vector<Entity*> children;

	void addChild(Entity* child);
	void removeChild(Entity* child);

	// Methods that should be overwritten
	// by derived classes 
	virtual void render(Camera* camera);
	virtual void update(float delta_time);

	// Some useful methods
	Matrix44 getGlobalMatrix();
	float distance(Entity* e);
};
