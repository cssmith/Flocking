#ifndef CONSTANTS_H
#define CONSTANTS_H

const float PI = 3.14159;

// The size of the coordinate space 
const float BOTTOM = -400;
const float TOP = 400;
const float LEFT = -400;
const float RIGHT = 400;

const float MAX_DIST = sqrt(pow(BOTTOM-TOP, 2) + pow(LEFT-RIGHT,2));

// Maximum boid size and speed
const float MAX_SIZE = 2;
const float MAX_VELOCITY = 35;

// Size of the neighborhood influencing each boid
const int HOOD_SIZE = 3;

// The instinctual heading the boids want to follow
const float instinct_x = -1.0;
const float instinct_y = -1.0;

// How much peers influence the boid's motion
const float PEER_INFLUENCE = 0.5;
// How much randomness influences the boid's motion
const float RANDOMNESS = 0.1;
// How much instincts influence the boid's motion
// Even when this is really low (0.01), the effect is
// compounded by influencing all the boids and the "simulation"
// is no fun...
const float INSTINCT_INFLUENCE = 0.00;
// How much centering to the neighbor's positions influences motion
const float CENTER_INFLUENCE = 0.00;


// Number of segments to use when drawing boids
const int NUM_CIRCLE_SEGMENTS = 30;

class Color {
public:
	float r, g, b;
	Color() {
		r = 1.0; g = 1.0; b = 1.0;
	}

	Color(float new_r, float new_g, float new_b) {
		r = new_r; g = new_g; b = new_b;
	}
};

#endif CONSTANTS_H