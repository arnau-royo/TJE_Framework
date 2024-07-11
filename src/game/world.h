#pragma once
#include "framework/entities/entity.h"
#include "framework/utils.h"

#include "framework/entities/entity_collider.h"

#include "framework/camera.h"
#include "framework/input.h"

#include "graphics/texture.h"
#include "graphics/shader.h"

#include "game/game.h"
#include "stages/stage.h"

#include <algorithm>
#include <fstream>

class Camera;

class EntityPlayer;

class EntityEnemy;



class World {

	static World* instance;

	public:

		static World* get_instance() {
			if (instance != nullptr) {
				return instance;
			}
			instance = new World();
			return instance;
		}

		World();

		Entity root;

		EntityMesh* skybox = nullptr;

		EntityPlayer* player = nullptr;

		EntityEnemy* enemy = nullptr;

		

		Camera* camera = nullptr;
		Camera* camera2D = nullptr;


		std::vector<Vector3> waypoints;

		float camera_yaw = 0.f;
		float camera_pitch = 0.f;
		float camera_speed = 2.0f;
		float mouse_speed = 0.5f;

		bool free_camera = false;

		void render();
		void update(float seconds_elapsed);

		float sphere_radius = .05f;
		float sphere_ground_radius = .05f;
		float player_height = 0.14f;

		//Scene

		std::vector<Entity*> entities_to_destroy;

		bool parseScene(const char* filename, Entity* root);
		void addEntity(Entity* entity);
		void removeEntity(Entity* entity);


		//collisions 
		void getCollisions(const Vector3& target_position, std::vector<sCollisionData>& collisions, std::vector<sCollisionData>& ground_collisions, eCollisionFilter filter);

		sCollisionData raycast(const Vector3& origin, const Vector3& direction, int layer, float max_ray_dist);

		void get_enemy_collisions(const Vector3& position, std::vector<sCollisionData>& collisions);


		/*
		//AI
		std::vector<Vector2> waypoints;
		void addWayPointFromScreenPos(const Vector2& coord);
		*/

};
