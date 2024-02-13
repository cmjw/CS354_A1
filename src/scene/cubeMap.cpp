#include "cubeMap.h"
#include "ray.h"
#include "../ui/TraceUI.h"
#include "../scene/material.h"
extern TraceUI* traceUI;

glm::dvec3 CubeMap::getColor(ray r) const
{
	// YOUR CODE HERE
	// FIXME: Implement Cube Map here

	glm::dvec3 d = r.getDirection();

	double x = d.x;
	double y = d.y;
	double z = d.z;

	double absX = fabs(x);
	double absY = fabs(y);
	double absZ = fabs(z);

	double maxAxis;
	double u;
	double v;
	int index;

	if (x > 0 && absX >= absY && absX >= absZ) {
		u = -z;
		v = y;
		maxAxis = absX;
		index = 0;
	} 
	else if (x <= 0 && absX >= absY && absX >= absZ) {
		u = z;
		v = y;
		maxAxis = absX;
		index = 1;
	}
	else if (y > 0 && absY >= absX && absY >= absZ) {
		u = x;
		v = -z;
		maxAxis = y;
		index = 2;
	} 
	else if (y <= 0 && absY >= absX && absY >= absZ) {
		u = x;
		v = z;
		maxAxis = y;
		index = 3;
	} 
	else if (z > 0 && absZ >= absX && absZ >= absY) {
		u = x;
		v = y;
		maxAxis = z;
		index = 4;
	} 
	else if (z <= 0 && absZ >= absX && absZ >= absY) {
		u = -x;
		v = y;
		maxAxis = z;
		index = 5;
	}

	// convert into (0, 1) range
	u = 0.5 * (u / maxAxis + 1);
	v = 0.5 * (v / maxAxis + 1);

	return tMap[index]->getMappedValue(glm::dvec2(u,v));

	return glm::dvec3();
}

CubeMap::CubeMap()
{
}

CubeMap::~CubeMap()
{
}

void CubeMap::setNthMap(int n, TextureMap* m)
{
	if (m != tMap[n].get())
		tMap[n].reset(m);
}