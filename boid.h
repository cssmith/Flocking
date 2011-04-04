#ifndef BOID_H
#define BOID_H

#include "constants.h"

class Boid {
	float x, y;
	float radius;
	float heading_x, heading_y;
	float velocity;
	Color color;

public:
	Boid();
	Boid(float x, float y, float radius, Color color);
	Boid(float x, float y, float radius, float vel, float h_x, float h_y, Color color);

	void setVelocity(float new_velocity);
	void setHeading(float new_x, float new_y);
	void draw();
	void updatePosition();
	void updateDirection(std::vector<Boid> neighbors);
	float distanceTo(Boid other);
	bool compareTo(Boid other);
	void clicked(int x, int y);
};

#endif