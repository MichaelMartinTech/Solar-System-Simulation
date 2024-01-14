#include "scene_node.h"
#include "graphics.h"

SceneNode::SceneNode(std::string identifier){
	this->identifier = identifier;
	this->nodeObject = nullptr;
	graph = nullptr;
	modelViewMatrix = glm::mat4(1.f);
	parent = nullptr;
	position = glm::vec3(0.f, 0.f, 0.f);
	rotation = glm::quat(glm::vec3(0.f, 0.f, 0.f));
	scale = glm::vec3(1.f, 1.f, 1.f);

	generateMatrix();
}

// Creates a node with a renderable object
SceneNode::SceneNode(std::string identifier, Object* obj){
	this->identifier = identifier;
	this->nodeObject = obj;
	graph = nullptr;
	modelViewMatrix = glm::mat4(1.f);
	parent = nullptr;
	position = glm::vec3(0.f, 0.f, 0.f);
	rotation = glm::quat(glm::vec3(0.f, 0.f, 0.f));
	scale = glm::vec3(1.f, 1.f, 1.f);

	generateMatrix();
}

SceneNode::~SceneNode(){
	if (nodeObject != nullptr) {
		delete nodeObject;
		nodeObject = nullptr;
	}
}

void SceneNode::setParent(SceneNode* parentNode) { 
	if (parent != nullptr) {
		parent->removeChild(identifier);
	}

	parent = parentNode;
}
const SceneNode* SceneNode::getParent() const { return parent; }

void SceneNode::setPosition(glm::vec3 position) {
	this->position = position;
	generateMatrix();
}

void SceneNode::setRotation(glm::quat euler) {
	this->rotation = euler;
	generateMatrix();
}

void SceneNode::setScale(glm::vec3 scale) {
	this->scale = scale;
	generateMatrix();
}

void SceneNode::setLocalPosition(glm::vec3 position){
	this->position = position;
	generateMatrix();
}

void SceneNode::setLocalRotation(glm::quat euler){
	this->rotation = euler;

	generateMatrix();
}

void SceneNode::setLocalScale(glm::vec3 scale) {
	this->scale = scale;
	generateMatrix();
}

const glm::vec3 SceneNode::getLocalPosition() const
{
	return position;
}

const glm::quat SceneNode::getLocalRotation() const
{
	return rotation;
}

const glm::vec3 SceneNode::getLocalScale() const
{
	return scale;
}

const glm::mat4 SceneNode::getWorldMatrix() const
{
	if (parent == nullptr)
		return modelViewMatrix;

	return parent->getWorldMatrix() * modelViewMatrix;
}

glm::vec3 SceneNode::getPosition() const
{
	if(parent == nullptr)
		return position;

	return glm::vec3(getWorldMatrix()[3]);
}

glm::quat SceneNode::getRotation() const
{
	if (parent == nullptr)
		return rotation;

	return parent->getRotation() * rotation;
}

glm::vec3 SceneNode::getScale() const
{
	if (parent == nullptr)
		return scale;

	return parent->getScale() * scale;
}

const glm::vec3 SceneNode::getForwardVector() const
{
	return glm::rotate(getRotation(), glm::vec3(0, 0, -1));
}

const glm::vec3 SceneNode::getRightVector() const
{
	return glm::rotate(getRotation(), glm::vec3(1, 0, 0));
}

const glm::vec3 SceneNode::getUpVector() const
{
	return glm::rotate(getRotation(), glm::vec3(0, 1, 0));
}

void SceneNode::setGraph(SceneGraph* graph)
{
	this->graph = graph;
}

const Object* SceneNode::getObject() const{
	return nodeObject;
}

void SceneNode::addChild(SceneNode* childNode){
	childNode->setParent(this);
	childNode->setGraph(graph);
	children.push_back(childNode);
}

bool SceneNode::addChild(const std::string& identifier, SceneNode* childNode)
{
	if(this->identifier == identifier) {
		addChild(childNode);
		return true;
	}

	for (std::vector<SceneNode*>::const_iterator iter = children.cbegin(); iter != children.cend(); ++iter) {
		if ((*iter)->addChild(identifier, childNode)) {
			return true;
		}
	}
	return false;
}

const SceneNode* SceneNode::getChild(const std::string& identifier){
	std::vector<SceneNode*>::const_iterator iter = std::find_if(children.cbegin(), children.cend(), [&identifier](SceneNode* node) { return node->identifier == identifier; });

	if (iter == children.cend()) return nullptr;

	return *iter;
}

void SceneNode::removeChild(const std::string& identifier)
{
	std::vector<SceneNode*>::const_iterator iter = std::find_if(children.cbegin(), children.cend(), [&identifier](SceneNode* node) { return node->identifier == identifier; });

	if (iter == children.cend()) return;

	children.erase(iter);
}

void SceneNode::render(){
	graph->PushMatrix(modelViewMatrix);

	if(nodeObject != nullptr) {
		glUniformMatrix4fv(Graphics::getInstance()->getModelMatrix(), 1, GL_FALSE, glm::value_ptr(graph->getModelViewMatrix()));

		if (nodeObject->hasTex()) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, nodeObject->getTextureID());

			GLuint sampler = Graphics::getInstance()->getShader()->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found\n");
			}
			glUniform1i(sampler, 0);
		}

		if (nodeObject->hasNormalMap()) {
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, nodeObject->getNormalMapID());

			GLuint normalMap = Graphics::getInstance()->getShader()->GetUniformLocation("nor");
			if (normalMap == INVALID_UNIFORM_LOCATION)
			{
				printf("normalMap Not found\n");
			}
			glUniform1i(normalMap, 1);
		}

		nodeObject->Render();
	}

	for (std::vector<SceneNode*>::const_iterator iter = children.cbegin(); iter != children.cend(); ++iter) {
		(*iter)->render();
	}

	graph->PopMatrix();
}

void SceneNode::generateMatrix()
{
	modelViewMatrix = glm::translate(glm::mat4(1.f), position) * glm::toMat4(rotation) * glm::scale(glm::mat4(1.f), scale);
}
