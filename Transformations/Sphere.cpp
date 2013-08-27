#include "Sphere.h"

//Sphere creation code based upon sample given by Ed Angel in the class textbook "Interactive Computer Graphics"
void createSphere(vec4 *sphereBuffer, int start, int &end, vec4 trans, vec4 *colors, vec4 color) {
	const float DegreesToRadians = M_PI / 180.0;
	int k = start + 1;
	for (float phi = -80.0; phi <= 80.0; phi += 25.0) {
		float phir = phi * DegreesToRadians;
		float phir20 = (phi + 25.0) * DegreesToRadians;
		for (float theta = -180.0; theta <= 180.0; theta += 25.0) {
			float thetar = theta * DegreesToRadians;
			sphereBuffer[k] = vec4(sin(thetar) * cos(phir), cos(thetar) * cos(
					phir), sin(phir), 1);
			sphereBuffer[k] = Translate(trans) * Scale(.5, .5, .5) * sphereBuffer[k];
			colors[k] = color;
			k++;
			sphereBuffer[k] = vec4(sin(thetar) * cos(phir20), cos(thetar) * cos(
					phir20), sin(phir20), 1);
			sphereBuffer[k] = Translate(trans) * Scale(.5, .5, .5) * sphereBuffer[k];
			colors[k] = color;
			k++;
		}
	}
	sphereBuffer[start] = sphereBuffer[start + 1];
	sphereBuffer[k] = sphereBuffer[k-1];
	end = k + 1;
}
