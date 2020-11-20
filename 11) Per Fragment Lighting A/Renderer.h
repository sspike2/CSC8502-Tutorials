# pragma once

#include "../nclgl/OGLRenderer.h"
#include "../nclgl/HeightMap.h"
#include "../nclgl/Camera.h"
#include "../nclgl/Light.h"

//class HeightMap;
//class Camera;
//class Light; // Predeclare our new class type ...
//class Shader;

class Renderer : public OGLRenderer
{
public:
	Renderer(Window& parent);
	~Renderer(void);

	void RenderScene() override;
	void UpdateScene(float dt) override;

protected:
	HeightMap* heightMap;
	Shader* shader;
	Camera* camera;
	Light* light; //A new thing !
	GLuint texture;
};