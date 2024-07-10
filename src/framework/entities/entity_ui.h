#pragma once

#include "entity_mesh.h"

enum eButtonId {
	UndefinedButton,
	PlayButton,
	OptionsButton,
	ExitButton,
	// ...
};

class EntityUI : public EntityMesh {

public:

	EntityUI(Vector2 size, const Material& material);
	EntityUI(Vector2 pos, Vector2 size, const Material& material, eButtonId button_id = UndefinedButton, const std::string& name = "");
	~EntityUI() {};

	Vector2 position;
	Vector2 size;

	bool visible = true;

	float mask = 1.0f;

	//3d hud
	bool is3D = false;
	Vector3 pos3d;

	eButtonId button_id = UndefinedButton;

	//Methods overwritten from base class
	void render(Camera* camera);
	void update(float seconds_elapsed);
	void update3D(Vector3 position3d);


};
