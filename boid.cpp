//
// Patrick Smith, pjs482@gmail.com
// 

#include <stdlib.h>
#include <stdio.h>
#include <gl/glut.h>
#include <math.h>
#include <vector>
#include "boid.h"
#include "constants.h"

Boid::Boid() {
	velocity = 0.0;
	heading_x = 0.0;
	heading_y = 0.0;
	x = 0.0; 
	y = 0.0;
	radius = 3.0;
	color.r = 0.0, color.g = 0.0, color.b = 1.0;
}

Boid::Boid(float x, float y, float radius, Color color) 
	:x(x), y(y), radius(radius), color(color) {
	velocity = 0.0;
	heading_x = 0.0;
	heading_y = 0.0;
}

Boid::Boid(float x, float y, float radius, float vel, float h_x, float h_y, Color color) 
	:x(x), y(y), radius(radius), color(color), velocity(vel), heading_x(h_x), heading_y(h_y) {

}

void Boid::setVelocity(float new_velocity) {
	velocity = new_velocity;
}

void Boid::setHeading(float new_x, float new_y) {
	heading_x = new_x;
	heading_y = new_y;
}

// Draw the boid as a filled circle
void Boid::draw() {
	glBegin(GL_POLYGON); 
		glLineWidth(3);  
		glColor3f(color.r, color.g, color.b);
		
		float draw_x = x + radius;
		float draw_y = y;
		for(int j = 0; j <= NUM_CIRCLE_SEGMENTS; j++)
		{
			glVertex2f(draw_x, draw_y);
			draw_x = x + radius * cos( (float)j / (float)NUM_CIRCLE_SEGMENTS * 2.0 * PI);
			draw_y = y + radius * sin( (float)j / (float)NUM_CIRCLE_SEGMENTS * 2.0 * PI);
			glVertex2f(draw_x, draw_y);
		}
	glEnd(); 
}

// Update the boid's position, using its velocity 
void Boid::updatePosition() {
	float delta = 0.1;
	x = x + delta * velocity * heading_x;
	y = y + delta * velocity * heading_y;

	// Make the boid wrap around if it is leaving the window
	if (x < LEFT) {
		x = RIGHT;
	} else if (x > RIGHT) {
		x = LEFT;
	}
	if (y < BOTTOM) {
		y = TOP;
	} else if (y > TOP) {
		y = BOTTOM;
	}
}

void Boid::updateDirection(std::vector<Boid> neighbors) {
	// set the velocity and heading to be the average of the neighbors
	float new_heading_x = 0.0;
	float new_heading_y = 0.0;
	float new_velocity = 0.0;
	float new_x = 0.0;
	float new_y = 0.0;
	for(int i=0; i < neighbors.size(); i++) {
		new_heading_x += neighbors[i].heading_x;
		new_heading_y += neighbors[i].heading_y;
		new_velocity += neighbors[i].velocity;
		new_x += neighbors[i].x;
		new_y += neighbors[i].y;
	}
	float move_to_x = (new_x / (float)HOOD_SIZE) - x;
	float move_to_y = (new_y / (float)HOOD_SIZE) - y;

	heading_x = CENTER_INFLUENCE * (move_to_x) + (1.0 - CENTER_INFLUENCE) * heading_x;
	heading_y = CENTER_INFLUENCE * (move_to_y) + (1.0 - CENTER_INFLUENCE) * heading_y;

	heading_x = PEER_INFLUENCE * (new_heading_x / (float)HOOD_SIZE) + (1.0 - PEER_INFLUENCE) * heading_x;
	heading_y = PEER_INFLUENCE * (new_heading_y / (float)HOOD_SIZE) + (1.0 - PEER_INFLUENCE) * heading_y;
	velocity = PEER_INFLUENCE * new_velocity / (float)HOOD_SIZE + (1.0 - PEER_INFLUENCE) * velocity;


	// Let instincts influence motion
	heading_x = INSTINCT_INFLUENCE * instinct_x + (1.0 - INSTINCT_INFLUENCE) * heading_x;
	heading_y = INSTINCT_INFLUENCE * instinct_y + (1.0 - INSTINCT_INFLUENCE) * heading_y;

	// Add some randomness
	heading_x += RANDOMNESS * ( (float)rand() / (float)RAND_MAX - 0.5);
	heading_y += RANDOMNESS * ( (float)rand() / (float)RAND_MAX - 0.5);
	velocity += RANDOMNESS * ( (float)rand() / (float)RAND_MAX - 0.5);

	if (velocity > MAX_VELOCITY) 
		velocity = MAX_VELOCITY;
}

// NOTE: Due to world constraints, this may not be correct...
float Boid::distanceTo(Boid other) {
	float dif_x = x - other.x;
	float dif_y = y - other.y;
	return sqrt(pow(dif_x, 2) + pow(dif_y, 2));
}

bool Boid::compareTo(Boid other) {
	return (x == other.x && y == other.y && radius == other.radius &&
		velocity == other.velocity && heading_x == heading_x &&
		heading_y == other.heading_y && color.r == other.color.r &&
		color.g == other.color.g && color.b == other.color.b);
}

void Boid::clicked(int click_x, int click_y) {
	float dif_x = x - (float)click_x;
	float dif_y = y - (float)click_y;
	float dist = sqrt(pow(dif_x, 2) + pow(dif_y, 2));

	//printf("%f %f; %f %f; %f %f %f\n", x, y, (float)click_x, (float)click_y, dif_x, dif_y, dist);

	//printf("%f %f %f\n", heading_x, heading_y, velocity);
	heading_x = 2.0 * dif_x / dist;
	heading_y = 2.0 * dif_y / dist;
	velocity = MAX_VELOCITY * (1.0 - dist / MAX_DIST) ;
	//velocity = ESCAPE_VELOCITY * dist / pow(abs(BOTTOM),2) ;
	//printf("%f %f %f\n", heading_x, heading_y, velocity);
}
