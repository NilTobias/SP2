#include "collisionVolume.h"
#include <math.h>

collisionVolume::collisionVolume(Vector3 cC, float Mass, float Velocity, bool Elastic)
{
	setCentre(cC);
	if (Mass > 0)
		mass = Mass;
	velocity = Velocity;
	isElastic = Elastic;
}

collisionVolume::collisionVolume()
{
	collisionCentre.SetZero();
	isElastic = false;
	mass = 1;
	velocity = 0;
}

collisionVolume::~collisionVolume()
{

}

Vector3 collisionVolume::getCentre(void)const
{
	return collisionCentre;
}
void collisionVolume::setCentre(Vector3 cC)
{
	collisionCentre = cC;
}

float collisionVolume::getMass(void)const
{
	return mass;
}
void collisionVolume::setMass(float m)
{
	if (m > 0)
		mass = m;
}

float collisionVolume::getVelocity(void)const
{
	return velocity;
}
void collisionVolume::setVelocity(float v)
{
	velocity = v;
}

float collisionVolume::getCOORD(int Axis)const
{
	if (Axis == 0)
		return collisionCentre.x;
	else if (Axis == 1)
		return collisionCentre.y;
	else
		return collisionCentre.z;
}
void collisionVolume::setCOORD(float X, float Y, float Z)
{
	collisionCentre.x = X;
	collisionCentre.y = Y;
	collisionCentre.z = Z;
}

bool collisionVolume::getCollisionType(void)const
{
	return isElastic;
}
void collisionVolume::setCollisionType(bool elastic)
{
	isElastic = elastic;
}
