#include "Renderer.h"

Renderer::Renderer(Window& parent) : OGLRenderer(parent)
{
	heightMap = new HeightMap(TEXTUREDIR "noise.png");
	camera = new Camera(-40, 270, Vector3(-2100, 3300, 2000), 300);

	shader = new Shader("TexturedVertex(3).glsl", "TexturedFragment(3).glsl");

	Vector3 dimensions = heightMap->GetHeightmapSize();
	camera->SetPosition(dimensions * Vector3(0.5, 2, 0.5));

	if (!shader->LoadSuccess())
	{
		return;
	}

	terrainTex = SOIL_load_OGL_texture(TEXTUREDIR "g_tex.jpg",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	if (!terrainTex)
	{
		return;
	}

	SetTextureRepeating(terrainTex, true);

	projMatrix = Matrix4::Perspective(1.0f, 10000.0f,
		(float)width / (float)height, 45.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	init = true;
}
Renderer ::~Renderer(void)
{
	delete heightMap;
	delete camera;
	delete shader;
}

void Renderer::UpdateScene(float dt)
{
	camera->UpdateCamera(dt);
	viewMatrix = camera->BuildViewMatrix();
}
void Renderer::RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	BindShader(shader);
	UpdateShaderMatrices();

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);

	//gluniform1f(glGetUniformLocation(currentShader->GetProgram(),))

	glUniform4fv(glGetUniformLocation(currentShader->GetProgram(),
		"fogcolor"), 1, (float*)&Vector4(1,0, 0, 1));

	glUniform1f(glGetUniformLocation(currentShader->GetProgram(),
		"linearStart"), 500.0f);

	glUniform1f(glGetUniformLocation(currentShader->GetProgram(),
		"linearEnd"), 5000.0f);

	glUniform1f(glGetUniformLocation(currentShader->GetProgram(),
		"density"), 00.0002f);

	curTime = time(0);

	glUniform1f(glGetUniformLocation(currentShader->GetProgram(),
		"time"), curTime);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, terrainTex);
	heightMap->Draw();
}