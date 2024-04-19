#include "entity_mesh.h"

#include "framework/camera.h"
#include "graphics/mesh.h"
#include "graphics/shader.h"



void EntityMesh::render() {

	// Get the last camera that was activated 
	Camera* camera = Camera::current;

	// Enable shader and pass uniforms 
	shader->enable();
	shader->setUniform("u_model", model);
	shader->setUniform("u_viewproj", camera->viewprojection_matrix);
	shader->setTexture("u_texture", texture, 0); //0 ja que només tenim un slot de moment


	// Render the mesh using the shader
	mesh->render(GL_TRIANGLES);

	// Disable shader after finishing rendering
	shader->disable();
};
