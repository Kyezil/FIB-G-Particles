#version 330
in vec3 vertex;
in vec4 particle;
uniform mat4 modelViewProjectionMatrix;

out vec3 center;
out vec3 position;
out float life;

uniform float size = 1.0f;
uniform float radius = 2.0f;

void main() {
    center = radius*particle.xyz;
    life = particle.w;
    position = size*vertex + center;
    gl_Position = modelViewProjectionMatrix * vec4(position, 1.0);
}
