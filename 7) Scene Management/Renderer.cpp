# include "Renderer.h"

# include <algorithm > // For std :: sort ...
#include <time.h> 

Renderer::Renderer(Window& parent) : OGLRenderer(parent)
{
	camera = new Camera(0.0f, 0.0f, (Vector3(0, 100, 750.0f)), 50);
	quad = Mesh::GenerateQuad();
	cube = Mesh::LoadFromMeshFile("sphere.msh");


	shader = new Shader("RimGlowVertex.glsl", "RimGlowFrag.glsl");

	texture = SOIL_load_OGL_texture(TEXTUREDIR "stainedglass.tga",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);

	texture2 = SOIL_load_OGL_texture(TEXTUREDIR "Barren Reds.JPG",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);



	SetTextureRepeating(texture, true);
	//setTextureF

	if (!shader->LoadSuccess() || !texture)
	{
		return;
	}

	root = new SceneNode();

	/*for (int i = 0; i < 5; ++i)
	{
		SceneNode* s = new SceneNode();
		s->SetColour(Vector4(1.0f, 1.0f, 1.0f, 0.5f));
		s->SetTransform(Matrix4::Translation(
			Vector3(0, 100.0f, -300.0f + 100.0f + 100 * i)));
		s->SetModelScale(Vector3(100.0f, 100.0f, 100.0f));
		s->SetBoundingRadius(100.0f);
		s->SetMesh(quad);
		s->SetTexture(texture);
		s->SetTextureMatrix(Matrix4::Scale(Vector3(10, 10, 10)));
		root->AddChild(s);
	}*/

	SceneNode* node = new SceneNode();
	node->SetColour(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	node->SetTransform(Matrix4::Translation(
		Vector3(0, 100.0f, 500.0f)));
	node->SetModelScale(Vector3(100.0f, 100.0f, 100.0f));
	node->SetBoundingRadius(100.0f);
	node->SetMesh(cube);
	node->SetTexture(texture2);
	node->SetTextureMatrix(Matrix4::Scale(Vector3(1, 1, 1)));
	root->AddChild(node);





	projMatrix = Matrix4::Perspective(1.0f, 10000.0f,
		(float)width / (float)height, 45.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);
	//glBlendFunc(GL_ONE, GL_ZERO);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	init = true;
}

Renderer ::~Renderer(void)
{
	delete root;
	delete quad;
	delete camera;
	delete cube;
	delete shader;
	glDeleteTextures(1, &texture);

}



void Renderer::UpdateScene(float dt)
{
	camera->UpdateCamera(dt);
	viewMatrix = camera->BuildViewMatrix();
	//frameFrustum.FromMatrix(projMatrix * viewMatrix);

	root->Update(dt);

}

void Renderer::BuildNodeLists(SceneNode* from)
{
	//if (frameFrustum.InsideFrustum(*from))
	{
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

	}

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
		Matrix4 model = n->GetWorldTransform() *
			Matrix4::Scale(n->GetModelScale());

		glUniformMatrix4fv(
			glGetUniformLocation(shader->GetProgram(),
				"modelMatrix"), 1, false, model.values);

		glUniform4fv(glGetUniformLocation(shader->GetProgram(),
			"nodeColour"), 1, (float*)&n->GetColour());

		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(),
			"textureMatrix"), 1, false, n->GetTextureMatrix().values);




		texture = n->GetTexture();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		//G

		glUniform1i(glGetUniformLocation(shader->GetProgram(),
			"useTexture"), texture);

		n->Draw(*this);

	}
}


void Renderer::RenderScene()
{
	BuildNodeLists(root);
	SortNodeLists();

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	BindShader(shader);
	UpdateShaderMatrices();

	glUniform3fv(glGetUniformLocation(shader->GetProgram(),
		"diffuseColor"), 0, (float*)&Vector3(1, 1, 1));

	//glUniform2fv(glGetUniformLocation(shader->GetProgram(),
		//"pattern_scale"), 0, (float*)&Vector2(.5f, .5f));


	//glUniform1fv(glGetUniformLocation(shader->GetProgram(),
		//"time"), 0, (float*)&std::chrono::high_resolution_clock::now());


	//glUniform1i(glGetUniformLocation(shader->GetProgram(),
		//"diffuseTex"), 0);


	DrawNodes();

	ClearNodeLists();

}

void Renderer::ClearNodeLists()
{
	transparentNodeList.clear();
	nodeList.clear();

}