#version 410 core

layout(location=0) in vec3 vPosition;
layout(location=1) in vec3 vNormal;
layout(location=2) in vec2 vTexCoords;

out vec3 fNormal;
out vec4 fPosEye;
out vec4 lightPosEye;
out vec2 fTexCoords;
out vec4 fragPosLightSpace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform	mat3 normalMatrix;
uniform vec4 lightPos;
uniform vec4 lightSpaceTrMatrix;


void main() 
{


	//compute eye space coordinates
	fPosEye = view * model * vec4(vPosition, 1.0f);
	lightPosEye = view * model * lightPos;

	
	fragPosLightSpace = lightSpaceTrMatrix * model * vec4(vPosition, 1.0f);


	fTexCoords = vTexCoords;

	fNormal = normalize(normalMatrix * vNormal);
	gl_Position = projection * view * model * vec4(vPosition, 1.0f);
}
