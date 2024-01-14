#pragma once

#include "scene_node.h"
#include <vector>

extern class SceneNode;

class SceneGraph
{
public:
	SceneGraph();

	void PushMatrix(const glm::mat4 &matrix);
	void PopMatrix();

	const glm::mat4 getModelViewMatrix() const;

	void Render();

	const SceneNode* getNode(const std::string& identifier) const;

	void AddNode(SceneNode* childNode);
	void Addnode(const std::string& identifier, SceneNode* childNode);

private:
	SceneNode* root;
	glm::mat4 currentMatrix;
	std::vector<glm::mat4> matrixStack;
};

