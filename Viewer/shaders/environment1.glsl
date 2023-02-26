# version 330 core

layout (location = 0) in vec3 Pos;
layout (location = 1) in vec3 Normal;

uniform mat4 model;
uniform mat4 world;
uniform mat4 view;
uniform mat4 projection;

out vec3 Position;
out vec3 normal;

void main() {
    normal = mat3(transpose(inverse(world * model))) * Normal;
    Position = vec3(world * model * vec4(Pos, 1.0));
    gl_Position = projection * view * world * model * vec4(Pos, 1.0);
}