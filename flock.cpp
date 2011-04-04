#include <stdlib.h>
#include <stdio.h>
#include <gl/glut.h>
#include <math.h>
#include <vector>
#include "boid.h"
#include "constants.h"

std::vector<Boid> boids;

std::vector<Boid> getNeighbors(Boid b) {
	std::vector<Boid> neighbors;
	std::vector<Boid> the_boids(boids);

	while (neighbors.size() < HOOD_SIZE) {
		float min_dist = pow(abs(BOTTOM) + abs(TOP), 2);
		float min_index = 0;
		for (int i=0; i < the_boids.size(); i++) {
			if (! b.compareTo(the_boids.at(i))) {
				float dist = b.distanceTo(the_boids.at(i));
				if (dist < min_dist) {
					min_dist = dist;
					min_index = i;
				}
			}
		}
		neighbors.push_back(the_boids.at(min_index));
		
		the_boids.erase(the_boids.begin() + (min_index));
	}

	return neighbors;
}

Boid randomBoid() {
	float randNum = (float)rand() / (float)RAND_MAX; 
	float x = randNum * (float)(abs(LEFT)+abs(RIGHT)) - (float) RIGHT;
	randNum = (float)rand() / (float)RAND_MAX; 
	float y = randNum * (float)(abs(TOP)+abs(BOTTOM)) - (float) TOP;
	randNum = (float)rand() / (float)RAND_MAX; 
	//float size = randNum * MAX_SIZE;
	float size = MAX_SIZE;
	randNum = (float)rand() / (float)RAND_MAX; 
	float vel = randNum * MAX_VELOCITY;
	float vel_x = (float)rand() / (float)RAND_MAX; 
	float vel_y = (float)rand() / (float)RAND_MAX; 
	Color c ((float)rand() / (float)RAND_MAX, 
			 (float)rand() / (float)RAND_MAX, 
			 (float)rand() / (float)RAND_MAX);
	return Boid(x, y, size, vel, vel_x, vel_y, c);
}

void renderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set the camera
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(BOTTOM, TOP, LEFT, RIGHT, 20.0, 2000.0);
	glTranslatef(0.0,0.0,-20.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Update all the boid's positions
	for(int i=0; i < boids.size(); i++) {
		std::vector<Boid> neighbors = getNeighbors(boids[i]);
		boids[i].updatePosition();
		boids[i].updateDirection(neighbors);
	}

	// Display the boids
	for(int i=0; i < boids.size(); i++) {
		boids.at(i).draw();
	}

	glutSwapBuffers();
}

void handleClick(int button, int state, int x, int y) {
	if (state == GLUT_UP) {
		for(int i=0; i<boids.size(); i++) {
			boids[i].clicked(x - abs(LEFT), -1 * (y - abs(TOP)));
		}
	}
}

// Resize the scene using the projection matrix
void resize(int width, int height) {
	if(height == 0)
		height = 1;
	float ratio = 1.0* width / height;
	
	// Use the projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, width, height);
	gluPerspective(45, ratio, 0, 1);

	// Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

void main(int argc, char** argv) {

	// Initialize GLUT and display a window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(abs(BOTTOM)+abs(TOP), abs(LEFT)+abs(RIGHT));
	glutCreateWindow("Flocking demo");

	// Callback to display scene
	glutDisplayFunc(renderScene);
	// Callback to resize scene
	glutReshapeFunc(resize);
	// Callback for idling
	glutIdleFunc(renderScene);
	// Callback for mouse click
	glutMouseFunc(handleClick);

	// Add some boids to the scene
	int numBoids = 100;
	if (argc == 2) {
		numBoids = atoi(argv[1]);
	}

	srand(10);

	for (int i=0; i < numBoids; i++) {
		boids.push_back(randomBoid());
	}

	glutMainLoop();
}