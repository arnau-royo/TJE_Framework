#include "world.h"
#include "framework/camera.h"
#include "framework/input.h"
#include "framework/entities/entity_mesh.h"
#include "graphics/texture.h"
#include "graphics/shader.h"
#include "game/game.h"
#include "stages/stage.h"
#include "entities/entity_player.h"

#include <algorithm>
#include <fstream>

World* World::instance = nullptr;

World::World()
{
	int window_width = Game::instance->window_width;
	int window_height = Game::instance->window_height;

	// Create our 3D camera
	camera = new Camera();
	camera->lookAt(Vector3(0.f, 20.f, -20.f), Vector3(0.f, 0.f, 0.f), Vector3::UP); //position the camera and point to 0,0,0
	camera->setPerspective(70.f, window_width / (float)window_height, 0.01f, 1000.f); //set the projection, we want to be perspective

	Material player_material;
	player_material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");

	//player = new EntityPlayer(Mesh::Get("data/scene/ambulance.002.obj"), player_material, "player"); //TODO: canviar el player

	Mesh* map_mesh = Mesh::Get("data/meshes/box.ASE"); //TODO: canviar la mesh 
	Material map_mat;
	map_mat.diffuse = Texture::Get("data/textures/texture.tga"); //TODO: canviar la textura per la adecuada


	EntityMesh* map = new EntityMesh(map_mesh, map_mat);

	root.addChild(map);

	//parseScene("data/myscene.scene", &root);
}

void World::render() {
	//Set the camera as default
	camera->enable();

	// Set the camera as default
	camera->enable();

	// Set flags
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	if (!free_camera) {
		Matrix44 mYaw;
		mYaw.setRotation(camera_yaw, Vector3(0, 1, 0));

		Matrix44 mPitch;
		mPitch.setRotation(camera_pitch, Vector3(-1, 0, 0));

		Vector3 front = (mPitch * mYaw).frontVector();
		Vector3 eye;
		Vector3 center;
		 
	}


}