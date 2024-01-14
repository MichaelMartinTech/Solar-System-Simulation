#ifndef GRAPHICS_HEADERS_H
#define GRAPHICS_HEADERS_H

#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
#define GLM_ENABLE_EXPERIMENTAL 

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>

// GLM for matricies
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>

#define INVALID_UNIFORM_LOCATION 0x7fffffff

struct Vertex
{
	glm::vec3 vertex;
	glm::vec3 normal;
	glm::vec3 tangent;
	glm::vec3 bitangent;
	glm::vec2 texcoord;

	Vertex(): vertex(), normal(), texcoord(), tangent(), bitangent() {}
	Vertex(glm::vec3 v, glm::vec3 n, glm::vec2 tc, glm::vec3 t, glm::vec3 bt): vertex(v), normal(n) , texcoord(tc), tangent(t), bitangent(bt) {}
};

#endif /* GRAPHICS_HEADERS_H */
