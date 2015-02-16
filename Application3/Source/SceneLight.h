#ifndef SCENE_LIGHT_H
#define SCENE_LIGHT_H

#include "Scene.h"
#include "Camera2.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "Utility.h"

class SceneLight : public Scene
{
		enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHTENABLED,
		U_TOTAL,
	};

	enum GEOMETRY_TYPE
	{
		GEO_CUBE,
		GEO_AXES,
		GEO_QUAD,
		GEO_SPHERE,
		GEO_HEAD,
		GEO_LIGHTBALL,
		NUM_GEOMETRY,
	};
public:
	SceneLight();
	~SceneLight();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
private:

	void RenderMesh(Mesh *mesh, bool enableLight);
	

	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];

	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];
	
	float rotateAngle;
	float rotateMoon;
	float LSPEED;
	bool lightOn;


	Camera2 camera;
	Light light[1];

	MS modelStack, viewStack, projectionStack;
};

#endif