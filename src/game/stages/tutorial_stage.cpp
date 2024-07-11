#include "tutorial_stage.h"

#include "framework/camera.h"
#include "framework/input.h"
#include "framework/entities/entity_collider.h"

#include "game/entities/entity_player.h"

#include "play_stage.h"

#include "game/game.h"
#include "game/world.h"

//TUTORIAL STAGE
TutorialStage::TutorialStage()
{
	//Creating the player
	Mesh* player_mesh = Mesh::Get("data/meshes/player/player_no_head_anim.MESH");

	Material player_material;

	player_material.diffuse = Texture::Get("data/textures/player/player.png");
	player_material.shader = Shader::Get("data/shaders/skinning.vs", "data/shaders/texture.fs");
	//player_material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");

	(World::get_instance()->player) = new EntityPlayer(player_mesh, player_material, "player");
	(World::get_instance()->player)->setLayer(1 | 2);
	(World::get_instance()->player)->isAnimated = true;

	//Create enemy1
	(World::get_instance()->enemy) = new EntityEnemy(Mesh::Get("data/meshes/player/zombie_anim.MESH"), "zombie_1");
	(World::get_instance()->enemy)->setLayer(eCollisionFilter::ENEMY);
	(World::get_instance()->enemy)->isAnimated = true;

	(World::get_instance()->enemy)->model.setTranslation(0.f, -0.01f, -2.2f); //TODO borrar o modificar a on es vol que spawnwgi l'enemic

	(World::get_instance()->root).addChild((World::get_instance()->enemy));

	//Create enemy2
	(World::get_instance()->enemy2) = new EntityEnemy(Mesh::Get("data/meshes/player/zombie_anim.MESH"), "zombie_2");
	(World::get_instance()->enemy2)->setLayer(eCollisionFilter::ENEMY);
	(World::get_instance()->enemy2)->isAnimated = true;

	(World::get_instance()->enemy2)->model.setTranslation(-2.f, -0.01f, -2.2f); //TODO borrar o modificar a on es vol que spawnwgi l'enemic

	(World::get_instance()->root).addChild((World::get_instance()->enemy2));

	//Parse scene tutorial
	(World::get_instance()->parseScene("data/myscene_temp.scene", &(World::get_instance()->root)));
}

void TutorialStage::onEnter(Stage* previousStage)
{
	// lock mouse
	SDL_ShowCursor(false);
	SDL_SetRelativeMouseMode(SDL_TRUE);
	Game::instance->mouse_locked = true;
}

void TutorialStage::render()
{
	World::get_instance()->render();

	//ui
	drawText(50, 80, "THIS IS THE TUTORIAL STAGE", Vector3(0, 128, 0), 3);
	drawText(50, 110, "TO PASS THE STAGE KILL ALL ZOMBIES", Vector3(139, 0, 0), 3);
	drawText(50, 140, "TO MOVE, USE THE AWSD KEYS AND THE MOUSE", Vector3(1, 1, 1), 2);
	drawText(50, 165, "TO ATTACK (PUNCH) USE THE V KEY", Vector3(1, 1, 1), 2);
	drawText(50, 190, "WHEN YOU PUNCH AN ENEMY YOUR HELATH INCREASES", Vector3(1, 1, 1), 2);
	drawText(50, 215, "TO JUMP USE THE SPACE KEY", Vector3(1, 1, 1), 2);
	drawText(50, 240, "TO SPRINT USE THE SHIFT KEY", Vector3(1, 1, 1), 2);

}

void TutorialStage::update(float seconds_elapsed)
{

	if (((World::get_instance()->enemy)->healthbar == 0.0) && ((World::get_instance()->enemy2)->healthbar == 0.0)) {
		(World::get_instance()->clearScene()); //Delete the actual scene

		World::get_instance()->removeEntity(World::get_instance()->player);

		PlayStage* playstage = new PlayStage(); //When I have to change the stage I create the playstage
		StageManager::get_instance()->stages["playStage"] = playstage;
		StageManager::get_instance()->goTo("playStage");
	}

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
