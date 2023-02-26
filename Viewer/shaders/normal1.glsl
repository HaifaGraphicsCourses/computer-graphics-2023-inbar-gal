# version 330 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 TexCoord;
layout (location = 3) in vec3 Tangent;
layout (location = 4) in vec3 Bitangent;

uniform mat4 model;
uniform mat4 world;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightPos;
uniform vec3 viewPos;

out vec3 vertexPosition;
out vec3 vertexNormal;
out vec2 vertexTexture;

out vec3 TangentLightPos;
out vec3 TangentViewPos;
out vec3 TangentFragPos;

void main() {
    vec4 temp = projection * view * world * model * vec4(Position, 1.0);

    vertexPosition = vec3(temp);
    vertexNormal = Normal;
    vertexTexture = TexCoord;
    
    mat3 normalMatrix = transpose(inverse(mat3(world * model)));
    vec3 T = normalize(normalMatrix * Tangent);
    vec3 N = normalize(normalMatrix * Normal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    
    mat3 TBN = transpose(mat3(T, B, N));    
    TangentLightPos = TBN * lightPos;
    TangentViewPos  = TBN * viewPos;
    TangentFragPos  = TBN * vertexPosition;

    gl_Position = temp;
}