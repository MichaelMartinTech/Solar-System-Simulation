#version 460

layout (location = 0) in vec3 v_position; 
layout (location = 1) in vec3 v_normal; 
layout (location = 2) in vec2 v_tc;  
    
out vec3 position_worldSpace; 
out vec3 normal_cameraSpace; 
out vec3 eyeDirection_cameraSpace; 
out vec3 lightDirection_cameraSpace; 
out vec2 tc;

uniform mat4 projectionMatrix; 
uniform mat4 viewMatrix; 
uniform mat4 modelMatrix; 
uniform vec3 sunPosition; 

void main(void) 
{ 
    gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * vec4(v_position, 1); 

    position_worldSpace = (modelMatrix * vec4(v_position, 1)).xyz; 

    vec3 vertexPosition_cameraSpace = (viewMatrix * modelMatrix * vec4(v_position, 1)).xyz; 
    eyeDirection_cameraSpace = vec3(0,0,0) - vertexPosition_cameraSpace; 

    vec3 lightPosition_cameraSpace = (viewMatrix * vec4(sunPosition, 1)).xyz; 
    lightDirection_cameraSpace = lightPosition_cameraSpace + eyeDirection_cameraSpace; 

    normal_cameraSpace = (viewMatrix * modelMatrix * vec4(v_normal, 0)).xyz; 

    tc = v_tc;
}