#include "object.h"
#include "Texture.h"

class Sphere :
    public Object
{
public:
    Sphere();


    virtual void Render(GLint positionAttribLoc, GLint colorAttribLoc);
    virtual void Render();

    Sphere(int prec);
    Sphere(int prec, const char* fname, const char* normalMap);

    int getNumVertices() const;
    int getNumIndices() const;

    std::vector<int> getIndices();
    std::vector<glm::vec3> getVertices();
    std::vector<glm::vec2> getTexCoords();
    std::vector<glm::vec3> getNormals();


private:
    glm::vec3 pivotLocation;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;

    GLuint vao;

    void setupVertices();
    void setupBuffers();

    int numVertices;
    int numIndices;
    std::vector<int> indices;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> normals;
    void init(int);
    float toRadians(float degrees);

    std::vector<float> pvalues; // vertex positions
    std::vector<float> tvalues; // texture coordinates
    std::vector<float> nvalues; // normal vectors
};