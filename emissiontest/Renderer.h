#pragma once
# include "../nclgl/OGLRenderer.h"
# include "../nclgl/Camera.h"
# include "../nclgl/HeightMap.h"
# include "../nclgl/SceneNode.h"
# include "../nclgl/Frustum.h"

class Renderer : public OGLRenderer
{
public:
	Renderer(Window& parent);
	~Renderer(void);

	void RenderScene() override;
	void UpdateScene(float dt) override;

	SceneNode* root = new SceneNode();

	SceneNode* depthTest;


	const int numOfBuildings = 1;

protected:
	void DrawSkybox();
	void FillBuffers(); //G- Buffer Fill Render Pass
	void DrawPointLights(); // Lighting Render Pass
	void CombineBuffers(); // Combination Render Pass
						   // Make a new texture ...
	void GenerateScreenTexture(GLuint& into, bool depth = false);

	void GenerateBuildings();

	Shader* sceneShader;      // Shader to fill our GBuffers
	Shader* pointlightShader; // Shader to calculate lighting
	Shader* combineShader;    // shader to stick it all together
	Shader* skyboxShader;     // skybox

	Shader* textureShader;    // depthTest;



	GLuint bufferFBO; // FBO for our G- Buffer pass
	GLuint bufferColourTex; // Albedo goes here
	GLuint bufferNormalTex; // Normals go here
	GLuint bufferDepthTex; // Depth goes here
	GLuint bufferEmissionTex; // emmision goes here

	GLuint buildingTex;
	GLuint buildingemmis;


	GLuint pointLightFBO; // FBO for our lighting pass
	GLuint lightDiffuseTex; // Store diffuse lighting
	GLuint lightSpecularTex; // Store specular lighting
	Light* pointLights; // Array of lighting data
	Mesh* sphere; // Light volume
	Mesh* quad; // To draw a full - screen quad
	Mesh* cube;
	Camera* camera; // Our usual camera
	GLuint earthTex;
	GLuint earthBump;

	Vector4 emissionColor;


	GLuint tex;
	GLuint bumpTex;
	GLuint emmisTex;

	GLuint Skybox;


	void BuildNodeLists(SceneNode* from);
	void SortNodeLists();
	void ClearNodeLists();
	void DrawNodes();
	void DrawNode(SceneNode* n);

	void DrawNode(SceneNode* n, bool tex);




	//Frustum frameFrustum;

	vector < SceneNode*> transparentNodeList;
	vector < SceneNode*> nodeList;



};
