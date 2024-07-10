#include "entity_ui.h"

#include "graphics/mesh.h"
#include "graphics/shader.h"
#include "framework/input.h"
#include "framework/camera.h"
#include "game/world.h"
#include "game/game.h"
#include "game/stages/stage.h"

EntityUI::EntityUI(Vector2 size, const Material& material)
{
	this->size = size;

	this->material = material;

	if (!this->material.shader) {
		this->material.shader = Shader::Get("data/shaders/basic.vs", material.diffuse ? "data/shaders/texture.fs" : "data/shaders/flat.fs");
	}
}

EntityUI::EntityUI(Vector2 pos, Vector2 size, const Material& material, eButtonId button_id, const std::string& name)
{
	position = pos;
	this->size = size;
	this->button_id = button_id;

	mesh = new Mesh();
	mesh->createQuad(pos.x, pos.y, size.x, size.y, true);

	this->material = material;

	if (!this->material.shader) {
		this->material.shader = Shader::Get("data/shaders/basic.vs", material.diffuse ? "data/shaders/texture.fs" : "data/shaders/flat.fs");
	}
}

void EntityUI::update(float seconds_elapsed)
{
	Vector2 mouse_pos = Input::mouse_position;

	if (button_id != UndefinedButton &&
		mouse_pos.x > (position.x - size.x * 0.5f) &&
		mouse_pos.x < (position.x + size.x * 0.5f) &&
		mouse_pos.y >(position.y - size.y * 0.5f) &&
		mouse_pos.y < (position.y + size.y * 0.5f)) {

		material.color = Vector4::GREEN;

		if (Input::isMousePressed(SDL_BUTTON_LEFT)) {
			StageManager::get_instance()->current->onButtonPressed(button_id);
		}
	}
	else {
		material.color = Vector4::WHITE;
	}

	Entity::update(seconds_elapsed);
}

void EntityUI::render(Camera* camera2d)
{
	if (!visible)
		return;

	if (!is3D) {
		glDisable(GL_DEPTH_TEST);
	}

	glDisable(GL_CULL_FACE);

	glEnable(GL_BLEND); //es pot comentar
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //es pot comentar

	material.shader->enable();

	World* world = World::get_instance();
	Matrix44 viewProj = camera2d->viewprojection_matrix;

	material.shader->setUniform("u_model", model);
	material.shader->setUniform("u_viewprojection", viewProj);
	material.shader->setUniform("u_color", material.color);
	material.shader->setUniform("u_mask", mask);
	
	if (material.diffuse) {
		material.shader->setUniform("u_texture", material.diffuse, 0);
	}

	if (is3D)
	{
		Vector2 _size = size;
		//float max_dist = 5.0f;
		//float dist = clamp(world->camera->eye.distance(pos3d), 0.01f, max_dist);
		//_size *= 1.f - dist / max_dist;

		Mesh quad;
		quad.createQuad(position.x, position.y, _size.x, _size.y, true);
		quad.render(GL_TRIANGLES);
	}
	else
	{
		mesh->render(GL_TRIANGLES);
	}

	material.shader->disable();

	glDisable(GL_BLEND); //Deixo les flags tal i com estaven abans
	glEnable(GL_DEPTH_TEST);

	Entity::render(camera2d);
}

/*
void EntityUI::update3D(Vector3 position3d) {
	pos3d = position3d;
}
*/

