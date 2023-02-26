# version 330 core

uniform vec3 cameraPos;
uniform sampler2D textureMap;

in vec3 Position;
in vec3 normal;

out vec4 FragColor;

void main() {
    float ratio = 1.00 / 1.52;
    vec3 I = normalize(Position - cameraPos);
    vec3 R = refract(I, normalize(normal), ratio);
    FragColor = vec4(texture(textureMap, R.xy).rgb, 1.0);
}