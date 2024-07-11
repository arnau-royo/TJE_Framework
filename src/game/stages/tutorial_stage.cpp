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

	(World::get_instance()->enemy) = new EntityEnemy(Mesh::Get("data/meshes/player/zombie_anim.MESH"), "zombie_1");
	(World::get_instance()->enemy)->setLayer(eCollisionFilter::ENEMY);
	(World::get_instance()->enemy)->isAnimated = true;

	(World::get_instance()->enemy)->model.setTranslation(-2.f, -0.01f, -2.2f); //TODO borrar o modificar a on es vol que spawnwgi l'enemic

	(World::get_instance()->root).addChild((World::get_instance()->enemy));

	(World::get_instance()->parseScene("data/myscene_temp.scene", &(World::get_instance()->root)));

	//(World::get_instance()->parseScene("data/myscene2.scene", &(World::get_instance()->root)));
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
}

void TutorialStage::update(float seconds_elapsed)
{

	if ((World::get_instance()->enemy)->healthbar == 0.0) {
		(World::get_instance()->clearScene()); //Delete the actual scene

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