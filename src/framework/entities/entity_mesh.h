#pragma once

#include "entity.h"
#include "framework/animation.h"
#include "graphics/material.h"

class EntityMesh : public Entity {

public:
	EntityMesh() {};
	EntityMesh(Mesh* mesh, const Material& material, const std::string& name = "");
	~EntityMesh();

	// Attributes of the derived class 
	Mesh* mesh = nullptr;

	Material material;

	//Instancing
	bool isInstanced = false;
	std::vector<Matrix44> models;

	// Methods overwritten from base class
	void render(Camera* camera) ;
	void update(float delta_time);

	void addInstance(const Matrix44& model);
};
