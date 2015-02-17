#ifndef COLLISION_VOLUME_H
#define COLLISION_VOLUME_H

#include "Vector3.h"

class collisionVolume
{
public:
	collisionVolume(Vector3 cC, float Mass, float Velocity, bool Elastic);
	collisionVolume();
	~collisionVolume();


	Vector3 getCentre(void)const;
	void setCentre(Vector3 cC);

	float getMass(void)const;
	void setMass(float m);

	float getVelocity(void)const;
	void setVelocity(float v);

	float getCOORD(int Axis)const;
	void setCOORD(float X, float Y, float Z);

	bool getCollisionType(void)const;
	void setCollisionType(bool elastic);

private:
	Vector3 collisionCentre;
	float mass;
	float velocity;
	bool isElastic;
};

#endif COLLISION_VOLUME_H