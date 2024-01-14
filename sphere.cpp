#include "sphere.h"
#include "graphics.h"
#include <map>

Sphere::Sphere()
{
    init(48);
    setupVertices();
    setupBuffers();
}

Sphere::Sphere(int prec) { // prec is precision, or number of slices

    init(prec);
    setupVertices();
    setupBuffers();
}

Sphere::Sphere(int prec, const char* fname, const char* normalMap) : Object(fname, normalMap) { // prec is precision, or number of slices

    init(prec);
    setupVertices();
    setupBuffers();
}


void Sphere::Render(GLint positionAttribLoc, GLint colorAttribLoc)
{
    //glBindVertexArray(vao);
    // Enable Vertext Attributes
    glEnableVertexAttribArray(positionAttribLoc);
    glEnableVertexAttribArray(colorAttribLoc);

    // Bind your VBO buffer(s) and then setup vertex attribute pointers
    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glVertexAttribPointer(positionAttribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(colorAttribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
 

    // Bind your index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

    // Render
    glDrawArrays(GL_TRIANGLES, 0, getNumIndices());

    // Disable Vertex Attribuates
    glDisableVertexAttribArray(positionAttribLoc);
    glDisableVertexAttribArray(colorAttribLoc);
}

void Sphere::Render()
{
    glBindVertexArray(vao);

    Graphics* graphics = Graphics::getInstance();
    // Enable vertex attibute arrays for each vertex attrib
    glEnableVertexAttribArray(graphics->getPositionAttribute());
    glEnableVertexAttribArray(graphics->getNormalAttribute());
    glEnableVertexAttribArray(graphics->getTangentAttribute());
    glEnableVertexAttribArray(graphics->getBitangentAttribute());
    glEnableVertexAttribArray(graphics->getTextCoordAttribute());

    // Bind your VBO
    glBindBuffer(GL_ARRAY_BUFFER, VB);

    // Set vertex attribute pointers to the load correct data. Update here to load the correct attributes.
    glVertexAttribPointer(graphics->getPositionAttribute(), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(graphics->getNormalAttribute(), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glVertexAttribPointer(graphics->getTangentAttribute(), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
    glVertexAttribPointer(graphics->getBitangentAttribute(), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));
    glVertexAttribPointer(graphics->getTextCoordAttribute(), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcoord));

    // If has texture, set up texture unit(s): update here for texture rendering

    glUniform1i(graphics->getHasTextureAttribute(), hasTex());
    glUniform1i(graphics->getHasNormalAttribute(), hasNormalMap());
    glUniform1i(graphics->getIsEmissiveAttribute(), getIsEmissive());

    // Bind your Element Array
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

    // Render
    glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

    // Disable vertex arrays
    glDisableVertexAttribArray(graphics->getPositionAttribute());
    glDisableVertexAttribArray(graphics->getNormalAttribute());
    glDisableVertexAttribArray(graphics->getTangentAttribute());
    glDisableVertexAttribArray(graphics->getBitangentAttribute());
    glDisableVertexAttribArray(graphics->getTextCoordAttribute());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void Sphere::setupVertices() {
    std::vector<int> ind = getIndices();
    std::vector<glm::vec3> vert = getVertices();
    std::vector<glm::vec2> tex = getTexCoords();
    std::vector<glm::vec3> norm = getNormals();

    std::map<int, Vertex> vertTemp;

    int numIndices = getNumIndices();
    for (int i = 0; i < numIndices; i+=3) {
        glm::vec3 positions[3];
        glm::vec3 normals[3];
        glm::vec2 textCoords[3];


        for (int k = 0; k < 3; k++) {
            int index = ind.at(i+k);
            positions[k] = vert[index];
            normals[k] = norm[index];
            textCoords[k] = tex[index];
        }

        glm::vec3 deltaPos1 = positions[1] - positions[0];
        glm::vec3 deltaPos2 = positions[2] - positions[0];

        glm::vec2 deltaTex1 = textCoords[1] - textCoords[0];
        glm::vec2 deltaTex2 = textCoords[2] - textCoords[0];

        float r = 1.0f / (deltaTex1.x * deltaTex2.y - deltaTex1.t * deltaTex2.x);
        glm::vec3 tangent = (deltaPos1 * deltaTex2.y - deltaPos2 * deltaTex1.y) * r;
        glm::vec3 bitangent = (deltaPos2 * deltaTex1.x - deltaPos1 * deltaTex2.x) * r;

        for (int k = 0; k < 3; k++) {
            int index = ind.at(i + k);

            // add if it doesn't or update existing
            if (!vertTemp.count(index)) {
                vertTemp.emplace(
                    index,
                    Vertex(
                        positions[k],
                        normals[k],
                        textCoords[k],
                        tangent,
                        bitangent
                    )
                );
            }
            else {
                /*vertTemp[index].tangent += tangent;
                vertTemp[index].bitangent += bitangent;*/
            }

            Indices.push_back(index);
        }
    }

    for (std::map<int, Vertex>::iterator v = vertTemp.begin(); v != vertTemp.end(); ++v) {
        Vertices.push_back(v->second);
    }

    int i = 0;
}


void Sphere::setupBuffers() {
    // For OpenGL 3
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &VB);
    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &IB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);
}

float Sphere::toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }

void Sphere::init(int prec) {
    numVertices = (prec + 1) * (prec + 1);
    numIndices = prec * prec * 6;
    for (int i = 0; i < numVertices; i++) { vertices.push_back(glm::vec3()); }
    for (int i = 0; i < numVertices; i++) { texCoords.push_back(glm::vec2()); }
    for (int i = 0; i < numVertices; i++) { normals.push_back(glm::vec3()); }
    for (int i = 0; i < numIndices; i++) { indices.push_back(0); }

    for (int i = 0; i <= prec; i++) {
        for (int j = 0; j <= prec; j++) {
            float y = (float)cos(toRadians(180.f - i * 180.f / prec));
            float x = -(float)cos(toRadians(j * 360.f / prec)) * (float)abs(cos(asin(y)));
            float z = (float)sin(toRadians(j * 360.f / prec)) * (float)abs(cos(asin(y)));
            vertices[i * (prec + 1) + j] = glm::vec3(x, y, z);
            texCoords[i * (prec + 1) + j] = glm::vec2(((float)j / prec), ((float)i / prec));
            normals[i * (prec + 1) + j] = glm::vec3(x, y, z);
        }
    }

    // calculate triangles indices
    for (int i = 0; i < prec; i++) {
        for (int j = 0; j < prec; j++) {
            indices[6 * (i * prec + j) + 0] = i * (prec + 1) + j;
            indices[6 * (i * prec + j) + 1] = i * (prec + 1) + j + 1;
            indices[6 * (i * prec + j) + 2] = (i + 1) * (prec + 1) + j;
            indices[6 * (i * prec + j) + 3] = i * (prec + 1) + j + 1;
            indices[6 * (i * prec + j) + 4] = (i + 1) * (prec + 1) + j + 1;
            indices[6 * (i * prec + j) + 5] = (i + 1) * (prec + 1) + j;
        }
    }
}

// accessors
int Sphere::getNumVertices() const { return numVertices; }
int Sphere::getNumIndices() const { return numIndices; }

std::vector<int> Sphere::getIndices() { return indices; }
std::vector<glm::vec3> Sphere::getVertices() { return vertices; }
std::vector<glm::vec2> Sphere::getTexCoords() { return texCoords; }
std::vector<glm::vec3> Sphere::getNormals() { return normals; }

