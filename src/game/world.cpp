#include "world.h"

#include "entities/entity_player.h"


//Crating a world instance
World* World::instance = nullptr;

World::World()
{
	int window_width = Game::instance->window_width;
	int window_height = Game::instance->window_height;

	// Create our 3D camera
	camera = new Camera();
	camera->lookAt(Vector3(0.f, 20.f, -20.f), Vector3(0.f, 0.f, 0.f), Vector3::UP); //position the camera and point to 0,0,0
	camera->setPerspective(70.f, window_width / (float)window_height, 0.01f, 1000.f); //set the projection, we want to be perspective

	//camera for UI
	camera2D = new Camera();
	camera2D->view_matrix.setIdentity();
	camera2D->setOrthographic(0, window_width, window_height, 0, -1.f, 1.f); //El 0 està a dalt

	//Creating the player
	Mesh* player_mesh = Mesh::Get("data/meshes/player/player_no_head_anim.MESH");
	
	Material player_material;

	player_material.diffuse = Texture::Get("data/textures/player/player.png");
	player_material.shader = Shader::Get("data/shaders/skinning.vs", "data/shaders/texture.fs");
	//player_material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	
	player = new EntityPlayer(player_mesh, player_material, "player");
	player->setLayer(1 | 2);
	player->isAnimated = true;


	enemy = new EntityEnemy(Mesh::Get("data/meshes/player/zombie_anim.MESH"), "zombie_1");
	enemy->setLayer(eCollisionFilter::ENEMY);
	enemy->isAnimated = true;

	enemy->model.setTranslation(-2.f, -0.01f, -2.2f); //TODO borrar o modificar a on es vol que spawnwgi l'enemic

	root.addChild(enemy);

	
	//Skybox
	Material landscape_cubemap;
	landscape_cubemap.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/cubemap.fs");
	landscape_cubemap.diffuse = new Texture();
	landscape_cubemap.diffuse->loadCubemap("landscape", {
		"data/textures/skybox/px.png",
		"data/textures/skybox/nx.png" ,
		"data/textures/skybox/ny.png" ,
		"data/textures/skybox/py.png" ,
		"data/textures/skybox/pz.png" ,
		"data/textures/skybox/nz.png" });

	skybox = new EntityMesh(Mesh::Get("data/meshes/cubemap.ASE"), landscape_cubemap, "landscape");

	//parseScene("data/myscene_temp.scene", &root);
	parseScene("data/myscene2.scene", &root);
}

void World::render() {
	//Set the camera as default
	camera->enable();

	//Render skybox
	glDisable(GL_DEPTH_TEST);
	skybox->render(camera);
	glEnable(GL_DEPTH_TEST);

	// Draw the floor grid
	drawGrid();

	// Set flags
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	//Render entity player
	player->render(camera);

	//Render entity enemy TODO
	//enemy->render(camera);

	//Render all scene tree
	root.render(camera);
}

