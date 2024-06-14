#pragma once

#include "entity_gun.h"
#include "framework/input.h"


EntityGun::EntityGun(Mesh* mesh, Material* mat, const std::string& name = "") {
	
	this->mesh = mesh;
	this->name = name;

	Material* blaster_mat;
	blaster_mat->shader = Shader::Get("data/shaders/basic.vs", "data/shaders/flat.fs");
	//blaster_mat.diffuse = new Texture();
	//blaster_mat.diffuse->load("data/meshes/blaster/blasterG.mtl");

	this->material = blaster_mat;
	EntityMesh(mesh, blaster_mat, name);
}

void EntityGun::fire(Vector3 origin, Vector3 direction)
{
	if (ammo > 0) {
		//render projectil
		 if (Input::wasButtonPressed(SDL_BUTTON_LEFT)) {
			 ammo -= 1;
		 }
		 else if (Input::wasButtonPressed(SDL_BUTTON_RIGHT)) {
			 ammo -= 5;
		 }
	}	
}

