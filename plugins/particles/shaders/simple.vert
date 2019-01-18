#version 330
in vec3 vertex;
in vec3 particle;
uniform mat4 modelViewProjectionMatrix;

out vec3 center;
out vec3 position;

uniform float size = 0.7f;
uniform float radius = 2.0f;

void main() {
    center = radius*particle;
    position = size*vertex + center;
    gl_Position = modelViewProjectionMatrix * vec4(position, 1.0);
}
