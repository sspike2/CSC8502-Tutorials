# pragma once

# include "../nclgl/OGLRenderer.h"

class Renderer : public OGLRenderer
{
public:
	Renderer(Window& parent);
	virtual ~Renderer(void);

	virtual void RenderScene();

	void UpdateTextureMatrix(float rotation);
	void UpdateTextureMatrixTiling(float value);
	void ToggleRepeating();
	void ToggleFiltering();
	void ToggleBlendMode();

protected:
	Shader* shader;
	Mesh* triangle;
	GLuint texture;
	GLuint bump;
	GLuint AlphaTex;

	bool filtering;
	bool repeating;
	int blendMode;


};