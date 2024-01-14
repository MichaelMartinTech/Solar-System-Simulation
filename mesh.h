#ifndef MESH_H
#define MESH_H

#include <vector>
#include "graphics_headers.h"
#include "object.h"
#include "Texture.h"

class Mesh : public Object
{
public:
    Mesh();
    Mesh(glm::vec3 pivot, const char* fname);
    Mesh(glm::vec3 pivot, const char* fname, const char* tname, const char* nname);

    ~Mesh();
    void Update(glm::mat4 model);
    void Render(GLint posAttrib, GLint colAttrib);
    void Render();

    glm::mat4 GetModel();

    bool InitBuffers();
    bool loadModelFromFile(const char* path);

private:
    glm::vec3 pivotLocation;
    glm::mat4 model;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;

    GLuint vao;

    float angle;
};

#endif