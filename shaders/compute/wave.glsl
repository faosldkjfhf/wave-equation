#version 430 core
layout(local_size_x = 8, local_size_y = 8, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform image2D heightMap;
layout(rgba32f, binding = 1) uniform image2D derivativeMap;

uniform bool uMouseClicked;
uniform vec2 uMousePosition;
uniform float uTau; // time step size
uniform float uAlpha; // damping effect

void main() {
    ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);
    vec4 color = imageLoad(heightMap, texelCoord);

    // start a ripple where the mouse is clicked
    if (uMouseClicked && texelCoord == ivec2(uMousePosition)) {
        color = vec4(1.0);
    }

    // update heights based on the derivative map
    vec4 utt = vec4(uTau * imageLoad(derivativeMap, texelCoord).rgb, 1.0);
    color += utt;

    imageStore(heightMap, texelCoord, color);
}
