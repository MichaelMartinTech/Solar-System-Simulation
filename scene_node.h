#pragma once

#include "graphics_headers.h"
#include "object.h"
#include "scene_graph.h"
#include <string>
#include <vector>

extern class SceneGraph;

class SceneNode
{
public:
	// Creates an empty node
	SceneNode(std::string identifier);

	// Creates a node with a renderable object
	SceneNode(std::string identifier, Object* obj);
	~SceneNode();

	void setPosition(glm::vec3 position);
	void setRotation(glm::quat euler);
	void setScale(glm::vec3 scale);

	void setLocalPosition(glm::vec3 position);
	void setLocalRotation(glm::quat euler);
	void setLocalScale(glm::vec3 scale);

	const glm::vec3 getLocalPosition() const;
	const glm::quat getLocalRotation() const;
	const glm::vec3 getLocalScale() const;

	const glm::mat4 getWorldMatrix() const;

	glm::vec3 getPosition() const;
	glm::quat getRotation() const;
	glm::vec3 getScale() const;

	const glm::vec3 getForwardVector() const;
	const glm::vec3 getRightVector() const;
	const glm::vec3 getUpVector() const;

	void setGraph(SceneGraph* graph);

	void setParent(SceneNode* parentNode);
	const SceneNode* getParent() const;

	const Object* getObject() const;

	void addChild(SceneNode* childNode);
	bool addChild(const std::string& identifier, SceneNode* childNode);
	const SceneNode* getChild(const std::string& identifier);

	void removeChild(const std::string& identifier);

	void render();

private:
	void generateMatrix();

	std::string identifier;
	SceneGraph* graph;
	SceneNode* parent;

	glm::mat4 modelViewMatrix;
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;

	Object* nodeObject;
	std::vector<SceneNode*> children;
};

