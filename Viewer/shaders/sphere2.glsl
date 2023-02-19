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
uniform bool a;
uniform bool d;
uniform bool s;

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec2 vertexTexture;

out vec4 FragColor;

void main() {
	vec4 ambient, diffuse, specular;
	vec3 texColor = texture(textureMap, vertexTexture).xyz;

	// ambient
	ambient = vec4(texColor, 1.0) * ambientLightColor * ambientModelColor;

	// diffuse
	vec3 NewNormal = normalize(vertexNormal);
	vec3 lightVector = normalize(lightPosition - vertexPosition);
	float dFactor = max(dot(NewNormal, lightVector), 0.0f);
	diffuse = vec4(texColor, 1.0) * diffuseLightColor * dFactor * diffuseModelColor;

	// specular
	vec3 eyeVector = normalize(eyePosition - vertexPosition);
	vec3 reflectedLightVector = reflect(-lightVector, vertexNormal);
	float sFactor = pow(max(dot(eyeVector, reflectedLightVector), 0.0f), shine);
	specular = vec4(texColor, 1.0) * specularLightColor * sFactor * specularModelColor;
	
	if(a == true) {
		FragColor = clamp(ambient, 0, 1);
	}
	else if(d == true) {
		FragColor = clamp(ambient + diffuse, 0, 1);
	}
	else if(s == true) {
		FragColor = clamp(ambient + diffuse + specular, 0, 1);
	}
}