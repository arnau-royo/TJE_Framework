#pragma once

#include "entity.h"
#include "framework/animation.h"
#include "graphics/material.h"


struct sCollisionData {
	Vector3 colPoint;
	Vector3 colNormal;
	float distance = 1e10f;
	bool collided = false;
	//EntityCollider* collider = nullptr;
};

enum eCollisionFilter {
	NONE = 0,
	FLOOR = 1 << 0,
	WALL = 1 << 1,
	PLAYER = 1 << 2,
	ENEMY = 1 << 3,
	SCENARIO = FLOOR | WALL,
	ALL = 0xFF
};

class EntityMesh : public Entity {

public:

	// Attributes of the derived class  
	Mesh* mesh = nullptr;
	Texture* texture = nullptr;
	Shader* shader = nullptr;
	Material* material = nullptr;
	Vector4 color;

	EntityMesh();
	EntityMesh(Mesh* mesh, Material* material, const std::string& name = "");
	~EntityMesh();

	std::string last_drop = "heal";

	//Instancing

	bool isInstanced = false;
	std::vector<Matrix44> models;

	//Methods overwritten from base class

	void render(Camera* camera);
	void update(float delta_time);

	void addInstance(const Matrix44& model);
};
