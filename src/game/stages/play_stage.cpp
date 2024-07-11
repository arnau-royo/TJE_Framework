#include "play_stage.h"

#include "framework/camera.h"
#include "framework/input.h"
#include "framework/entities/entity_collider.h"

#include "game/entities/entity_player.h"

#include "game/game.h"
#include "game/world.h"

//PLAY STAGE
PlayStage::PlayStage()
{
	/*
	//Create enemy1
	(World::get_instance()->enemy) = new EntityEnemy(Mesh::Get("data/meshes/player/zombie_anim.MESH"), "zombie_1");
	(World::get_instance()->enemy)->setLayer(eCollisionFilter::ENEMY);
	(World::get_instance()->enemy)->isAnimated = true;

	(World::get_instance()->enemy)->model.setTranslation(-2.f, -0.01f, -2.2f); //TODO borrar o modificar a on es vol que spawnwgi l'enemic

	(World::get_instance()->root).addChild((World::get_instance()->enemy));

	//(World::get_instance()->parseScene("data/myscene_temp.scene", &(World::get_instance()->root)));

*/

	//Parse scene tutorial
	(World::get_instance()->parseScene("data/myscene2.scene", &(World::get_instance()->root)));
	

	//Creating the player
	Mesh* player_mesh = Mesh::Get("data/meshes/player/player_no_head_anim.MESH");

	Material player_material;

	player_material.diffuse = Texture::Get("data/textures/player/player.png");
	player_material.shader = Shader::Get("data/shaders/skinning.vs", "data/shaders/texture.fs");
	//player_material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");

	(World::get_instance()->player) = new EntityPlayer(player_mesh, player_material, "player");
	(World::get_instance()->player)->setLayer(1 | 2);
	(World::get_instance()->player)->isAnimated = true;
}

void PlayStage::onEnter(Stage* previousStage)
{
	// lock mouse
	SDL_ShowCursor(false);
	SDL_SetRelativeMouseMode(SDL_TRUE);
	Game::instance->mouse_locked = true;
}

void PlayStage::render()
{
	World::get_instance()->render();

	//ui
}

void PlayStage::update(float seconds_elapsed)
{
	
	if ((World::get_instance()->player)->healthbar == 0.0) {
		StageManager::get_instance()->goTo("defeatStage");
	}
	
	World::get_instance()->update(seconds_elapsed);

	Camera* camera = World::get_instance()->camera;

	//Get Ray direction

	Vector2 mouse_pos = Input::mouse_position;
	Vector3 ray_origin = camera->eye;

	Vector3 ray_direction = camera->getRayDirection(mouse_pos.x, mouse_pos.y, Game::instance->window_width, Game::instance->window_height);

	//Fill Collision vector

	std::vector<Vector3> collisions;

	for (Entity* e : World::get_instance()->root.children) {

		/*
			EntityCollider* collider = dynamic_cast<EntityCollider*>(e);
		if (!collider) {
			continue;
		}


		Vector3 col_point;
		Vector3 col_normal;

		if (collider->mesh->testRayCollision(collider->model, ray_origin, ray_direction, col_point, col_normal)); {
			collisions.push_back(col_point);
		}
		*/
	}

	//Get entities

	for (auto& col_point : collisions) {
		Mesh* mesh = Mesh::Get("");
		EntityMesh* new_entity = new EntityMesh(mesh, {});
		new_entity->model.setTranslation(col_point);
		World::get_instance()->addEntity(new_entity);
	}

}
