#include "entity_mesh.h"

#include "framework/camera.h"
#include "graphics/mesh.h"
#include "graphics/shader.h"



EntityMesh::EntityMesh(Mesh* mesh, const Material& material, const std::string& name)
{
	this->mesh = mesh;
	this->name = name;
	this->material = material;
}

EntityMesh::~EntityMesh()
{

}

void EntityMesh::render(Camera* camera) {

	if (!mesh) return;

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	if (!material.shader) {
		material.shader = Shader::Get(isInstanced ? "data/shaders/instanced.vs" : "data/shaders/basic.vs", "data/shaders/flat.fs");
	}

	//Enable shader
	material.shader->enable();

	//Upload uniforms 
	material.shader->setUniform("u_color", material.color);
	material.shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	//shader->setTexture("u_texture", texture, 0); //0 ja que només tenim un slot de moment

	if (material.diffuse) {
		material.shader->setUniform("u_texture", material.diffuse, 0);
	}

	if (!isInstanced) {
		material.shader->setUniform("u_model", getGlobalMatrix());
	}

	if (isInstanced) {
		mesh->renderInstanced(GL_TRIANGLES, models.data(), models.size());
	} else{
		mesh->render(GL_TRIANGLES);
	}

	// Render the mesh using the shader
	//mesh->render(GL_TRIANGLES);

	// Disable shader after finishing rendering
	material.shader->disable();

	Entity::render(camera);
};

//a l'update fer igual que al render i cridar la funció del parse scene (aula global)
void EntityMesh::update(float seconds_elapsed) {
	Entity::update(seconds_elapsed);
}

void EntityMesh::addInstance(const Matrix44& model)
{
	models.push_back(model);
}

