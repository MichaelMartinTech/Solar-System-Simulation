#include "camera.h"
#include <algorithm>

Camera::Camera()
{

}

Camera::~Camera()
{

}

bool Camera::Initialize(int w, int h)
{
    //--Init the view and projection matrices
    //  if you will be having a moving camera the view matrix will need to more dynamic
    //  ...Like you should update it before you render more dynamic 
    //  for this project having them static will be fine
    //view = glm::lookAt( glm::vec3(x, y, z), //Eye Position
    //                    glm::vec3(0.0, 0.0, 0.0), //Focus point
    //                    glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up
    position = glm::vec3(25, 0, 0);
    target = glm::vec3(0);
    up = glm::vec3(0, 1, 0);
    zoom = 40;
    aspect = w / float(h);

    return true;
}


void Camera::UpdateVectors(const glm::vec3& offset) {
    this->offset = offset;
}

void Camera::Follow(const SceneNode* node)
{
    targetNode = const_cast<SceneNode*>(node);
}

void Camera::Zoom(float amount) {
    this->zoom = std::max<float>(std::min<float>(this->zoom - amount, 11.f), 8.f);
}

glm::mat4 Camera::GetProjection()
{
  return glm::perspective(glm::radians(90.f), //the FoV typically 90 degrees is good which is what this is set to
      aspect, //Aspect Ratio, so Circles stay Circular
      0.01f, //Distance to the near plane, normally a small value like this
      300000.0f); //Distance to the far plane
}

glm::mat4 Camera::GetView()
{
  return matrix;
}

void Camera::Update(double dt)
{
    if (targetNode != nullptr) {
        target = targetNode->getPosition();
        up = targetNode->getUpVector();
        position = targetNode->getPosition() + offset;

        glm::vec3 forward = glm::normalize(target - position);
        position += forward * (zoom  / 12.f);
    }
    else {
        target = glm::mix(target, t_target, dt * 20);
        position = glm::mix(position, t_position, dt * 30);
        up = glm::mix(up, t_up, dt * 30);
    }
    matrix = glm::lookAt(position, target, up);
}