void World::update(float seconds_elapsed)
{
	if (free_camera || Input::isKeyPressed(SDL_SCANCODE_F))
	{
		float speed = seconds_elapsed * camera_speed;

		// Mouse input to rotate the cam
		if (Input::isMousePressed(SDL_BUTTON_LEFT) || Game::instance->mouse_locked) //is left button pressed?
		{
			camera->rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f, -1.0f, 0.0f));
			camera->rotate(Input::mouse_delta.y * 0.005f, camera->getLocalVector(Vector3(-1.0f, 0.0f, 0.0f)));
		}

		// Async input to move the camera around
		if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) speed *= 10; //move faster with left shift
		if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) camera->move(Vector3(0.0f, 0.0f, 1.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) camera->move(Vector3(0.0f, 0.0f, -1.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) camera->move(Vector3(1.0f, 0.0f, 0.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) camera->move(Vector3(-1.0f, 0.0f, 0.0f) * speed);
	}
	else
	{
		//Update our scene
		root.update(seconds_elapsed);

		//and the player
		player->update(seconds_elapsed);

		//Get mouse deltas
		camera_yaw -= Input::mouse_delta.x * seconds_elapsed * mouse_speed;
		camera_pitch -= Input::mouse_delta.y * seconds_elapsed * mouse_speed;

		//Restrict pitch angle
		camera_pitch = clamp(camera_pitch, -M_PI * 0.4f, M_PI * 0.4f);

		Matrix44 mYaw;
		mYaw.setRotation(camera_yaw, Vector3(0, 1, 0));

		Matrix44 mPitch;
		mPitch.setRotation(camera_pitch, Vector3(-1, 0, 0));

		Vector3 front = (mPitch * mYaw).frontVector().normalize();
		Vector3 eye;
		Vector3 center;

		//A l'eye l'he tirat de moment endavant i a dalt, però estaria bé que no tingues cap el player
		eye = player->model.getTranslation() + Vector3(0.0f, 0.17f, 0.0f) + front * -0.01f; //0.2f enlloc de 0.1f per donar-li una mica més d'alçada al eye del player Alçada de la camara
		center = eye + front;

		//Ray-mesh intersection to move the camera up to the collision point

		Vector3 dir = eye - center;

		sCollisionData data = World::get_instance()->raycast(center, dir, eCollisionFilter::ALL, dir.length());

		if (data.collided) {
			eye = data.colPoint;
		}

		camera->lookAt(eye, center, Vector3(0, 1, 0));

	}

	// Move Skybox to camera position
	skybox->model.setTranslation(camera->eye);

	//Delete pending entities
	for (auto e : entities_to_destroy) {
		root.removeChild(e);
		delete e;
	}

	entities_to_destroy.clear();
}

bool World::parseScene(const char* filename, Entity* root)
{
	std::cout << " + Scene loading: " << filename << "..." << std::endl;

	std::ifstream file(filename);

	if (!file.good()) {
		std::cerr << "Scene [ERROR]" << " File not found!" << std::endl;
		return false;
	}

	std::map < std::string, sRenderData> meshes_to_load;

	std::string scene_info, mesh_name, model_data;
	file >> scene_info; file >> scene_info;
	int mesh_count = 0;

	// Read file line by line and store mesh path and model info in separated variables
	while (file >> mesh_name >> model_data)
	{
		if (mesh_name[0] == '#')
			continue;

		// Get all 16 matrix floats
		std::vector<std::string> tokens = tokenize(model_data, ",");

		// Fill matrix converting chars to floats
		Matrix44 model;
		for (int t = 0; t < tokens.size(); ++t) {
			model.m[t] = (float)atof(tokens[t].c_str());
		}

		// Add model to mesh list (might be instanced!)
		sRenderData& render_data = meshes_to_load[mesh_name];
		render_data.models.push_back(model);
		mesh_count++;
	}

	// Iterate through meshes loaded and create corresponding entities
	for (auto data : meshes_to_load) {

		mesh_name = "data/" + data.first;
		sRenderData& render_data = data.second;

		// No transforms, nothing to do here
		if (render_data.models.empty())
			continue;


		Material mat = render_data.material;
		
		EntityMesh* new_entity = nullptr;
		EntityMesh* new_entity2 = nullptr;

		size_t enemy_tag = data.first.find("@enemy");
		size_t player_tag = data.first.find("@player");
		size_t enemy_waypoint_tag = data.first.find("@waypoint");

		if (player_tag != std::string::npos) {
			assert(player);
			player->model.setTranslation(render_data.models[0].getTranslation());
		}
		else if (enemy_tag != std::string::npos) {
			Mesh* mesh = Mesh::Get("data/meshes/player/player.obj");
			new_entity2 = new EntityEnemy(mesh, "zombie_1");
			assert(new_entity2);
			new_entity2->model.setTranslation(render_data.models[0].getTranslation());
		}
		else if (enemy_waypoint_tag != std::string::npos){
			waypoints.push_back(render_data.models[0].getTranslation());
			continue;

		}
		else {
			Mesh* mesh = Mesh::Get(mesh_name.c_str());

			//new_entity = new EntityMesh(mesh, mat);
			new_entity = new EntityCollider(mesh, mat); //Ha de ser un entity collider per aplicar les collisions calculades a l'objecte Entity Collider
		}

		if (!new_entity) {
			continue;
		}

		new_entity->name = data.first;

		// Create instanced entity
		if (render_data.models.size() > 1) {
			new_entity->isInstanced = true;
			new_entity->models = render_data.models; // Add all instances
		}
		// Create normal entity
		else {
			new_entity->model = render_data.models[0];
		}

		// Add entity to scene root
		root->addChild(new_entity);
	}

	std::cout << "Scene [OK]" << " Meshes added: " << mesh_count << std::endl;
	return true;
}

void World::addEntity(Entity* entity)
{
	root.addChild(entity);
}

void World::removeEntity(Entity* entity)
{
	entities_to_destroy.push_back(entity);
}

void World::getCollisions(const Vector3& target_position, std::vector<sCollisionData>& collisions, std::vector<sCollisionData>& ground_collisions, eCollisionFilter filter)
{
	for (auto e : root.children)
	{
		EntityCollider* ec = dynamic_cast<EntityCollider*>(e);
		if (ec == nullptr) {
			continue;
		}
		ec->getCollisions(target_position, collisions, ground_collisions, filter);
	}
}

void World::get_enemy_collisions(const Vector3& position, std::vector<sCollisionData>& collisions)
{
	for (auto e : root.children)
	{
		EntityEnemy* ec = dynamic_cast<EntityEnemy*>(e);
		if (ec == nullptr) {
			continue;
		}

		Vector3 collision_point;
		Vector3 collision_normal;

		if (ec->mesh->testSphereCollision(ec->model, position, 0.1, collision_point, collision_normal)) {
			collisions.push_back({ collision_point, collision_normal.normalize(), position.distance(collision_point), true, ec });
		}
	}
}



sCollisionData World::raycast(const Vector3& origin, const Vector3& direction, int layer, float max_ray_dist) {
	sCollisionData data;

	for (auto e : root.children) {
		EntityCollider* ec = dynamic_cast<EntityCollider*>(e);
		if (ec == nullptr || !(ec->getLayer() & layer)) {
			continue;
		}


		Vector3 col_point;
		Vector3 col_normal;
		
		if (!ec->mesh->testRayCollision(ec->model, origin, direction, col_point, col_normal, max_ray_dist)); {
			continue;
		}
		

		data.collided = true;

		//There are collisions? Update if nearest

		float non_distance = (col_point - origin).length();
		if (non_distance < data.distance) {
			data.colPoint = col_point;
			data.colNormal = col_normal;
			data.distance = non_distance;
			data.collider = ec;
		}
	}
	return data;
}


/*
void World::addWayPointFromScreenPos(const Vector2& coord)
{
	Camera* camera = Camera::current;

	//Get ray direction
	Vector3 ray_origin = camera->eye;
	Vector3 ray_direction = camera->getRayDirection(Input::mouse_position.x, Input::mouse_position.y, Game::instance->window_width, Game::instance->window_height);

	//Collision data
	sCollisionData data = World::get_instance()->raycast(ray_origin, ray_direction);

	//Push new waypoint
	if (data.collided) {
		waypoints.push_back(data.colPoint);
	}

*/
