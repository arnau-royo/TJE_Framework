#ifndef ENTITY_COLLIDER_H
#define ENTITY_COLLIDER_H

#include "entity_mesh.h"
#include "game/world.h"


class EntityCollider : public EntityMesh {

public:
    bool is_static = true;

    int layer = eCollisionFilter::ALL;

    EntityCollider();
    EntityCollider(Mesh* mesh, const Material& material, const std::string& name = "") :
        EntityMesh(mesh, material, name) {};
    ~EntityCollider() {};

    void getCollisionsWithModel(const Matrix44 model, const Vector3& center, std::vector<sCollisionData>& collisions, std::vector<sCollisionData>& ground_collisions, eCollisionFilter filter);
    void getCollisions(const Vector3& target_position, std::vector<sCollisionData>& collisions, std::vector<sCollisionData>& ground_collisions, eCollisionFilter filter);

    int getLayer() const { return layer; }
    void setLayer(int new_layer) { layer = new_layer; }
};
#endif //ENTITY_COLLIDER_H
