#version 410 core

in vec3 fNormal;
in vec4 fPosEye;
in vec2 fTexCoords;
in vec4 fragPosLightSpace;
in vec4 lightPosEye;

in vec4 lightFelinar;

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
vec3 diffuse;
vec3 specular;
vec3 ambientPunctiform;
vec3 diffusePunctiform;
vec3 specularPunctiform;
float specularStrength = 0.5f;
float ambientStrength = 0.2f;
float shininess = 32.0f;

float constant = 1.0f;
float linear = 0.0045f;
float quadratic = 0.0075f;

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

float specFelinar;
vec3 eyeFelinar;
void computeLightComponents()
{		
	vec3 cameraPosEye = vec3(0.0f);//in eye coordinates, the viewer is situated at the origin
	
	//transform normal
	vec3 normalEye = normalize(fNormal);	
	eyeFelinar=normalEye;
	
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
	specFelinar=specCoeff;
	specular = specularStrength * specCoeff * lightColor;
}

void computePunctiform(){

	vec3 cameraPosEye = vec3(0.0f);//in eye coordinates, the viewer is situated at the origin
	
	//transform normal
	vec3 normalEye = normalize(fNormal);	
	
	//compute light direction
	vec3 lightDirN = normalize(lightPosEye.xyz - fPosEye.xyz);
	
	//compute view direction 
	vec3 viewDirN = normalize(cameraPosEye - fPosEye.xyz);

	//compute half vector
	vec3 halfVector = normalize(lightDirN + viewDirN);
		
	//compute distance to light
	float dist = length(lightPosEye.xyz - fPosEye.xyz) + 5.f;
	//compute attenuation
	float att = 1.0f / (constant + linear * dist + quadratic * (dist * dist));


	//compute ambient light
	ambientPunctiform = att * ambientStrength * lightColor;

	//compute diffuse light
	diffusePunctiform = att * max(dot(normalEye, lightDirN), 0.0f) * lightColor;

	
	//compute specular light
	float specCoeff = pow(max(dot(normalEye, halfVector), 0.0f), shininess);
	specularPunctiform = att*specularStrength * specCoeff * lightColor;

}

float atenuareFelinar;
float distantaFelinar;

vec3 ambientF;
vec3 diffuseF;
vec3 specularF;

void computeFelinar(){
	
	distantaFelinar = length(lightFelinar.xyz - fPosEye.xyz);	
	atenuareFelinar=1.0f/(constant+linear*distantaFelinar + quadratic*distantaFelinar*distantaFelinar);





	vec3 cameraPosEye = vec3(0.0f);//in eye coordinates, the viewer is situated at the origin
	
	//transform normal
	vec3 normalEye = normalize(fNormal);	
	
	//compute light direction
	vec3 lightDirN = normalize(lightPosEye.xyz - fPosEye.xyz);
	
	//compute view direction 
	vec3 viewDirN = normalize(cameraPosEye - fPosEye.xyz);
		
	vec3 reflection = reflect(-lightDirN, normalEye);






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
	computePunctiform();
	computeFelinar();
 	ambientF= atenuareFelinar * ambientStrength * lightColor;
 	diffuseF=atenuareFelinar * max(dot(eyeFelinar, normalize(lightFelinar.xyz-fPosEye.xyz)), 0.0f) * lightColor;
 	specularF = atenuareFelinar * specularStrength * specFelinar * lightColor;


 	
	vec3 baseColor = vec3(245.f/255.f, 247.f/255.f, 136.f/255.f);//yellowish


	
	ambient *= texture(diffuseTexture, fTexCoords).rgb;
	diffuse *= texture(diffuseTexture, fTexCoords).rgb;
	specular *= texture(specularTexture, fTexCoords).rgb;

	ambientPunctiform *= texture(diffuseTexture, fTexCoords).rgb;
	diffusePunctiform *= texture(diffuseTexture, fTexCoords).rgb;
	specularPunctiform *= texture(specularTexture, fTexCoords).rgb;
	ambientPunctiform *= baseColor;
	diffusePunctiform *= baseColor;
	specularPunctiform *= baseColor;


	ambientF*=vec3(0.8f,0.0f,0.0f);
	diffuseF*=vec3(0.8f,0.0f,0.0f);
	specularF*=vec3(0.8f,0.0f,0.0f);

	if(darkness == 1){
		ambient *=0.05f;
		diffuse *= 0.05f;
		specular *=0.05f;
	}

	if(lightning ==1){
		ambient *=17.f;
		diffuse *= 17.f;
		specular *=17.f;
	}
	vec3 color;
	if(darkness == 1){
		shadow = 1.f;

		ambient+=ambientPunctiform;
		diffuse+=diffusePunctiform;
		specular+=specularPunctiform;


		//ambient+=ambientF;
		//diffuse+=diffuseF;
		//specular+=specularF;

		color =  min((ambient + (1.0f - shadow)*diffuse) + (1.0f - shadow)*specular, 1.0f);
	}else{
		shadow = computeShadow();

		color = min((ambient + (1.0f - shadow)*diffuse) + (1.0f - shadow)*specular, 1.0f);
	}

    

	
	vec3 fogColor = vec3(0.5f, 0.5f, 0.5f);

	float fogFactor = computeFog();
	if (darkness == 1){
		fogColor = vec3(0.05f, 0.05f, 0.05f);
	}




	color=mix(fogColor, color, fogFactor);


	

	fColor = vec4(color, 1.0f);



}