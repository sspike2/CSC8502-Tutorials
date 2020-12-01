#include "Renderer.h"

Renderer::Renderer(Window& parent) : OGLRenderer(parent)
{



	triangle = Mesh::LoadFromMeshFile("cube.msh");


	texture = SOIL_load_OGL_texture(TEXTUREDIR "/Sush/ground_asphalt_synth_11.png",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);

	bump = SOIL_load_OGL_texture(TEXTUREDIR "/Sush/ground_asphalt_synth_11_Bump.png",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);

	AlphaTex = SOIL_load_OGL_texture(TEXTUREDIR "/Sush/ground_asphalt_synth_12.png",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);




	if (!texture || !bump || !AlphaTex)
	{
		return;
	}
	shader = new Shader("TexturedVertex(2).glsl", "texturedfragment(2).glsl");

	if (!shader->LoadSuccess())
	{
		return;
	}
	filtering = true;
	repeating = false;
	init = true;

	blendMode = 0;

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);


}
Renderer ::~Renderer(void)
{
	delete triangle;
	delete shader;
	glDeleteTextures(1, &texture);
}
void Renderer::RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	BindShader(shader);
	UpdateShaderMatrices();


	SetTextureToShader(texture , 0, "diffuseTex", shader);
	SetTextureToShader(AlphaTex, 1, "alphaTex" , shader);
	SetTextureToShader(bump, 2, "bumpTex", shader);

	glUniform4fv(glGetUniformLocation(currentShader->GetProgram(),
		"EmmisColour"), 1, (float*)&Vector4(1, 0, 0, 1));


	//glUniform4fv()


	//SetTexture(texture,"")


	//glUniform1i(glGetUniformLocation(shader->GetProgram(), "diffuseTex"), 0); // this last parameter
	//glActiveTexture(GL_TEXTURE0); // should match this number !
	//glBindTexture(GL_TEXTURE_2D, texture);

	//glUniform1i(glGetUniformLocation(shader->GetProgram(), "alphaTex"), 1); // this last parameter
	//glActiveTexture(GL_TEXTURE1); // should match this number !
	//glBindTexture(GL_TEXTURE_2D, AlphaTex);



	triangle->Draw();
}
void Renderer::UpdateTextureMatrix(float value)
{
	Matrix4 push = Matrix4::Translation(Vector3(-0.5f, -0.5f, 0));
	Matrix4 pop = Matrix4::Translation(Vector3(0.5f, 0.5f, 0));
	Matrix4 rotation = Matrix4::Rotation(value, Vector3(0, 0, 1));
	textureMatrix = pop * rotation * push;
}

void Renderer::UpdateTextureMatrixTiling(float value)
{
	Vector3 scale = textureMatrix.GetScalingVector();
	textureMatrix.SetScalingVector(Vector3(scale.x + value, scale.y + value, scale.z + value));
}
void Renderer::ToggleRepeating()
{
	repeating = !repeating;
	SetTextureRepeating(texture, repeating);
}
void Renderer::ToggleFiltering()
{
	filtering = !filtering;
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		filtering ? GL_LINEAR : GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		filtering ? GL_LINEAR : GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
}



void Renderer::ToggleBlendMode()
{
	blendMode = (blendMode + 1) % 4;
	switch (blendMode)
	{
	case (0): glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); break;
	case (1): glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR); break;
	case (2): glBlendFunc(GL_ONE, GL_ZERO); break;
	case (3): glBlendFunc(GL_SRC_ALPHA, GL_ONE); break;
	};
}