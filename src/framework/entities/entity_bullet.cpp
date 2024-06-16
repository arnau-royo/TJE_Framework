#pragma once

#include "entity_bullet.h"
#include "graphics/material.h"

EntityBullet::EntityBullet(Mesh* mesh, const Material& mat, const std::string& name) {
	
	EntityMesh(mesh, mat, name);

}

void EntityBullet::render(Camera* camera)
{
}

void EntityBullet::update(float seconds_elapsed)
{
}

void EntityBullet::ithits()
{
	//EntityCollider::getCollisions();
}
