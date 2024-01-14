#include "object.h"
#include "graphics.h"

Object::Object()
{
	// Vertex Set Up
	setupVerticies();

	// Buffer Set Up
	if (!InitBuffers()) {
		printf("Some buffers not initialized.\n");
	}
}

Object::Object(const std::string& fname, const std::string& nMapname)
{
	// Vertex Set Up
	setupVerticies();

	// Buffer Set Up
	if (!InitBuffers()) {
		printf("Some buffers not initialized.\n");
	}

	m_texture = nullptr;
	m_normalMap = nullptr;

	if (fname.length() > 0) {
		m_texture = new Texture(fname.c_str());
	}

	if (nMapname.length() > 0) {
		m_normalMap = new Texture(nMapname.c_str());
	}
}

Object::~Object()
{
	Vertices.clear();
	Indices.clear();
}

void Object::Render(GLint posAttribLoc, GLint colAttribLoc)
{
	glBindVertexArray(vao);

	// Enable vertex attibute arrays for each vertex attrib
	glEnableVertexAttribArray(posAttribLoc);
	glEnableVertexAttribArray(colAttribLoc);

	// Bind your VBO
	glBindBuffer(GL_ARRAY_BUFFER, VB);

	// Set vertex attribute pointers to the load correct data
	glVertexAttribPointer(posAttribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(colAttribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glVertexAttribPointer(colAttribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcoord));
	glVertexAttribPointer(colAttribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
	glVertexAttribPointer(colAttribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

	// Bind your Element Array
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

	// Render
	glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

	// Disable vertex arrays
	glDisableVertexAttribArray(posAttribLoc);
	glDisableVertexAttribArray(colAttribLoc);
}

void Object::Render() {
	glBindVertexArray(vao);
	Graphics* graphics = Graphics::getInstance();
	// Enable vertex attibute arrays for each vertex attrib
	glEnableVertexAttribArray(graphics->getPositionAttribute());
	glEnableVertexAttribArray(graphics->getNormalAttribute());
	glEnableVertexAttribArray(graphics->getTextCoordAttribute());

	// Bind your VBO
	glBindBuffer(GL_ARRAY_BUFFER, VB);

	// Set vertex attribute pointers to the load correct data. Update here to load the correct attributes.
	glVertexAttribPointer(graphics->getPositionAttribute(), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(graphics->getNormalAttribute(), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glVertexAttribPointer(graphics->getTextCoordAttribute(), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcoord));

	// If has texture, set up texture unit(s): update here for texture rendering

	glUniform1i(graphics->getHasTextureAttribute(), hasTex());
	glUniform1i(graphics->getIsEmissiveAttribute(), false);

	// Bind your Element Array
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

	// Render
	glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

	// Disable vertex arrays
	glDisableVertexAttribArray(graphics->getPositionAttribute());
	glDisableVertexAttribArray(graphics->getNormalAttribute());
	glDisableVertexAttribArray(graphics->getTextCoordAttribute());
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

bool Object::InitBuffers() {

	// For OpenGL 3
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

	return true;
}

void Object::setupVerticies() {
	Vertices = {
	  {{1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 0.0f}, {0,0.}, {1.,1.,1.}, {1.,1.,1.}},
	  {{1.0f, -1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {1.,0.}, {1.,1.,1.}, {1.,1.,1.}},
	  {{-1.0f, -1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {1.,1.}, {1.,1.,1.}, {1.,1.,1.}},
	  {{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 1.0f}, {0.,1.}, {1.,1.,1.}, {1.,1.,1.}},
	  {{1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 0.0f}, {0.,0.}, {1.,1.,1.}, {1.,1.,1.}},
	  {{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 1.0f}, {1.,0.}, {1.,1.,1.}, {1.,1.,1.}},
	  {{-1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}, {1.,1.}, {1.,1.,1.}, {1.,1.,1.}},
	  {{-1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, {0.,0.}, {1.,1.,1.}, {1.,1.,1.}}
	};

	Indices = {
	  1, 2, 3,
	  7, 6, 5,
	  0, 4, 5,
	  1, 5, 6,
	  6, 7, 3,
	  0, 3, 7,
	  0, 1, 3,
	  4, 7, 5,
	  1, 0, 5,
	  2, 1, 6,
	  2, 6, 3,
	  4, 0, 7
	};
}

bool Object::hasTex() {
	return m_texture != nullptr;
}

bool Object::hasNormalMap()
{
	return m_normalMap != nullptr;
}

GLuint Object::getTextureID() {
	return m_texture->getTextureID();
}

GLuint Object::getNormalMapID()
{
	return m_normalMap->getTextureID();
}

bool Object::getIsEmissive() const
{
	return isEmissive;
}
void Object::setIsEmissive(bool newValue)
{
	isEmissive = newValue;
}
