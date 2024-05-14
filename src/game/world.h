#pragma once
#include "framework/entities/entity.h"
#include "framework/utils.h"
#include "framework/entities/entity_mesh.h"

class Camera;
class EntityPlayer;

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

		EntityPlayer* player = nullptr;

		Camera* camera = nullptr;

		float camera_yaw = 0.f;
		float camera_pitch = 0.f;
		float camera_speed = 2.0f;
		float mouse_speed = 0.5f;

		bool free_camera = true;

		void render();
		void update(float seconds_elapsed);

		//Scene

		std::map<std::string, sRenderData> meshes_to_load;

		std::vector<Entity*> entities_to_destroy;

		bool parseScene(const char* filename, Entity* root);
		void addEntity(Entity* entity);
		void removeEntity(Entity* entity);

};