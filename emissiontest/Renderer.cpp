#include"Renderer.h"
#include <algorithm>
const int LIGHT_NUM = 32;
Renderer::Renderer(Window& parent) : OGLRenderer(parent)
{



	sphere = Mesh::LoadFromMeshFile("Sphere.msh");
	quad = Mesh::GenerateQuad();
	cube = Mesh::LoadFromMeshFile("cube.msh");

	earthTex = SOIL_load_OGL_texture(
		TEXTUREDIR"/Sush/ground_asphalt_synth_11.png", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	earthBump = SOIL_load_OGL_texture(
		TEXTUREDIR"/Sush/ground_asphalt_synth_11_Bump.png", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	buildingemmis = SOIL_load_OGL_texture(
		TEXTUREDIR"/Sush/window2.png", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	buildingTex = SOIL_load_OGL_texture(
		TEXTUREDIR"/Sush/window1.png", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);


	Skybox = SOIL_load_OGL_cubemap(
		TEXTUREDIR"/Sush/skybox/S.png", TEXTUREDIR"/Sush/skybox/N.png",
		TEXTUREDIR"/Sush/skybox/U.png", TEXTUREDIR"/Sush/skybox/D.png",
		TEXTUREDIR"/Sush/skybox/E.png", TEXTUREDIR"/Sush/skybox/W.png",
		SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);

	//buildings ;


	//glActiveTexture(earthTex);
	//Wow64SetThreadContext
	SetTextureRepeating(earthTex, true, false);
	SetTextureRepeating(earthBump, true, false);

	SetTextureRepeating(buildingTex, true);
	SetTextureRepeating(buildingemmis, true);

	camera = new Camera(0.0f, -90.0f, (Vector3(0, 300, 750.0f)), 3000);



	pointLights = new Light[LIGHT_NUM];

	for (int i = 0; i < LIGHT_NUM; ++i)
	{
		Light& l = pointLights[i];
		l.SetPosition(Vector3(rand() % 5000,
			150.0f,
			rand() % 5000));

		l.SetColour(Vector4(0.5f + (float)(rand() / (float)RAND_MAX),
			0.5f + (float)(rand() / (float)RAND_MAX),
			0.5f + (float)(rand() / (float)RAND_MAX),
			1));
		l.SetRadius(250.0f + (rand() % 250));
	}





	sceneShader = new Shader("bumpvertex.glsl", // reused !
		"bufferFragment.glsl");
	pointlightShader = new Shader("pointlightvertex.glsl",
		"pointlightfragment.glsl");
	combineShader = new Shader("combinevert.glsl",
		"combinefrag.glsl");
	skyboxShader = new Shader(
		"skyboxVertex.glsl", "skyboxFragment.glsl");

	textureShader = new Shader("TexturedVertex.glsl", "texturedfragment.glsl");


	bool scene, point, combine, sky;
	scene = sceneShader->LoadSuccess();
	point = pointlightShader->LoadSuccess();
	combine = combineShader->LoadSuccess();
	sky = skyboxShader->LoadSuccess();
	if (!scene || !point || !combine || !sky)
	{
		return;
	}
	glGenFramebuffers(1, &bufferFBO);
	glGenFramebuffers(1, &pointLightFBO);






	SceneNode* s = new SceneNode();
	s->SetColour(Vector4(1.0f, 1.0f, 1.0f, 0.5f));
	s->SetTransform(Matrix4::Translation(Vector3(5000.0f, 00, -100.0f)) *
		Matrix4::Rotation(-90, Vector3(1, 0, 0)));
	//s->SetTransform(Matrix4::Rotation(-90, Vector3(0, 0, 1)));
	s->SetModelScale(Vector3(10000.0f, 10000.0f, 10000.0f));
	//s->SetBoundingRadius(100.0f);
	s->SetMesh(quad);
	s->SetTexture(earthTex);
	s->SetBumpTex(earthBump);

	s->SetTextureMatrix(Matrix4::Scale(Vector3(10, 10, 1))
		* Matrix4::Translation(Vector3(0.5f, 0.5f, 0))
		* Matrix4::Rotation(90, Vector3(0, 0, 1))
	);
	root->AddChild(s);



	//depthTest = new SceneNode();
	//depthTest->SetColour(Vector4(1.0f, 1.0f, 1.0f, 1));
	//depthTest->SetTransform(Matrix4::Translation(Vector3(0, 00, -100.0f))
	//	//*		Matrix4::Rotation(-90, Vector3(1, 0, 0))
	//	//s->SetTransform(Matrix4::Rotation(-90, Vector3(0, 0, 1))
	//);
	//depthTest->SetModelScale(Vector3(10000.0f, 10000.0f, 10000.0f));
	////s->SetBoundingRadius(100.0f);
	//depthTest->SetMesh(cube);
	//depthTest->SetTexture(buildingTex);
	//depthTest->name = "depth";
	//s->SetBumpTex(earthBump);

	//s->SetTextureMatrix(Matrix4::Scale(Vector3(10, 10, 1))
	//* Matrix4::Translation(Vector3(0.5f, 0.5f, 0))
	//* Matrix4::Rotation(90, Vector3(0, 0, 1))
	//);













	GenerateBuildings();


	emissionColor = Vector4(1, 1, 0, 1);


	GLenum buffers[3] = {
		GL_COLOR_ATTACHMENT0,
		GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2
	};

	// Generate our scene depth texture...
	GenerateScreenTexture(bufferDepthTex, true);
	GenerateScreenTexture(bufferColourTex);
	GenerateScreenTexture(bufferNormalTex);
	GenerateScreenTexture(bufferEmissionTex);
	GenerateScreenTexture(lightDiffuseTex);
	GenerateScreenTexture(lightSpecularTex);

	// And now attach them to our FBOs
	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, bufferColourTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,
		GL_TEXTURE_2D, bufferNormalTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2,
		GL_TEXTURE_2D, bufferEmissionTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D, bufferDepthTex, 0);
	glDrawBuffers(3, buffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) !=
		GL_FRAMEBUFFER_COMPLETE)
	{
		return;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, pointLightFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, lightDiffuseTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,
		GL_TEXTURE_2D, lightSpecularTex, 0);
	glDrawBuffers(2, buffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) !=
		GL_FRAMEBUFFER_COMPLETE)
	{
		return;
	}





	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);



	init = true;
}
Renderer ::~Renderer(void)
{
	delete sceneShader;
	delete combineShader;
	delete pointlightShader;

	delete camera;
	delete sphere;
	delete quad;
	delete[] pointLights;
	glDeleteTextures(1, &bufferColourTex);
	glDeleteTextures(1, &bufferNormalTex);
	glDeleteTextures(1, &bufferEmissionTex);
	glDeleteTextures(1, &bufferDepthTex);
	glDeleteTextures(1, &lightDiffuseTex);
	glDeleteTextures(1, &lightSpecularTex);

	glDeleteFramebuffers(1, &bufferFBO);
	glDeleteFramebuffers(1, &pointLightFBO);
}
void Renderer::GenerateScreenTexture(GLuint& into, bool depth)
{
	glGenTextures(1, &into);
	glBindTexture(GL_TEXTURE_2D, into);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	GLuint format = depth ? GL_DEPTH_COMPONENT24 : GL_RGBA8;
	GLuint type = depth ? GL_DEPTH_COMPONENT : GL_RGBA;

	glTexImage2D(GL_TEXTURE_2D, 0,
		format, width, height, 0, type, GL_UNSIGNED_BYTE, NULL);

	glBindTexture(GL_TEXTURE_2D, 0);
}


