#include "shader.h"

Shader::Shader()
{
  m_shaderProg = 0;
}

Shader::~Shader()
{
  for (std::vector<GLuint>::iterator it = m_shaderObjList.begin() ; it != m_shaderObjList.end() ; it++)
  {
    glDeleteShader(*it);
  }

  if (m_shaderProg != 0)
  {
    glDeleteProgram(m_shaderProg);
    m_shaderProg = 0;
  }
}

bool Shader::Initialize()
{
  m_shaderProg = glCreateProgram();

  if (m_shaderProg == 0) 
  {
    std::cerr << "Error creating shader program\n";
    return false;
  }

  return true;
}

// Use this method to add shaders to the program. When finished - call finalize()
bool Shader::AddShader(GLenum ShaderType)
{
  std::string s;

  if(ShaderType == GL_VERTEX_SHADER)
  {
    s = "#version 460\n \
         \
        layout(location=0) in vec3 position; \
        layout(location=1) in vec3 normal; \
        layout(location=2) in vec3 tangent; \
        layout(location=3) in vec3 bitangent; \
        layout(location=4) in vec2 textcoord; \
             \
        out vec3 position_worldSpace;  \
        out vec3 normal_cameraSpace;  \
        out vec3 eyeDirection_cameraSpace;  \
        out vec3 lightDirection_cameraSpace;  \
        out vec2 tc; \
        out mat3 tbnMatrix;  \
         \
        uniform mat4 projectionMatrix;  \
        uniform mat4 viewMatrix;  \
        uniform mat4 modelMatrix;  \
         \
        void main(void)  \
        {  \
            gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * vec4(position, 1);  \
         \
            position_worldSpace = (modelMatrix * vec4(position, 1)).xyz;  \
         \
            vec3 vertexPosition_cameraSpace = (viewMatrix * modelMatrix * vec4(position, 1)).xyz;  \
            eyeDirection_cameraSpace = vec3(0,0,0) - vertexPosition_cameraSpace;  \
         \
            vec3 lightPosition_cameraSpace = (viewMatrix * vec4(0,0,0, 1)).xyz;  \
            lightDirection_cameraSpace = lightPosition_cameraSpace + eyeDirection_cameraSpace;  \
         \
            mat4 VM = viewMatrix * modelMatrix; \
         \
            normal_cameraSpace = (VM * normalize(vec4(normal, 0))).xyz;  \
            vec3 tangent_cameraSpace = (VM * normalize(vec4(tangent, 0))).xyz;  \
            vec3 bitangent_cameraSpace = (VM * normalize(vec4(bitangent, 0))).xyz;  \
            tbnMatrix = transpose(mat3( \
                tangent_cameraSpace, \
                bitangent_cameraSpace, \
                normal_cameraSpace \
            )); \
         \
            tc = textcoord; \
        }";
  }
  else if(ShaderType == GL_FRAGMENT_SHADER)
  {
      s = "#version 460\n \
         \
        in vec3 position_worldSpace;  \
        in vec3 normal_cameraSpace;  \
        in vec3 eyeDirection_cameraSpace;  \
        in vec3 lightDirection_cameraSpace;  \
        in vec2 tc; \
        in mat3 tbnMatrix;  \
         \
        uniform sampler2D sp; \
        uniform sampler2D nor; \
        uniform bool hasTexture; \
        uniform bool hasNormal; \
        uniform bool isEmissive; \
        uniform mat4 projectionMatrix;  \
        uniform mat4 viewMatrix;  \
        uniform mat4 modelMatrix;  \
         \
        out vec4 frag_color;  \
         \
        void main(void)  \
        { \
	        vec3 diffuseColor = vec3(1); \
	        vec3 specularColor = vec3(1); \
	        vec3 lightColor = vec3(1); \
	        float lightPower = 200.0f; \
	        float specularPower = 1.0f; \
	         \
	        if(hasTexture) { \
		        diffuseColor = texture(sp, tc).rgb; \
	        } \
	         \
	        vec3 ambientColor = vec3(0.1) * diffuseColor; \
	        float distance = length(-position_worldSpace) / 40; \
	         \
	        vec3 normal = normalize(normal_cameraSpace); \
            \
	        vec3 light = normalize(lightDirection_cameraSpace); \
	        vec3 eye = normalize(eyeDirection_cameraSpace); \
            \
            if(hasNormal) { \
                normal = normalize(texture(nor, tc).rgb * 2.0 - 1.0); \
                light = tbnMatrix * light; \
                eye = tbnMatrix * eye; \
            } \
	         \
	        vec3 ref = reflect(-light, normal); \
	         \
	        float cosTheta = clamp(dot(normal,light), 0, 1); \
	        float cosAlpha = clamp(dot(eye, ref), 0, 1); \
	         \
            if(isEmissive) \
                frag_color = vec4(diffuseColor, 1); \
            else \
	            frag_color = vec4(ambientColor +  \
		            diffuseColor * lightColor * lightPower * cosTheta / (distance * distance) + \
		            specularColor * lightColor * specularPower * cosAlpha / (distance * distance), 1); \
        }";
  }

  GLuint ShaderObj = glCreateShader(ShaderType);

  if (ShaderObj == 0) 
  {
    std::cerr << "Error creating shader type " << ShaderType << std::endl;
    return false;
  }

  // Save the shader object - will be deleted in the destructor
  m_shaderObjList.push_back(ShaderObj);

  const GLchar* p[1];
  p[0] = s.c_str();
  GLint Lengths[1] = { (GLint)s.size() };

  glShaderSource(ShaderObj, 1, p, Lengths);

  glCompileShader(ShaderObj);

  GLint success;
  glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);

  if (!success) 
  {
    GLchar InfoLog[1024];
    glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
    std::cerr << "Error compiling: " << InfoLog << std::endl;
    return false;
  }

  glAttachShader(m_shaderProg, ShaderObj);

  return true;
}


