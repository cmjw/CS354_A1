#include <cmath>
#include <iostream>

#include "light.h"
#include <glm/glm.hpp>
#include <glm/gtx/io.hpp>


using namespace std;

double DirectionalLight::distanceAttenuation(const glm::dvec3& P) const
{
	// distance to light is infinite, so f(di) goes to 0.  Return 1.
	return 1.0;
}


glm::dvec3 DirectionalLight::shadowAttenuation(const ray& r, const glm::dvec3& p) const
{
	// YOUR CODE HERE:
	// You should implement shadow-handling code here.

	ray shadowRay = ray(p, getDirection(p), glm::dvec3(1, 1, 1), ray::SHADOW);
	isect i;

	if (scene->intersect(shadowRay, i)) {
		return glm::dvec3(0, 0, 0);
	}

	return glm::dvec3(1.0, 1.0, 1.0);
}

glm::dvec3 DirectionalLight::getColor() const
{
	return color;
}

glm::dvec3 DirectionalLight::getDirection(const glm::dvec3& P) const
{
	return -orientation;
}

double PointLight::distanceAttenuation(const glm::dvec3& P) const
{

	// YOUR CODE HERE

	// You'll need to modify this method to attenuate the intensity 
	// of the light based on the distance between the source and the 
	// point P.  For now, we assume no attenuation and just return 1.0

	double d = glm::distance(P, position);

	double atten = 1.0 / (constantTerm + linearTerm * d + quadraticTerm * (d * d));

	return glm::min(1.0, atten);
}

glm::dvec3 PointLight::getColor() const
{
	return color;
}

glm::dvec3 PointLight::getDirection(const glm::dvec3& P) const
{
	return glm::normalize(position - P);
}


glm::dvec3 PointLight::shadowAttenuation(const ray& r, const glm::dvec3& p) const
{
	// YOUR CODE HERE:
	// You should implement shadow-handling code here.

	//std::cout << "Point Light Shadow Attenuation" << std::endl;

	glm::dvec3 d = glm::normalize((r.getPosition() - p));
	isect i;

	ray shadowRay = ray(p, getDirection(p), glm::dvec3(1, 1, 1), ray::SHADOW);

	if (!(scene->intersect(shadowRay, i))) {
		// no intersection, no shadow
		//std::cout << "No intersection" << std::endl;

		return glm::dvec3(1, 1, 1);
	}

	glm::dvec3 Q = shadowRay.at(i.getT());

	double qLen = glm::distance(p, Q);
	double lightLen = glm::distance(p, position);

	//std::cout << "q and light" << qLen << " " << lightLen << std::endl;

	if (qLen < lightLen) {
		return glm::dvec3(0, 0, 0);
	}

	return glm::dvec3(1, 1, 1);
}

#define VERBOSE 0
