#include "scene_graph.h"

SceneGraph::SceneGraph()
{
	matrixStack.reserve(10);
	currentMatrix = glm::mat4(1.f);
	matrixStack.push_back(currentMatrix);
}

void SceneGraph::PushMatrix(const glm::mat4& matrix)
{
	currentMatrix *= matrix;
	matrixStack.push_back(currentMatrix);
}

void SceneGraph::PopMatrix()
{
	if (matrixStack.size() > 1) {
		matrixStack.pop_back();
		currentMatrix = matrixStack.back();
	}
}

const glm::mat4 SceneGraph::getModelViewMatrix() const
{
	return currentMatrix;
}

void SceneGraph::Render()
{
	if (root != nullptr) {
		root->render();
	}
}

const SceneNode* SceneGraph::getNode(const std::string& identifier) const
{
	if (root == nullptr) return nullptr;

	return root->getChild(identifier);
}

void SceneGraph::AddNode(SceneNode* childNode)
{
	if (root == nullptr) {
		root = new SceneNode("root");
		root->setGraph(this);
	}

	root->addChild(childNode);
}

void SceneGraph::Addnode(const std::string& identifier, SceneNode* childNode)
{
	if (root == nullptr) {
		root = new SceneNode("root");
		root->setGraph(this);
	}

	root->addChild(identifier, childNode);
}
