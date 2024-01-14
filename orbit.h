#pragma once

#include "graphics_headers.h"
#include "scene_node.h"

class Orbit
{
public:
	Orbit(SceneNode*target, float A, float B, float speed, float spin);
	~Orbit();

	void Update(double delta);

private:
	float pointA, pointB;
	float speed, spin, progress;

	SceneNode*target;
};

