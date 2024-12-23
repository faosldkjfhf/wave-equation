#version 430 core

out vec4 FragColor;

in vec2 outTexCoord;

uniform sampler2D uTexture;
uniform sampler2D uDerivativeTexture;

void main() {
    FragColor = vec4(texture(uTexture, outTexCoord).rgb, 1.0);
}
