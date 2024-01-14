#define _USE_MATH_DEFINES

#include "graphics.h"
#include <cmath>

Graphics* Graphics::instance = nullptr;

Graphics::Graphics()
{
	graph = new SceneGraph();
}

Graphics::~Graphics()
{
	if (graph != nullptr) {
		delete graph;
		graph = nullptr;
	}

	if (m_camera != nullptr) {
		delete m_camera;
		m_camera = nullptr;
	}

	if (m_shader != nullptr) {
		delete m_shader;
		m_shader = nullptr;
	}
}


SceneNode* Graphics::AddObject(const std::string& identifier, Object* obj) {
	SceneNode* node = new SceneNode(identifier, obj);
	graph->AddNode(node);
	return node;
}

bool Graphics::Initialize(int width, int height)
{
	// Used for the linux OS
#if !defined(__APPLE__) && !defined(MACOSX)
  // cout << glewGetString(GLEW_VERSION) << endl;
	glewExperimental = GL_TRUE;

	auto status = glewInit();

	// This is here to grab the error that comes from glew init.
	// This error is an GL_INVALID_ENUM that has no effects on the performance
	glGetError();

	//Check for error
	if (status != GLEW_OK)
	{
		std::cerr << "GLEW Error: " << glewGetErrorString(status) << "\n";
		return false;
	}
#endif



	// Init Camera
	m_camera = new Camera();
	if (!m_camera->Initialize(width, height))
	{
		printf("Camera Failed to Initialize\n");
		return false;
	}

	// Set up the shaders
	m_shader = new Shader();
	if (!m_shader->Initialize())
	{
		printf("Shader Failed to Initialize\n");
		return false;
	}

	// Add the vertex shader
	if (!m_shader->AddShader(GL_VERTEX_SHADER))
	{
		printf("Vertex Shader failed to Initialize\n");
		return false;
	}

	// Add the fragment shader
	if (!m_shader->AddShader(GL_FRAGMENT_SHADER))
	{
		printf("Fragment Shader failed to Initialize\n");
		return false;
	}

	// Connect the program
	if (!m_shader->Finalize())
	{
		printf("Program to Finalize\n");
		return false;
	}

	// Populate location bindings of the shader uniform/attribs
	if (!collectShPrLocs()) {
		printf("Some shader attribs not located!\n");
	}

	//enable depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	return true;
}

void Graphics::Render()
{
	//clear the screen
	glClearColor(0.5, 0.2, 0.2, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Start the correct program
	m_shader->Enable();

	// Send in the projection and view to the shader (stay the same while camera intrinsic(perspective) and extrinsic (view) parameters are the same
	glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection()));
	glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView()));
	glUniform3f(m_sunPosition, 0, 0, 0);
	//glUniform3f(m_ambient, .2f, .2f, .2f);

	// Render the objects
	/*if (m_cube != NULL){
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_cube->GetModel()));
		m_cube->Render(m_positionAttrib,m_colorAttrib);
	}*/

	glm::mat4 modelViewMatrix = glm::mat4(1.f);
	glm::mat4 oldModelViewMatrix = glm::mat4(1.f);

	/*if (m_pyramid != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_pyramid->GetModel()));
		m_pyramid->Render(m_positionAttrib, m_colorAttrib);
	}*/

	if (graph != nullptr) {
		graph->Render();
	}

	// Get any errors from OpenGL
	auto error = glGetError();
	if (error != GL_NO_ERROR)
	{
		string val = ErrorString(error);
		std::cout << "Error initializing OpenGL! " << error << ", " << val << std::endl;
	}
}


