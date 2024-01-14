#include "mesh.h"
#include "graphics.h"
#include <map>

Mesh::Mesh()
{
	// Vertex Set Up
	// No mesh

	// Model Set Up
	angle = 0.0f;
	pivotLocation = glm::vec3(0.f, 0.f, 0.f);
	model = glm::translate(glm::mat4(1.0f), pivotLocation);

	// Buffer Set Up
	if (!InitBuffers()) {
		printf("Some buffers not initialized.\n");
	}

}

Mesh::Mesh(glm::vec3 pivot, const char* fname)
{
	// Vertex Set Up
	loadModelFromFile(fname);

	// Model Set Up
	angle = 0.0f;
	pivotLocation = pivot;
	model = glm::translate(glm::mat4(1.0f), pivotLocation);

	// Buffer Set Up
	if (!InitBuffers()) {
		printf("some buffers not initialized.\n");
	}
}

Mesh::Mesh(glm::vec3 pivot, const char* fname, const char* tname, const char* nname) : Object(tname, nname)
{
	// Vertex Set Up
	loadModelFromFile(fname);

	// Model Set Up
	angle = 0.0f;
	pivotLocation = pivot;
	model = glm::translate(glm::mat4(1.0f), pivotLocation);

	// Buffer Set Up
	if (!InitBuffers()) {
		printf("some buffers not initialized.\n");
	}
}


Mesh::~Mesh()
{
	Vertices.clear();
	Indices.clear();
}

void Mesh::Update(glm::mat4 inmodel)
{
	model = inmodel;

}

glm::mat4 Mesh::GetModel()
{
	return model;
}

void Mesh::Render(GLint posAttribLoc, GLint colAttribLoc)
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
	glVertexAttribPointer(colAttribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
	glVertexAttribPointer(colAttribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));
	glVertexAttribPointer(colAttribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcoord));

	// Bind your Element Array
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

	// Render
	glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

	// Disable vertex arrays
	glDisableVertexAttribArray(posAttribLoc);
	glDisableVertexAttribArray(colAttribLoc);
}

void Mesh::Render()
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


bool Mesh::InitBuffers() {

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

glm::vec3 fromVector(aiVector3D vector) {
	return glm::vec3(vector.x, vector.y, vector.z);
}

bool Mesh::loadModelFromFile(const char* path) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);

	if (!scene) {
		printf("couldn't open the .obj file. \n");
		return false;
	}

	const int ivertTotalSize = 2 * sizeof(aiVector3D);

	std::map<int, Vertex> vertTemp;

	int iTotalVerts = 0;

	for (int i = 0; i < scene->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[i];
		int iMeshFaces = mesh->mNumFaces;
		for (int j = 0; j < iMeshFaces; j++) {
			const aiFace& face = mesh->mFaces[j];

			glm::vec3 positions[3];
			glm::vec3 normals[3];
			glm::vec2 textCoords[3];


			for (int k = 0; k < 3; k++) {
				aiVector3D tex = mesh->mTextureCoords[0][face.mIndices[k]];
				positions[k] = fromVector(mesh->mVertices[face.mIndices[k]]);
				normals[k] = fromVector(mesh->mNormals[face.mIndices[k]]);
				textCoords[k] = glm::vec2(tex.x, tex.y);
			}

			glm::vec3 deltaPos1 = positions[1] - positions[0];
			glm::vec3 deltaPos2 = positions[2] - positions[0];

			glm::vec2 deltaTex1 = textCoords[1] - textCoords[0];
			glm::vec2 deltaTex2 = textCoords[2] - textCoords[0];

			float r = 1.0f / (deltaTex1.x * deltaTex2.y - deltaTex1.t * deltaTex2.x);
			glm::vec3 tangent = (deltaPos1 * deltaTex2.y - deltaPos2 * deltaTex1.y) * r;
			glm::vec3 bitangent = (deltaPos2 * deltaTex1.x - deltaPos1 * deltaTex2.x) * r;

			for (int k = 0; k < 3; k++) {
				// find index
				int index = face.mIndices[k];

				// add if it doesn't or update existing
				if (!vertTemp.count(index)) {
					vertTemp.emplace(index, Vertex(
						positions[k],
						normals[k],
						textCoords[k],
						tangent,
						bitangent
					));
				}
				else {
					/*vertTemp[index].tangent += tangent;
					vertTemp[index].bitangent += bitangent;*/
				}

				Indices.push_back(index);
			}
		}
		iTotalVerts += mesh->mNumVertices;
	}

	for (std::map<int, Vertex>::iterator v = vertTemp.begin(); v != vertTemp.end(); ++v) {
		Vertices.push_back(v->second);
	}
}
