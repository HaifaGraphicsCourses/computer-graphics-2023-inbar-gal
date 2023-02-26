# version 330 core

uniform vec4 ambientLightColor;
uniform vec4 diffuseLightColor;
uniform vec4 specularLightColor;
uniform vec4 ambientModelColor;
uniform vec4 diffuseModelColor;
uniform vec4 specularModelColor;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

uniform vec3 lightPosition;
uniform vec3 eyePosition;
uniform float shine;
uniform bool a;
uniform bool d;
uniform bool s;

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec2 vertexTexture;

in vec3 TangentLightPos;
in vec3 TangentViewPos;
in vec3 TangentFragPos;

out vec4 FragColor;

void main() {
	// obtain normal from normal map in range [0, 1]
    vec3 normal = texture(normalMap, vertexTexture).rgb;
    // transform normal vector to range [-1, 1]
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space
   
    // get diffuse color
    vec3 color = texture(diffuseMap, vertexTexture).rgb;
    
	// ambient
    vec4 ambient = 0.1 * vec4(color, 1.0);
    
	// diffuse
    vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec4 diffuse = diff * vec4(color, 1.0);
    
	// specular
    vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), shine);
    vec4 specular = vec4(0.2) * spec;

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