#version 430 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;

// uniform mat4 uModel;
// uniform mat4 uView;
// uniform mat4 uProjection;

out vec2 outTexCoord;

void main() {
    outTexCoord = inTexCoord;
    gl_Position = vec4(inPosition, 1.0);
    // gl_Position = uProjection * uView * uModel * vec4(inPosition, 1.0);
}
