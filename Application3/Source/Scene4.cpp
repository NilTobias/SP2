#include "Scene4.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Mtx44.h"

#include "Application.h"
#include "MeshBuilder.h"

Scene4::Scene4()
{
}

Scene4::~Scene4()
{
}

void Scene4::Init()
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
	
	//Load vertex and fragment shaders
	m_programID = LoadShaders( "Shader//TransformVertexShader.vertexshader", "Shader//SimpleFragmentShader.fragmentshader" );
	// Use our shader
	glUseProgram(m_programID);
	// Get a handle for our "MVP" uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	
	//variable to rotate geometry
	rotateAngle = 0;
	rotateMoon = 15;

	//Initialize camera settings
	camera.Init(Vector3(40, 30, 30), Vector3(0, 0, 0), Vector3(0, 1, 0));

		Mtx44 projection;
		projection.SetToPerspective(45.f,4.f/3.f,0.1f,1000.f);
		projectionStack.LoadMatrix(projection);

	//remove all glGenBuffers, glBindBuffer, glBufferData code
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);
	//meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(0,1,0),1);
	//meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", Color(1, 0, 0), 1,1,1);
	//meshList[GEO_CIRCLE] = MeshBuilder::GenerateCircle ("circle", Color(1,0,1), 36, 1);
	meshList[GEO_SRING1] = MeshBuilder::GenerateRing ("SOuterRing", Color(0,1,1), 36, 1.f, 0.5f);
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere ("sphere", Color(0,1,1), 18, 36, 1.f);
	meshList[GEO_SUN] = MeshBuilder::GenerateSphere ("Sun", Color(1,1,0), 18, 36, 2.f);
	meshList[GEO_MERCURY] = MeshBuilder::GenerateSphere ("Mercury", Color(0,1,0), 18, 36, 0.4f);
	meshList[GEO_VENUS] = MeshBuilder::GenerateSphere("Venus", Color(0,0,1),18,36, 0.6f);
	meshList[GEO_EARTH] = MeshBuilder::GenerateSphere("Earth", Color(0, 0.5f, 0.5f), 18, 36, 0.8f);
	meshList[GEO_EMOON] = MeshBuilder::GenerateSphere("EMoon", Color(0.3f, 0.8f, 0.5f), 18, 36, 0.1f);
	meshList[GEO_MARS] = MeshBuilder::GenerateSphere("Mars", Color(0.8, 0.5f, 0.5f), 18, 36, 0.5f);
	meshList[GEO_JUPITER] = MeshBuilder::GenerateSphere("Jupiter", Color(0.2, 0.2f, 0.3f), 18, 36, 0.81f);
	meshList[GEO_SATURN] = MeshBuilder::GenerateSphere("Saturn", Color(0.9f, 0.0f, 0.4f), 18, 36, 1.f);
	meshList[GEO_URANUS] = MeshBuilder::GenerateSphere("Uranus", Color(0.9f, 0.5f, 0.4f), 18, 36, 0.85f);
	meshList[GEO_NEPTUNE] = MeshBuilder::GenerateSphere("Neptune", Color(0.2f, 0.5f, 0.3f), 18, 36, 0.87f);
}

static float ROT_LIMIT = 45.f;
static float SCALE_LIMIT = 5.f;

