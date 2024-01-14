#include "orbit.h"
#include <cmath>

Orbit::Orbit(SceneNode *target, float A, float B, float speed, float spin): target(target), pointA(A), pointB(B), speed(speed), spin(1.f/spin)
{
	progress = (float)(rand() % 10000) / 10000;
}

Orbit::~Orbit()
{
}

void Orbit::Update(double delta)
{
	// x = a cos(t)
	// y = b sin(t)
	// time scale

	progress += delta / (speed * 2);
	while (progress > glm::two_pi<float>()) {
		progress -= glm::two_pi<float>();
	}

	float x = pointA * cos(progress);
	float z = pointB * sin(progress);

	target->setLocalPosition(glm::vec3(x, 0, z));
	target->setLocalRotation(glm::toQuat(glm::eulerAngleY(spin * (float)delta)) * target->getLocalRotation());
}
