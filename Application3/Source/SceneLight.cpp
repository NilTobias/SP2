#include "SceneLight.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Mtx44.h"

#include "Application.h"
#include "MeshBuilder.h"

SceneLight::SceneLight()
{
}

SceneLight::~SceneLight()
{
}

void SceneLight::Init()
{
	// Init VBO here

	// Set background color to dark blue
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	//Enable depth buffer and depth testing
	glEnable(GL_DEPTH_TEST);

	//Enable back face culling
	glEnable(GL_CULL_FACE);

	//Default to fill mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	LSPEED = 20.f;
	lightOn = true;

	//Load vertex and fragment shaders
	m_programID = LoadShaders( "Shader//Shading.vertexshader", "Shader//Shading.fragmentshader" );
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	glUseProgram(m_programID);

	light[0].position.Set(10, 20, 10);
	light[0].color.Set(1, 1, 1);
	light[0].power = 10;
	light[0].kC = 0.7f;
	light[0].kL = 0.05f;
	light[0].kQ = 0.003f;

	// Make sure you pass uniform parameters after glUseProgram()
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);


	//HEAD (ROOT) -----------------------------------------------------------------------
	meshList[GEO_HEAD] = MeshBuilder::GenerateSphere("Head", Color(0.3f, 0.5f, 0), 18, 36, 10);
	meshList[GEO_HEAD]->material.kAmbient.Set(0.15f, 0.15f, 0.15f);
	meshList[GEO_HEAD]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_HEAD]->material.kSpecular.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_HEAD]->material.kShininess = 5.f;

	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("Lightball",Color(1,1,1),10,10,1);

	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("Floor",Color(0.8,0.8,0.8), 1);
	meshList[GEO_QUAD]->material.kAmbient.Set(0.15f, 0.15f, 0.15f);
	meshList[GEO_QUAD]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_QUAD]->material.kSpecular.Set(0.f, 0.f, 0.f);
	meshList[GEO_QUAD]->material.kShininess = 5.f;


	//variable to rotate geometry
	rotateAngle = 0;
	rotateMoon = 15;

	//Initialize camera settings
	camera.Init(Vector3(100, 50, 50), Vector3(0, 0, 0), Vector3(0, 1, 0));

	Mtx44 projection;
	projection.SetToPerspective(45.f,4.f/3.f,0.1f,1000.f);
	projectionStack.LoadMatrix(projection);

	//remove all glGenBuffers, glBindBuffer, glBufferData code
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);
	//meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(0,1,0),1);
	//meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", Color(1, 0, 0), 1,1,1);
	//meshList[GEO_CIRCLE] = MeshBuilder::GenerateCircle ("circle", Color(1,0,1), 36, 1);
	//meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere ("sphere", Color(0,1,1), 18, 36, 1.f);
}

static float ROT_LIMIT = 45.f;
static float SCALE_LIMIT = 5.f;

void SceneLight::Update(double dt)
{
	if(Application::IsKeyPressed('1')) //enable back face culling
		glEnable(GL_CULL_FACE);
	if(Application::IsKeyPressed('2')) //disable back face culling
		glDisable(GL_CULL_FACE);
	if(Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //default fill mode
	if(Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode

	if(Application::IsKeyPressed('I'))
		light[0].position.z -= (float)(LSPEED * dt);
	if(Application::IsKeyPressed('K'))
		light[0].position.z += (float)(LSPEED * dt);
	if(Application::IsKeyPressed('J'))
		light[0].position.x -= (float)(LSPEED * dt);
	if(Application::IsKeyPressed('L'))
		light[0].position.x += (float)(LSPEED * dt);
	if(Application::IsKeyPressed('O'))
		light[0].position.y -= (float)(LSPEED * dt);
	if(Application::IsKeyPressed('P'))
		light[0].position.y += (float)(LSPEED * dt);

	if(Application::IsKeyPressed('T'))
		lightOn = true;

	if (Application::IsKeyPressed('Y'))
		lightOn = false;
	
	rotateAngle += (float)(10 * dt);
	rotateMoon += (float)(60 * dt);
	camera.Update(dt);
}


void SceneLight::RenderMesh(Mesh *mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if(enableLight)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView = viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{	
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}
	mesh->Render();
}


void SceneLight::Render()
{
	// Render VBO here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	viewStack.LoadIdentity();
	viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z, camera.target.x, 
		camera.target.y,camera.target.z,
		camera.up.x, camera.up.y,camera.up.z
		);

	modelStack.LoadIdentity();

	Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
	glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);

	RenderMesh(meshList[GEO_AXES], false);
	//RenderMesh(meshList[GEO_TEST], false);

	modelStack.PushMatrix();
	modelStack.Translate(light[0].position.x, light[0].position.y, light[0].position.z);
	RenderMesh(meshList[GEO_LIGHTBALL], lightOn);
	modelStack.PopMatrix();


	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_HEAD], lightOn);
	modelStack.PopMatrix();

	/*
	modelStack.PushMatrix();
	modelStack.Translate(-0.5, 5.5, 0);
	modelStack.Scale(0.5f, 0.5f, 0.5f);
	RenderMesh(meshList[GEO_HEAD], lightOn);
	
	modelStack.PushMatrix();
	modelStack.Translate(2.5,-7.5f,0);
	modelStack.Scale(1.2f, 0.75f, 1.1f);
	RenderMesh(meshList[GEO_MOUTH], lightOn);
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	*/

	/*
	modelStack.PushMatrix();
	modelStack.Translate(-20,0,-20);
	RenderMesh(meshList[GEO_TEST], lightOn);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(20,0,0);
	RenderMesh(meshList[GEO_SPHERE2], lightOn);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-20,0,0);
	RenderMesh(meshList[GEO_SPHERE3], lightOn);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0,0,20);
	RenderMesh(meshList[GEO_SPHERE4], lightOn);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0,0,-20);
	RenderMesh(meshList[GEO_SPHERE5], lightOn);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(20,0,-20);
	RenderMesh(meshList[GEO_SPHERE6], lightOn);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(20,0,20);
	RenderMesh(meshList[GEO_SPHERE7], lightOn);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-20,0,20);
	RenderMesh(meshList[GEO_SPHERE8], lightOn);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-20,0,-20);
	RenderMesh(meshList[GEO_SPHERE9], lightOn);
	modelStack.PopMatrix();
	*/

	/*
	modelStack.PushMatrix();
	modelStack.Translate(-30,-10,25);
	modelStack.Rotate(-90,1,0,0);
	modelStack.Scale(100,100,100);
	RenderMesh(meshList[GEO_QUAD], lightOn);
	modelStack.PopMatrix();
	*/



}

void SceneLight::Exit()
{
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}
