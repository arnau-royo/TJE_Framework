#include "entity_player.h"
#include "graphics/mesh.h"
#include "graphics/texture.h"
#include "graphics/shader.h"

#include "framework/camera.h"
#include "framework/input.h"

#include "game/game.h"
#include "game/world.h"

EntityPlayer::EntityPlayer()
{
}

EntityPlayer::EntityPlayer(Mesh* mesh, const Material& material, const std::string& name)
{
	this->mesh = mesh;
	this->material = material;
	this->name = name;
}

void EntityPlayer::render(Camera* camera)
{
	//Render mesh
	EntityMesh::render(camera);
}

void EntityPlayer::update(float seconds_elapsed)
{
	float camera_yaw = World::get_instance()->camera_yaw;

	Matrix44 mYaw;
	mYaw.setRotation(camera_yaw, Vector3(0, 1, 0));

	Vector3 front = mYaw.frontVector(); //Vector3(0, 0, -1);
	Vector3 left = mYaw.rightVector(); //Vector3(1, 0, 0);

	Vector3 position = model.getTranslation();

	Vector3 move_dir;

	//moviment endavant/enrere

	if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP))
		move_dir += front;
	if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN))
		move_dir -= front;

	//moviement esquerra/dreta

	if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT))
		move_dir -= left;
	if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT))
		move_dir += left;

	float speed_mult = walk_speed;
	if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT))
		speed_mult *= 3.0f;

	move_dir.normalize();
	move_dir *= speed_mult;

	velocity += move_dir;

	//Update player's position
	position += velocity * seconds_elapsed;

	//Decreasing velocity when not moving
	velocity.x *= 0.5f;
	velocity.z *= 0.5f;

	model.setTranslation(position);
	model.rotate(camera_yaw, Vector3(0, 1, 0)); //S'ha de provar per veure que fa

	EntityMesh::update(seconds_elapsed);
}

