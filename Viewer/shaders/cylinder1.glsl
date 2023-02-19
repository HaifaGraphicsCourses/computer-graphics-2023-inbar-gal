# version 330 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 TexCoord;

uniform mat4 model;
uniform mat4 world;
uniform mat4 view;
uniform mat4 projection;

out vec3 vertexPosition;
out vec3 vertexNormal;
out vec2 vertexTexture;

void main() {
    vec4 temp = projection * view * world * model * vec4(Position, 1.0);
    vertexPosition = vec3(temp);
    vertexNormal = Normal;
    vertexTexture = vec2(Position.z, atan(Position.x, Position.y) + 3.141592);
    gl_Position = temp;
}