void Renderer::DrawSkybox()
{
	glDepthMask(GL_FALSE);

	BindShader(skyboxShader);
	UpdateShaderMatrices();

	quad->Draw();

	glDepthMask(GL_TRUE);
}










void Renderer::UpdateScene(float dt)
{
	camera->UpdateCamera(dt);
	viewMatrix = camera->BuildViewMatrix();
	//frameFrustum.FromMatrix(projMatrix * viewMatrix);
	root->Update(dt);


}
void Renderer::RenderScene()
{



	BuildNodeLists(root);
	SortNodeLists();
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	FillBuffers();
	ClearNodeLists();
	DrawPointLights();
	CombineBuffers();





}
void Renderer::FillBuffers()
{

	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	DrawSkybox();


	BindShader(sceneShader);

	modelMatrix.ToIdentity();
	viewMatrix = camera->BuildViewMatrix();
	projMatrix = Matrix4::Perspective(1.0f, 100000.0f,
		(float)width / (float)height, 45.0f);



	UpdateShaderMatrices();

	DrawNodes();


	/*glUniform1i(
		glGetUniformLocation(sceneShader->GetProgram(), "diffuseTex"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, earthTex);


	glUniform1i(
		glGetUniformLocation(sceneShader->GetProgram(), "bumpTex"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, earthBump);
	*/
	//heightMap->Draw();
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void Renderer::DrawPointLights()
{
	glBindFramebuffer(GL_FRAMEBUFFER, pointLightFBO);
	BindShader(pointlightShader);

	glClearColor(0, 0, 0, 1);
	glClear     (GL_COLOR_BUFFER_BIT);
	glBlendFunc (GL_ONE, GL_ONE);
	glCullFace  (GL_FRONT);
	glDepthFunc (GL_ALWAYS);
	glDepthMask (GL_FALSE);

	glUniform1i(glGetUniformLocation(
		pointlightShader->GetProgram(), "depthTex"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, bufferDepthTex);

	glUniform1i(glGetUniformLocation(
		pointlightShader->GetProgram(), "normTex"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, bufferNormalTex);

	glUniform3fv(glGetUniformLocation(pointlightShader->GetProgram(),
		"cameraPos"), 1, (float*)&camera->GetPosition());

	glUniform2f(glGetUniformLocation(pointlightShader->GetProgram(),
		"pixelSize"), 1.0f / width, 1.0f / height);

	Matrix4 invViewProj = (projMatrix * viewMatrix).Inverse();
	glUniformMatrix4fv(glGetUniformLocation(
		pointlightShader->GetProgram(), "inverseProjView"),
		1, false, invViewProj.values);
	UpdateShaderMatrices();
	for (int i = 0; i < LIGHT_NUM; ++i)
	{
		Light& l = pointLights[i];
		SetShaderLight(l);
		sphere->Draw();
	}

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glCullFace(GL_BACK);


	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glClearColor(0.2f, 0.2f, 0.2f, 1);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void Renderer::CombineBuffers()
{
	//glBlitFramebuffer(bufferFBO,);
	//glBindFramebuffer(GL_READ_FRAMEBUFFER, bufferFBO);
	//glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	//glBindFramebuffer(GL_FRAMEBUFFER, combineFBO);
	BindShader(combineShader);
	modelMatrix.ToIdentity();
	viewMatrix.ToIdentity();
	projMatrix.ToIdentity();
	UpdateShaderMatrices();

	//glGetTexImage(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, GL_FLOAT, &bufferDepthTex);

	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		//GL_TEXTURE_2D, bufferDepthTex, 0);

	SetTextureToShader(bufferColourTex, 0, "diffuseTex", combineShader);
	SetTextureToShader(lightDiffuseTex, 1, "diffuseLight", combineShader);
	SetTextureToShader(bufferEmissionTex, 2, "emissionTex", combineShader);
	SetTextureToShader(lightSpecularTex, 3, "specularLight", combineShader);


	glUniform1i(glGetUniformLocation(
		combineShader->GetProgram(), "depthTex"), 4);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, bufferDepthTex);


	glUniform4fv(glGetUniformLocation(sceneShader->GetProgram(),
		"fogcolor"), 1, (float*)&Vector4(1, 0, 0, 1));


	//SetTextureToShader(bufferColourTex, 4, "depthTex", combineShader);
	//glBindTexture(GL_TEXTURE_2D, bufferDepthTex);

	//SetTextureToShader(buff)



	/*glUniform1i(glGetUniformLocation(
		combineShader->GetProgram(), "diffuseTex"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, bufferColourTex);*/

	/*glUniform1i(glGetUniformLocation(
		combineShader->GetProgram(), "diffuseLight"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, lightDiffuseTex);*/


	/*glUniform1i(glGetUniformLocation(
		combineShader->GetProgram(), "specularLight"), 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, lightSpecularTex);*/



	//glUniform4fv(glGetUniformLocation(sceneShader->GetProgram(),
		//"emissionColor"), 1, (float*)&emissionColor);

	quad->Draw();



	/*BindShader(textureShader);
	modelMatrix.ToIdentity();
	viewMatrix.ToIdentity();
	projMatrix.ToIdentity();
	UpdateShaderMatrices();*/

	/*glUniform1i(glGetUniformLocation(
		combineShader->GetProgram(), "diffuseTex"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, buildingTex);

	DrawNode(depthTest, true);*/



	//glBindFramebuffer(GL_FRAMEBUFFER, 0);



}

void Renderer::BuildNodeLists(SceneNode* from)
{
	//if (frameFrustum.InsideFrustum(*from))
	//{
	Vector3 dir = from->GetWorldTransform().GetPositionVector() -
		camera->GetPosition();
	from->SetCameraDistance(Vector3::Dot(dir, dir));

	if (from->GetColour().w < 1.0f)
	{
		transparentNodeList.push_back(from);

	}
	else
	{
		nodeList.push_back(from);

	}
	//}
	for (vector < SceneNode* >::const_iterator i =
		from->GetChildIteratorStart();
		i != from->GetChildIteratorEnd(); ++i)
	{
		BuildNodeLists((*i));

	}
}


void Renderer::SortNodeLists()
{
	std::sort(transparentNodeList.rbegin(), // note the r!
		transparentNodeList.rend(), // note the r!
		SceneNode::CompareByCameraDistance);
	std::sort(nodeList.begin(),
		nodeList.end(),
		SceneNode::CompareByCameraDistance);

}

void Renderer::DrawNodes()
{
	for (const auto& i : nodeList)
	{
		DrawNode(i);

	}
	for (const auto& i : transparentNodeList)
	{
		DrawNode(i);

	}

}


void Renderer::DrawNode(SceneNode* n)
{
	if (n->GetMesh())
	{

		if (n->name == "depth")
		{
			bool a = true;
		}

		tex = n->GetTexture();
		SetTextureToShader(tex, 0, "diffuseTex", sceneShader);

		bumpTex = n->GetBumpTex();
		SetTextureToShader(bumpTex, 1, "bumpTex", sceneShader);

		emmisTex = n->GetEmissionTex();
		SetTextureToShader(emmisTex, 2, "emiisionTex", sceneShader);

		glUniform4fv(glGetUniformLocation(sceneShader->GetProgram(),
			"emissionColor"), 1, (float*)&emissionColor);




		Matrix4 model = n->GetWorldTransform() *
			Matrix4::Scale(n->GetModelScale());
		glUniformMatrix4fv(glGetUniformLocation(sceneShader->GetProgram(),
			"modelMatrix"), 1, false, model.values);

		glUniform4fv(glGetUniformLocation(sceneShader->GetProgram(),
			"nodeColour"), 1, (float*)&n->GetColour());

		glUniformMatrix4fv(glGetUniformLocation(sceneShader->GetProgram(), "textureMatrix"),
			1, false, n->GetTextureMatrix().values);


		//bumpTex






		n->Draw(*this);

	}
}

void Renderer::DrawNode(SceneNode* n, bool tex)
{
	if (n->GetMesh())
	{

		if (n->name == "depth")
		{
			bool a = true;
		}

		tex = n->GetTexture();
		SetTextureToShader(tex, 0, "diffuseTex", textureShader);

		/*	bumpTex = n->GetBumpTex();
			SetTextureToShader(bumpTex, 1, "bumpTex", textureShader);

			emmisTex = n->GetEmissionTex();
			SetTextureToShader(emmisTex, 2, "emiisionTex", textureShader);

			glUniform4fv(glGetUniformLocation(textureShader->GetProgram(),
				"emissionColor"), 1, (float*)&emissionColor);*/


		Matrix4 model = n->GetWorldTransform() *
			Matrix4::Scale(n->GetModelScale());
		glUniformMatrix4fv(glGetUniformLocation(textureShader->GetProgram(),
			"modelMatrix"), 1, false, model.values);

		glUniform4fv(glGetUniformLocation(textureShader->GetProgram(),
			"nodeColour"), 1, (float*)&n->GetColour());

		glUniformMatrix4fv(glGetUniformLocation(textureShader->GetProgram(), "textureMatrix"),
			1, false, n->GetTextureMatrix().values);

		n->Draw(*this);

	}
}


void Renderer::ClearNodeLists()
{
	transparentNodeList.clear();
	nodeList.clear();

}

void Renderer::GenerateBuildings()
{

	// right side of road
	for (int i = 0; i < 15; i++)
	{
		SceneNode* building = new SceneNode();
		building->SetMesh(cube);

		int texid = rand() % 2;


		GLuint currentTex = texid == 0 ? buildingTex : buildingemmis;
		building->SetTexture(currentTex);
		building->SetEmissionTex(currentTex);
		//building->SetEmissionColour(Vector4(1.0f, 0.0f, 0.0f, 1.0f));

		//building->SetTexture(buildingTex2);
		float yScale = 10000 + rand() % 20000;
		float zpos = 0;
		if (i != 0)
		{
			zpos = -7000 * i;
			float offset = (rand() % 1000) + 1000;
			zpos = zpos + offset;
		}

		building->SetModelScale(Vector3(5000, yScale, 5000));
		building->SetTransform(Matrix4::Translation(Vector3(28000 + rand() % 200,
			(yScale / 2),
			zpos))
		);

		float xTexScale = 0, yTexScale = 0;

		if (texid == 0)
		{
			xTexScale = 10 + rand() % 30;
			yTexScale = 50 + rand() % 50;
		}
		else
		{
			xTexScale = 10 + rand() % 15;
			yTexScale = 50 + rand() % 10;
		}

		building->SetTextureMatrix(Matrix4::Scale(Vector3(xTexScale, yTexScale, 1))
			//* Matrix4::Translation(Vector3(0.5f, 0.5f, 0.0f))
			//* Matrix4::Rotation(180, Vector3(0, 0, 1))
		);
		//building->name = "building";
		root->AddChild(building);
	}

	//left side of road
	for (int i = 0; i < 15; i++)
	{
		SceneNode* building = new SceneNode();
		building->SetMesh(cube);
		int texid = rand() % 2;

		GLuint currentTex = texid == 0 ? buildingTex : buildingemmis;

		building->SetTexture(currentTex);
		building->SetEmissionTex(currentTex);
		//building->SetEmissionColour(Vector4(1.0f, 0.0f, 0.0f, 1.0f));

		float yScale = 10000 + rand() % 20000;
		float zpos = 0;
		if (i != 0)
		{
			zpos = -7000 * i;
			float offset = (rand() % 1000) + 1000;
			zpos = zpos + offset;
		}

		building->SetModelScale(Vector3(5000, yScale, 5000));
		building->SetTransform(Matrix4::Translation(Vector3(-5000,
			(yScale / 2),
			zpos))
		);

		float xTexScale = 0, yTexScale = 0;

		if (texid == 0)
		{
			xTexScale = 10 + rand() % 30;
			yTexScale = 50 + rand() % 50;
		}
		else
		{
			xTexScale = 10 + rand() % 15;
			yTexScale = 50 + rand() % 10;
		}

		building->SetTextureMatrix(Matrix4::Scale(Vector3(xTexScale, yTexScale, 1))
			//* Matrix4::Translation(Vector3(0.5f, 0.5f, 0.0f))
			//* Matrix4::Rotation(180, Vector3(0, 0, 1))
		);
		//building->name = "building";
		root->AddChild(building);
	}


}

//void Renderer::GenerateBuildings()
//{
//	for (int i = 0; i < numOfBuildings; i++)
//	{
//		SceneNode* node = new SceneNode();
//		node->SetMesh(cube);
//		//node->SetTexture(rand() % 2 == 0 ? buildingTex : buildingTex2);
//
//		node->SetTexture(buildingTex);
//		node->SetEmissionTex(buildingemmis);
//
//		node->SetModelScale(Vector3(5000, 60000, 5000));
//		node->SetTransform(Matrix4::Translation(Vector3(1000, 0, 10000)));
//		node->SetTextureMatrix(Matrix4::Scale(Vector3(2, 10, 1)));
//
//		root->AddChild(node);
//	}
//
//}