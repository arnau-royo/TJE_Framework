#include "graphics/mesh.h"
#include "entity.h"

class EntityMesh : public Entity {

public:

	// Attributes of the derived class  
	Mesh* mesh = nullptr;
	Texture* texture = nullptr;
	Shader* shader = nullptr;
	Vector4 color;

	// Methods overwritten from base class
	void render();
	void update(float elapsed_time);
};
