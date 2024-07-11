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
	int width = Game::instance->window_width;
	int height = Game::instance->window_height;
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