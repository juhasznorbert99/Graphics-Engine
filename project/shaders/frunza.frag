#version 410 core

in vec3 fNormal;
in vec4 fPosEye;
in vec2 fTexCoords;
in vec4 fragPosLightSpace;


out vec4 fColor;


//lighting
uniform	vec3 lightDir;
uniform	vec3 lightColor;

//texture
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D shadowMap;
uniform int darkness;
uniform int lightning;

vec3 ambient;
float ambientStrength = 0.2f;
vec3 diffuse;
vec3 specular;
float specularStrength = 0.5f;
float shininess = 32.0f;



float computeShadow()
{

	// Check whether current frag pos is in shadow
	float bias = 0.005f;
	

	vec3 normalizedCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	normalizedCoords = normalizedCoords * 0.5 + 0.5;

	if (normalizedCoords.z > 1.0f)
		return 0.0f;


	float closestDepth = texture(shadowMap, normalizedCoords.xy).r;

	// Get depth of current fragment from light's perspective
	float currentDepth = normalizedCoords.z;

	currentDepth = currentDepth - bias;

	// Check whether current frag pos is in shadow
	float shadow = currentDepth > closestDepth ? 1.0f : 0.0f;

	return shadow;
}
//modulate with shadow


void computeLightComponents()
{		
	vec3 cameraPosEye = vec3(0.0f);//in eye coordinates, the viewer is situated at the origin
	
	//transform normal
	vec3 normalEye = normalize(fNormal);	
	
	//compute light direction
	vec3 lightDirN = normalize(lightDir);
	
	//compute view direction 
	vec3 viewDirN = normalize(cameraPosEye - fPosEye.xyz);
		
	//compute ambient light
	ambient = ambientStrength * lightColor;
	
	//compute diffuse light
	diffuse = max(dot(normalEye, lightDirN), 0.0f) * lightColor;
	
	//compute specular light
	vec3 reflection = reflect(-lightDirN, normalEye);
	float specCoeff = pow(max(dot(viewDirN, reflection), 0.0f), shininess);
	specular = specularStrength * specCoeff * lightColor;
}



float computeFog()
{
 float fogDensity = 0.02f;
 float fragmentDistance = length(fPosEye);
 float fogFactor = exp(-pow(fragmentDistance * fogDensity, 1))*1000;

 return clamp(fogFactor, 0.0f, 1.0f);
}


void main() 
{
	float shadow = computeShadow();
	

	computeLightComponents();
	
	vec3 baseColor = vec3(0.9f, 0.35f, 0.0f);//orange
	
	ambient *= texture(diffuseTexture, fTexCoords).rgb;
	diffuse *= texture(diffuseTexture, fTexCoords).rgb;
	specular *= texture(specularTexture, fTexCoords).rgb;

    
   if(darkness == 1){
		ambient *=0.1f;
		diffuse *= 0.1f;
		specular *=0.1f;
	}else{
		computeLightComponents();
	
		vec3 baseColor = vec3(0.9f, 0.35f, 0.0f);//orange
		
		ambient *= texture(diffuseTexture, fTexCoords).rgb;
		diffuse *= texture(diffuseTexture, fTexCoords).rgb;
		specular *= texture(specularTexture, fTexCoords).rgb;
	}


	
	vec3 color = min((ambient + (1.0f - shadow)*diffuse) + (1.0f - shadow)*specular, 1.0f);


	float fogFactor = computeFog();
	vec3 fogColor = vec3(0.5f, 0.5f, 0.5f);
	if(darkness == 1){

		fogColor = vec3(1.f, 1.f, 1.f);
	}
	color=mix(fogColor, color, fogFactor);


	vec4 colorFromTexture = texture(diffuseTexture, fTexCoords);

	colorFromTexture.xyz = mix(fogColor,colorFromTexture.xyz,fogFactor);
	
	if(colorFromTexture.a < 0.1)
		discard;

	if(lightning == 1){
		colorFromTexture.xyz *= 5.0f;
	}


	if(darkness == 1){
		colorFromTexture.xyz *= 0.05f;
	}


	fColor = colorFromTexture;

	//fColor = vec4(color, 0.4f);



}