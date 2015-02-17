#ifndef COLLISION_SPHERE_H
#define COLLISION_SPHERE_H
#include "collisionVolume.h"


class collisionSphere : public collisionVolume
{
public:
	collisionSphere(float r);
	collisionSphere();
	~collisionSphere();

	float getRadius(void)const;
	void setRadius(float r);

	bool checkCollision(collisionSphere *target);

private:
	float radius;
};


#endif