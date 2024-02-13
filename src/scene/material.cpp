#include "material.h"
#include "../ui/TraceUI.h"
#include "light.h"
#include "ray.h"
extern TraceUI* traceUI;

#include <glm/gtx/io.hpp>
#include <iostream>
#include "../fileio/images.h"

using namespace std;
extern bool debugMode;

Material::~Material()
{
}

// Apply the phong model to this point on the surface of the object, returning
// the color of that point.
glm::dvec3 Material::shade(Scene* scene, const ray& r, const isect& i) const
{
	// YOUR CODE HERE

	// For now, this method just returns the diffuse color of the object.
	// This gives a single matte color for every distinct surface in the
	// scene, and that's it.  Simple, but enough to get you started.
	// (It's also inconsistent with the phong model...)

	// Your mission is to fill in this method with the rest of the phong
	// shading model, including the contributions of all the light sources.
	// You will need to call both distanceAttenuation() and
	// shadowAttenuation()
	// somewhere in your code in order to compute shadows and light falloff.
	//	if( debugMode )
	//		std::cout << "Debugging Phong code..." << std::endl;

	// When you're iterating through the lights,
	// you'll want to use code that looks something
	// like this:
	//
	// for ( const auto& pLight : scene->getAllLights() )
	// {
	//              // pLight has type unique_ptr<Light>
	// 		.
	// 		.
	// 		.
	// }


	glm::dvec3 I = ke(i) + ka(i) * scene->ambient();

	glm::dvec3 Q = r.at(i.getT());

	glm::dvec3 attenuation, L, Ii, diffuse, spec, ri, R, V;

	for (const auto& pLight : scene->getAllLights()) {
		attenuation = pLight->distanceAttenuation(Q) * pLight->shadowAttenuation(r, Q);

		L = pLight->getDirection(Q);

		Ii = pLight->getColor();

		double LdotN = dot(L, i.getN());

		// kd * Ii * max(L dot N, 0)
		diffuse = kd(i) * Ii * max(LdotN, 0.0);

		// ks * Ii * max(R dot V, 0) ^ n
		ri = -1.0 * L;
		R = ri - (2.0 * i.getN() * glm::dot(ri, i.getN()));
		V = -1.0 * r.getDirection();
		double n = shininess(i);

		spec = ks(i) * Ii * pow( max(glm::dot(R, V), 0.0), n );

		I += attenuation * (diffuse + spec);
	}

	return I;
	//return kd(i);
}

TextureMap::TextureMap(string filename)
{
	data = readImage(filename.c_str(), width, height);
	if (data.empty()) {
		width = 0;
		height = 0;
		string error("Unable to load texture map '");
		error.append(filename);
		error.append("'.");
		throw TextureMapException(error);
	}
}

glm::dvec3 TextureMap::getMappedValue(const glm::dvec2& coord) const
{
	// YOUR CODE HERE
	//
	// In order to add texture mapping support to the
	// raytracer, you need to implement this function.
	// What this function should do is convert from
	// parametric space which is the unit square
	// [0, 1] x [0, 1] in 2-space to bitmap coordinates,
	// and use these to perform bilinear interpolation
	// of the values.

	/*
	* I used this site as a reference to better understand the math 
	* behind linear interpolation: https://x-engineer.org/bilinear-interpolation/ .
	*/

	// bilinear interpolation

	double u = coord.x * getWidth() - 1;
	double v = coord.y * getHeight() - 1;

	// round down
	double fu = floor(u);
	double cu = fu + 1.0;

	double fv = floor(v);
	double cv = fv + 1.0;

	glm::dvec2 upperLeft  (fu, cv); 
	glm::dvec2 upperRight (cu, cv);
	glm::dvec2 lowerLeft  (fu, fv);
	glm::dvec2 lowerRight (cu, fv);

	// x = u, y = v
	double x1 = lowerLeft.x;
	double x2 = lowerRight.x;

	double y1 = lowerLeft.y;
	double y2 = upperLeft.y;

	glm::dvec3 Q12 = getPixelAt( (int) x1, (int) y2);
	glm::dvec3 Q22 = getPixelAt( (int) x2, (int) y2);
	glm::dvec3 Q11 = getPixelAt( (int) x1, (int) y1);
	glm::dvec3 Q21 = getPixelAt( (int) x2, (int) y1);

	// generate points
	glm::dvec3 R1 = Q11 * (x2 - u) / (x2 - x1) + Q21 * (u - x1) / (x2 - x1);
	glm::dvec3 R2 = Q12 * (x2 - u) / (x2 - x1) + Q22 * (u - x1) / (x2 - x1);

	// interpolated point
	glm::dvec3 P = R1 * (y2 - v) / (y2 - y1) + R2 * (v - y1) / (y2 - y1);

	return P;

	//return glm::dvec3(1, 1, 1);
}

glm::dvec3 TextureMap::getPixelAt(int x, int y) const
{
	// YOUR CODE HERE
	//
	// In order to add texture mapping support to the
	// raytracer, you need to implement this function.

	int X = x >= width ? -x : x;
	int Y = y >= height ? -y : y;

	int pos = 3 * (y * width + x);

	glm::dvec3 pixel (data[pos], data[pos + 1], data[pos + 2]);
	pixel /= 255.0;

	return pixel;

	//return glm::dvec3(1, 1, 1);
}

glm::dvec3 MaterialParameter::value(const isect& is) const
{
	if (0 != _textureMap)
		return _textureMap->getMappedValue(is.getUVCoordinates());
	else
		return _value;
}

double MaterialParameter::intensityValue(const isect& is) const
{
	if (0 != _textureMap) {
		glm::dvec3 value(
		        _textureMap->getMappedValue(is.getUVCoordinates()));
		return (0.299 * value[0]) + (0.587 * value[1]) +
		       (0.114 * value[2]);
	} else
		return (0.299 * _value[0]) + (0.587 * _value[1]) +
		       (0.114 * _value[2]);
}