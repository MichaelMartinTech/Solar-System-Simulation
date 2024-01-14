#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
#include <stack>
using namespace std;

#include "graphics_headers.h"
#include "camera.h"
#include "shader.h"
#include "scene_graph.h"

#define numVBOs 2;
#define numIBs 2;


class Graphics
{
  public:
    ~Graphics();
    bool Initialize(int width, int height);
    void Render();

    SceneNode* AddObject(const std::string& identifier, Object* obj);

    Camera* getCamera() { return m_camera; }
    Shader* getShader() { return m_shader; }
    GLint getModelMatrix() { return m_modelMatrix; }

    GLint getPositionAttribute();
    GLint getNormalAttribute();
    GLint getTangentAttribute();
    GLint getBitangentAttribute();
    GLint getTextCoordAttribute();
    GLint getHasNormalAttribute();
    GLint getHasTextureAttribute();
    GLint getIsEmissiveAttribute();

    static Graphics* getInstance() {
        if (instance == nullptr) {
            instance = new Graphics();
        }

        return instance;
    }

  private:
    Graphics();

    static Graphics* instance;

    std::string ErrorString(GLenum error);

    bool collectShPrLocs();

    Camera *m_camera;
    Shader *m_shader;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;
    GLint m_sunPos;
    GLint m_sunPosition;
    GLint m_positionAttrib;
    GLint m_normalAttrib;
    GLint m_tangentAttrib;
    GLint m_bitangentAttrib;
    GLint m_colorAttrib;
    GLint m_tcAttrib;
    GLint m_isEmissive;
    GLint m_hasTexture;
    GLint m_hasNormal;

    SceneGraph* graph;
};

#endif /* GRAPHICS_H */