void Scene4::Update(double dt)
{
	if(Application::IsKeyPressed('1')) //enable back face culling
		glEnable(GL_CULL_FACE);
	if(Application::IsKeyPressed('2')) //disable back face culling
		glDisable(GL_CULL_FACE);
	if(Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //default fill mode
	if(Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode

	rotateAngle += (float)(10 * dt);
	rotateMoon += (float)(60 * dt);
	camera.Update(dt);
}

void Scene4::Render()
{
	// Render VBO here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Temp variables
	Mtx44 MVP;

	viewStack.LoadIdentity();
	viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z, camera.target.x, 
		camera.target.y,camera.target.z,
		camera.up.x, camera.up.y,camera.up.z
		);

	modelStack.LoadIdentity();
	
	MVP= projectionStack.Top()* viewStack.Top()* modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	
	meshList[GEO_AXES]->Render();

	modelStack.PushMatrix(); //Sun
	modelStack.Rotate(rotateAngle,0,1,0);
	modelStack.Scale(4,4,4);
	MVP= projectionStack.Top()* viewStack.Top()* modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	meshList[GEO_SUN]->Render();
	modelStack.PopMatrix();

	modelStack.PushMatrix(); //MERCURY
	modelStack.Rotate(rotateAngle + 15,0,1,0);
	modelStack.Scale(2,2,2);
	modelStack.Translate(5,0,1);
	MVP= projectionStack.Top()* viewStack.Top()* modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	meshList[GEO_MERCURY]->Render();
	modelStack.PopMatrix();

	modelStack.PushMatrix(); //VENUS
	modelStack.Rotate(rotateAngle + 40, 0, 1, 0);
	modelStack.Scale(2,2,2);
	modelStack.Translate(7.3, 0, 1);
	MVP = projectionStack.Top()* viewStack.Top()* modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	meshList[GEO_VENUS]->Render();
	modelStack.PopMatrix();

	modelStack.PushMatrix(); //EARTH
	modelStack.Rotate(rotateAngle + 65, 0, 1, 0);
	modelStack.Scale(2, 2, 2);
	modelStack.Translate(10.5f, 0, 1);
	MVP = projectionStack.Top()* viewStack.Top()* modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	meshList[GEO_EARTH]->Render();

	modelStack.PushMatrix(); //MOON
	modelStack.Rotate(rotateMoon,0, 1, 0);
	modelStack.Scale(2, 2, 2);
	modelStack.Translate(0.0f, 0.2f, 1);
	MVP = projectionStack.Top()* viewStack.Top()* modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	meshList[GEO_EMOON]->Render();
	modelStack.PopMatrix();
	modelStack.PopMatrix();

	modelStack.PushMatrix(); //MARS
	modelStack.Rotate(rotateAngle - 35, 0, 1, 0);
	modelStack.Scale(2, 2, 2);
	modelStack.Translate(13.5f, 0.2f, 1);
	MVP = projectionStack.Top()* viewStack.Top()* modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	meshList[GEO_MARS]->Render();
	modelStack.PopMatrix();

	modelStack.PushMatrix(); //JUPITER
	modelStack.Rotate(rotateAngle - 5, 0, 1, 0);
	modelStack.Scale(2, 2, 2);
	modelStack.Translate(15.5f, 0.2f, 1);
	MVP = projectionStack.Top()* viewStack.Top()* modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	meshList[GEO_JUPITER]->Render();
	modelStack.PopMatrix();

	modelStack.PushMatrix(); //SATURN
	modelStack.Rotate(rotateAngle - 20, 0, 1, 0);
	modelStack.Scale(2, 2, 2);
	modelStack.Translate(19.5f, 0.2f, 1);
	MVP = projectionStack.Top()* viewStack.Top()* modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	meshList[GEO_SATURN]->Render();

	modelStack.PushMatrix(); //SATURN OUTER RING
	modelStack.Rotate(rotateAngle, 0, 1, 0);
	modelStack.Scale(2, 2, 2);
	MVP = projectionStack.Top()* viewStack.Top()* modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	meshList[GEO_SRING1]->Render();
	modelStack.PopMatrix();
	modelStack.PopMatrix();


	modelStack.PushMatrix(); //UR ANUS
	modelStack.Rotate(rotateAngle + 90, 0, 1, 0);
	modelStack.Scale(2, 2, 2);
	modelStack.Translate(23.5f, 0.2f, 1);
	MVP = projectionStack.Top()* viewStack.Top()* modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	meshList[GEO_URANUS]->Render();
	modelStack.PopMatrix();

	modelStack.PushMatrix(); //NEPTUNE
	modelStack.Rotate(rotateAngle + 75, 0, 1, 0);
	modelStack.Scale(2, 2, 2);
	modelStack.Translate(26.5f, 0.2f, 1);
	MVP = projectionStack.Top()* viewStack.Top()* modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	meshList[GEO_NEPTUNE]->Render();
	modelStack.PopMatrix();

}

void Scene4::Exit()
{
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}
