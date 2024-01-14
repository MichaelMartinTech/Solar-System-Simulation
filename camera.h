#ifndef CAMERA_H
#define CAMERA_H

#include "graphics_headers.h"
#include "scene_node.h"

class Camera
{
  public:
    Camera();
    ~Camera();
    bool Initialize(int w, int h);
    glm::mat4 GetProjection();
    glm::mat4 GetView();
    void UpdateVectors(const glm::vec3& offset);
    void Follow(const SceneNode* node);
    void Update(double dt);
    void Zoom(float amount);
  
  private:
    glm::mat4 projection;
    SceneNode* targetNode;

    glm::vec3 offset;
    glm::vec3 position;
    glm::vec3 t_position;
    glm::vec3 target;
    glm::vec3 t_target;
    glm::vec3 up;
    glm::vec3 t_up;
    glm::mat4 matrix;
    float zoom;
    float aspect;
};

#endif /* CAMERA_H */
