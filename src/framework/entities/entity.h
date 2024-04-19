#pragma once

#include "framework/includes.h"
#include "framework/framework.h"

class Camera;

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
	virtual void render(Camera* camera) override;
	virtual void update(float delta_time) override;

	// Some useful methods
	Matrix44 getGlobalMatrix();
	float distance(Entity* e);
};
