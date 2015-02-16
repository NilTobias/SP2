#ifndef SCENE_4_H
#define SCENE_4_H

#include "Scene.h"
#include "Camera2.h"
#include "Mesh.h"
#include "MatrixStack.h"

class Scene4 : public Scene
{
	enum GEOMETRY_TYPE
	{
		GEO_CUBE,
		GEO_AXES,
		GEO_QUAD,
		GEO_CIRCLE,
		GEO_RING,
		GEO_SPHERE,
		GEO_SUN,
		GEO_MERCURY,
		GEO_VENUS,
		GEO_EARTH,
		GEO_EMOON,
		GEO_MARS,
		GEO_JUPITER,
		GEO_SATURN,
		GEO_SRING1,
		GEO_SRING2,
		GEO_URANUS,
		GEO_NEPTUNE,
		NUM_GEOMETRY,
	};
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_TOTAL,
	};
public:
	Scene4();
	~Scene4();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
private:
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];

	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];
	
	float rotateAngle;
	float rotateMoon;

	Camera2 camera;

	MS modelStack, viewStack, projectionStack;
};

#endif