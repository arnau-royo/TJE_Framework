#include "entity_mesh.h"

#include "framework/camera.h"
#include "graphics/mesh.h"
#include "graphics/shader.h"


EntityMesh::EntityMesh()
{
}

EntityMesh::EntityMesh(Mesh* mesh, const Material& material, const std::string& name)
{
	this->mesh = mesh;
	this->material = material;
	this->name = name;
	
}

EntityMesh::~EntityMesh()
{
}



void EntityMesh::render(Camera* camera) {

	if (!mesh) return;

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	if (!material.shader) {

		std::string vs;
		std::string fs;

		if (isInstanced)
		{
			vs = "data/shaders/instanced.vs";
		}
		else
		{
			vs = "data/shaders/basic.vs";
		}

		if (material.diffuse || mesh->has_texture()) {
			fs = "data/shaders/texture.fs";
		}
		else {
			fs = "data/shaders/flat.fs";
		}

		material.shader = Shader::Get(vs.c_str(), fs.c_str());
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
		/*
		if (isAnimated){
			mesh->renderAnimated(GL_TRIANGLES, &animator.getCurrentSkeleton());
		}
		else {
			mesh->render(GL_TRIANGLES);
		}
		*/
		mesh->render(GL_TRIANGLES);
	}

	// Render the mesh using the shader
	//mesh->render(GL_TRIANGLES);

	// Disable shader after finishing rendering
	material.shader->disable();

	Entity::render(camera);
};

void EntityMesh::update(float seconds_elapsed) {
	
	if (isAnimated) {
		animator.update(seconds_elapsed); //Si es animat afegim un animator
	}

	Entity::update(seconds_elapsed);
}

void EntityMesh::addInstance(const Matrix44& model)
{
	models.push_back(model);
}

