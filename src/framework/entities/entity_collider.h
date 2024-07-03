#pragma once

#include "entity_mesh.h"

class EntityCollider;

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


class EntityCollider : public EntityMesh {


	void getCollisionsWithModel(const Matrix44 model, const Vector3& center, std::vector<sCollisionData>& collisions, std::vector<sCollisionData>& ground_collisions, eCollisionFilter filter);

	int layer = eCollisionFilter::ALL;

public:

	bool is_static = true;

	EntityCollider();
	EntityCollider(Mesh* mesh, const Material& material, const std::string& name = "") :
		EntityMesh(mesh, material, name) {};
	~EntityCollider() {};

	void getCollisions(const Vector3& target_position, std::vector<sCollisionData>& collisions, std::vector<sCollisionData>& ground_collisions, eCollisionFilter filter);


	int getLayer() { return layer; }
	void setLayer(int new_layer) { layer = new_layer; }
};

