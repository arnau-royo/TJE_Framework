#pragma once

#include "entity.h"
#include "framework/animation.h"
#include "graphics/material.h"

class EntityMesh : public Entity {

public:

	// Attributes of the derived class  
	Mesh* mesh = nullptr;
	Texture* texture = nullptr;
	Shader* shader = nullptr;
	Material material;
	Vector4 color;

	Animator animator;

	EntityMesh();
	EntityMesh(Mesh* mesh, const Material& material, const std::string& name = "");
	~EntityMesh();


	std::string last_drop = "heal";

	bool isAnimated = false;

	//Instancing
	bool isInstanced = false;
	std::vector<Matrix44> models;

	//Methods overwritten from base class
	void render(Camera* camera);
	void update(float delta_time);

	void addInstance(const Matrix44& model);
};
