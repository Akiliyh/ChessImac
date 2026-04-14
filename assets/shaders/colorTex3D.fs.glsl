#version 330

in vec3 vPosition_vs;;
in vec2 vTexCoords;
in vec3 vNormal_vs;

out vec4 FragColor;

uniform sampler2D uEarthTexture;
uniform vec3 uColor;
uniform bool uUseTexture;

uniform vec3 uLightPos;
uniform vec3 uLightColor;

uniform vec3 uLightPos2;
uniform vec3 uLightColor2;
uniform vec3 uCamPos;

uniform bool uIsSecondLightActive;

// thank you victor gordan man

vec3 pointLight(vec3 lightPos, vec3 lightColor)
{	
	// used in two variables so I calculate it here to not have to do it twice
	vec3 lightVec = lightPos - vPosition_vs;

	// intensity of light with respect to distance
	float dist = length(lightVec);
	float a = 0.2;
	float b = 0.1;
	float inten = 1.0f / (a * dist * dist + b * dist + 1.0f);

	// ambient lighting
	float ambient = 0.20f;

	// diffuse lighting
	vec3 normal = normalize(vNormal_vs);
	vec3 lightDirection = normalize(lightVec);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specular = 0.0f;
	if (diffuse != 0.0f)
	{
		float specularLight = 0.50f;
		vec3 viewDirection = normalize(uCamPos - vPosition_vs);
		vec3 halfwayVec = normalize(viewDirection + lightDirection);
		float specAmount = pow(max(dot(normal, halfwayVec), 0.0f), 16);
		specular = specAmount * specularLight;
	};

   		vec3 result = (ambient + diffuse * inten) * lightColor 
            + specular * inten * lightColor;

    return result;
}

vec4 direcLight(vec3 texColor)
{
    vec3 lightVec = uLightPos - vPosition_vs;;
	// ambient lighting
	float ambient = 0.20f;
    float dist = length(lightVec);
	float a = 3.0;
	float b = 0.7;
	float inten = 1.0f / (a * dist * dist + b * dist + 1.0f);

	// diffuse lighting
	vec3 normal = normalize(vNormal_vs);
	vec3 lightDirection = normalize(vec3(1.0f, 1.0f, 0.0f));
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specular = 0.0f;
	if (diffuse != 0.0f)
	{
		float specularLight = 0.50f;
		vec3 viewDirection = normalize(uCamPos - vPosition_vs);
		vec3 halfwayVec = normalize(viewDirection + lightDirection);
		float specAmount = pow(max(dot(normal, halfwayVec), 0.0f), 16);
		specular = specAmount * specularLight;
	};

    vec3 result = texColor * (ambient + diffuse * inten) + specular * inten * uLightColor;

    return vec4(result, 1.0);
}

vec4 spotLight(vec3 texColor)
{
	// controls how big the area that is lit up is
	float outerCone = 0.90f;
	float innerCone = 0.95f;

	// ambient lighting
	float ambient = 0.20f;

	// diffuse lighting
	vec3 normal = normalize(vNormal_vs);
	vec3 lightDirection = normalize(uLightPos - vPosition_vs);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specular = 0.0f;
	if (diffuse != 0.0f)
	{
		float specularLight = 0.50f;
		vec3 viewDirection = normalize(uCamPos - vPosition_vs);
		vec3 halfwayVec = normalize(viewDirection + lightDirection);
		float specAmount = pow(max(dot(normal, halfwayVec), 0.0f), 16);
		specular = specAmount * specularLight;
	};

	// calculates the intensity of the crntPos based on its angle to the center of the light cone
	float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
	float inten = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

    vec3 result = texColor * (ambient + diffuse * inten) + specular * inten * uLightColor;

    return vec4(result, 1.0);
}

void main() {

    if(uUseTexture) {
        vec3 texColor = texture(uEarthTexture, vTexCoords).rgb;
		vec3 result = pointLight(uLightPos, uLightColor);
		if (uIsSecondLightActive)
			result += pointLight(uLightPos2, uLightColor2);
        FragColor = vec4(result * (texColor * uColor), 1.0);
    } else {
		vec3 result = pointLight(uLightPos, uLightColor);
		if (uIsSecondLightActive)
			result +=  pointLight(uLightPos2, uLightColor2);
        FragColor = vec4(result * uColor, 1.0);
    }

    
}