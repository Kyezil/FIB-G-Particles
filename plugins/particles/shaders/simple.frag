#version 330
in vec3 center;
in vec3 position;
in float life;

uniform float size = 1.0f;
uniform float radius = 2.0f;

uniform mat4 modelViewMatrix;

float decay(float x) {
    float h = size/2; // max dist
    return pow(1-x/h, 4) * (4*x/h + 1);
}

void main() {
    float d = distance(center, position);
    // make circle
    if (d > size/2) discard;
    // color
    vec4 center_eye = modelViewMatrix * vec4(center, 0.0);
    float closeness = (center_eye.z + radius)/(2*radius); // in [0,1], 1 is close

    float distance_decay = exp(-(1-closeness));
    float intensity = decay(d)*distance_decay;
    gl_FragColor = vec4(intensity)*life*vec4(abs(center),1.0);

    // view particle border:
    //if (d > size/2-0.01) gl_FragColor.r = 1;
}
