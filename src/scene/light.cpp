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

	// 4th arg?
	ray originToDirectionalLight = ray(p, getDirection(p), glm::dvec3(1, 1, 1), ray::SHADOW);

	isect i;

	// ray from origin to light intersects an object
	if (scene->intersect(originToDirectionalLight, i)) {
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
	return 1.0;
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

	//ray d = glm::normalize((r.getPosition() - p));

	ray originToDirectionalLight = ray(p, getDirection(p), glm::dvec3(1, 1, 1), ray::SHADOW);
	isect i;

	// THERE IS SOMETHING WRONG HERE
	// if (scene->intersect(originToDirectionalLight, i)) {
	// 	return glm::dvec3(0.0, 0.0, 0.0);
	// }	

	return glm::dvec3(1.0, 1.0, 1.0);
}

#define VERBOSE 0

