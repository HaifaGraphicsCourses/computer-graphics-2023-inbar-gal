# version 330 core

uniform vec4 ambientLightColor;
uniform vec4 diffuseLightColor;
uniform vec4 specularLightColor;
uniform vec4 ambientModelColor;
uniform vec4 diffuseModelColor;
uniform vec4 specularModelColor;
uniform sampler2D textureMap;

uniform vec3 lightPosition;
uniform vec3 eyePosition;
uniform float shine;

uniform bool withTexture;
uniform int numberOfColors;

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec2 vertexTexture;

out vec4 FragColor;

void main() {
	vec4 ambient, diffuse, specular, temp;
	// ambient doesn't need parameters
	
	// diffuse
	vec3 NewNormal = normalize(vertexNormal);
	vec3 lightVector = normalize(lightPosition - vertexPosition);
	float dFactor = max(dot(NewNormal, lightVector), 0.0f);
	
	// specular
	vec3 eyeVector = normalize(eyePosition - vertexPosition);
	vec3 reflectedLightVector = reflect(-lightVector, vertexNormal);
	float sFactor = pow(max(dot(eyeVector, reflectedLightVector), 0.0f), shine);
	
	float colorChoice = floor(dFactor * numberOfColors);
	float cutBy = colorChoice / numberOfColors;

	if(withTexture == false) {
		float intensity = dot(lightVector, vertexNormal);

		ambient = ambientLightColor * ambientModelColor * cutBy;
		diffuse = diffuseLightColor * dFactor * diffuseModelColor * cutBy;
		specular = specularLightColor * sFactor * specularModelColor * cutBy;

		if(intensity > 0.8) {
			FragColor = clamp(ambient + diffuse + specular, 0, 1);
		}
		else if(intensity > 0.6) {
			FragColor = clamp(ambient + diffuse, 0, 1);
		}
		else if(intensity > 0.4) {
			temp = (ambient + diffuse) * 0.6;
			FragColor = clamp(temp, 0, 1);
		}
		else if(intensity > 0.2) {
			temp = (ambient + diffuse) * 0.3;
			FragColor = clamp(temp, 0, 1);
		}
		else {
			FragColor = clamp(ambient, 0, 1);
		}
	}
	else if(withTexture == true) {
		vec3 texColor = texture(textureMap, vertexTexture).xyz;
		texColor *= cutBy;		
		FragColor = vec4(texColor, 1.0);
	}
}