#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "graphics_headers.h"
#include "Texture.h"

class Object
{
public:
    Object();
    Object(const std::string& fname, const std::string& nMap);
    ~Object();

    virtual void Render(GLint posAttrib, GLint colAttrib);
    virtual void Render();
    virtual bool hasTex();
    virtual bool hasNormalMap();
    virtual GLuint getTextureID();
    virtual GLuint getNormalMapID();
    bool getIsEmissive() const;
    void setIsEmissive(bool newValue);

protected:
    bool InitBuffers();
    void setupVerticies();

private:
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;
    Texture* m_texture;
    Texture* m_normalMap;

    GLuint vao;

    bool isEmissive;
};

#endif /* OBJECT_H */