// After all the shaders have been added to the program call this function
// to link and validate the program.
bool Shader::Finalize()
{
  GLint Success = 0;
  GLchar ErrorLog[1024] = { 0 };

  glLinkProgram(m_shaderProg);

  glGetProgramiv(m_shaderProg, GL_LINK_STATUS, &Success);
  if (Success == 0)
  {
    glGetProgramInfoLog(m_shaderProg, sizeof(ErrorLog), NULL, ErrorLog);
    std::cerr << "Error linking shader program: " << ErrorLog << std::endl;
    return false;
  }

  glValidateProgram(m_shaderProg);
  glGetProgramiv(m_shaderProg, GL_VALIDATE_STATUS, &Success);
  if (!Success)
  {
    glGetProgramInfoLog(m_shaderProg, sizeof(ErrorLog), NULL, ErrorLog);
    std::cerr << "Invalid shader program: " << ErrorLog << std::endl;
    return false;
  }

  // Delete the intermediate shader objects that have been added to the program
  for (std::vector<GLuint>::iterator it = m_shaderObjList.begin(); it != m_shaderObjList.end(); it++)
  {
    glDeleteShader(*it);
  }

  m_shaderObjList.clear();

  return true;
}


void Shader::Enable()
{
    glUseProgram(m_shaderProg);
}


GLint Shader::GetUniformLocation(const char* pUniformName)
{
    GLuint Location = glGetUniformLocation(m_shaderProg, pUniformName);

    if (Location == INVALID_UNIFORM_LOCATION) {
        fprintf(stderr, "Warning! Unable to get the location of uniform '%s'\n", pUniformName);
    }

    return Location;
}

GLint Shader::GetAttribLocation(const char* pAttribName)
{
    GLuint Location = glGetAttribLocation(m_shaderProg, pAttribName);

    if (Location == -1) {
        fprintf(stderr, "Warning! Unable to get the location of attribute '%s'\n", pAttribName);
    }

    return Location;
}