bool Graphics::collectShPrLocs() {
	bool anyProblem = true;
	// Locate the projection matrix in the shader
	m_projectionMatrix = m_shader->GetUniformLocation("projectionMatrix");
	if (m_projectionMatrix == INVALID_UNIFORM_LOCATION)
	{
		printf("m_projectionMatrix not found\n");
		anyProblem = false;
	}

	// Locate the view matrix in the shader
	m_viewMatrix = m_shader->GetUniformLocation("viewMatrix");
	if (m_viewMatrix == INVALID_UNIFORM_LOCATION)
	{
		printf("m_viewMatrix not found\n");
		anyProblem = false;
	}

	// Locate the model matrix in the shader
	m_modelMatrix = m_shader->GetUniformLocation("modelMatrix");
	if (m_modelMatrix == INVALID_UNIFORM_LOCATION)
	{
		printf("m_modelMatrix not found\n");
		anyProblem = false;
	}

	// Locate the position vertex attribute
	m_positionAttrib = m_shader->GetAttribLocation("position");
	if (m_positionAttrib == -1)
	{
		printf("v_position attribute not found\n");
		anyProblem = false;
	}

	// Locate the color texture coordinate attribute
	m_normalAttrib = m_shader->GetAttribLocation("normal");
	if (m_normalAttrib == -1)
	{
		printf("normal attribute not found\n");
		anyProblem = false;
	}

	// Locate the color texture coordinate attribute
	m_tangentAttrib = m_shader->GetAttribLocation("tangent");
	if (m_tangentAttrib == -1)
	{
		printf("tangent attribute not found\n");
		anyProblem = false;
	}

	// Locate the color texture coordinate attribute
	m_bitangentAttrib = m_shader->GetAttribLocation("bitangent");
	if (m_bitangentAttrib == -1)
	{
		printf("bitangent attribute not found\n");
		anyProblem = false;
	}

	// Locate the color texture coordinate attribute
	m_tcAttrib = m_shader->GetAttribLocation("textcoord");
	if (m_tcAttrib == -1)
	{
		printf("v_texcoord attribute not found\n");
		anyProblem = false;
	}

	m_hasTexture = m_shader->GetUniformLocation("hasTexture");
	if (m_hasTexture == INVALID_UNIFORM_LOCATION) {
		printf("hasTexture uniform not found\n");
		anyProblem = false;
	}

	m_isEmissive = m_shader->GetUniformLocation("isEmissive");
	if (m_isEmissive == INVALID_UNIFORM_LOCATION) {
		printf("isEmissive uniform not found\n");
		anyProblem = false;
	}

	m_hasNormal = m_shader->GetUniformLocation("hasNormal");
	if (m_hasNormal == INVALID_UNIFORM_LOCATION) {
		printf("hasNormal uniform not found\n");
		anyProblem = false;
	}

	m_sunPosition = m_shader->GetUniformLocation("sunPosition");
	if (m_sunPosition == INVALID_UNIFORM_LOCATION) {
		printf("sunPosition uniform not found\n");
		anyProblem = false;
	}

	return anyProblem;
}

std::string Graphics::ErrorString(GLenum error)
{
	if (error == GL_INVALID_ENUM)
	{
		return "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument.";
	}

	else if (error == GL_INVALID_VALUE)
	{
		return "GL_INVALID_VALUE: A numeric argument is out of range.";
	}

	else if (error == GL_INVALID_OPERATION)
	{
		return "GL_INVALID_OPERATION: The specified operation is not allowed in the current state.";
	}

	else if (error == GL_INVALID_FRAMEBUFFER_OPERATION)
	{
		return "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete.";
	}

	else if (error == GL_OUT_OF_MEMORY)
	{
		return "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command.";
	}
	else
	{
		return "None";
	}
}


GLint Graphics::getPositionAttribute() {
	return m_positionAttrib;
}
GLint Graphics::getNormalAttribute() {
	return m_normalAttrib;
}
GLint Graphics::getTangentAttribute() {
	return m_tangentAttrib;
}
GLint Graphics::getBitangentAttribute() {
	return m_bitangentAttrib;
}
GLint Graphics::getTextCoordAttribute() {
	return m_tcAttrib;
}

GLint Graphics::getHasNormalAttribute() {
	return m_hasNormal;
}
GLint Graphics::getHasTextureAttribute() {
	return m_hasTexture;
}
GLint Graphics::getIsEmissiveAttribute() {
	return m_isEmissive;
}

