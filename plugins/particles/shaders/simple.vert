#version 330
in vec3 vertex;
in vec3 particle;
uniform mat4 modelViewProjectionMatrix;

void main() {
    gl_Position = modelViewProjectionMatrix * vec4(vertex + particle, 1.0);
}
