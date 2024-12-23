#version 430 core
layout(local_size_x = 8, local_size_y = 8, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform image2D heightMap;
layout(rgba32f, binding = 1) uniform image2D derivativeMap;

uniform bool uMouseClicked;
uniform vec2 uMousePosition;

const float tau = 0.2; // time step size
const float alpha = 0.985; // dampening

void main() {
    ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);
    vec4 color = imageLoad(heightMap, texelCoord);

    if (uMouseClicked && texelCoord == ivec2(uMousePosition)) {
        color = vec4(1.0);
    }

    // update heights
    vec4 deriv = vec4(tau * imageLoad(derivativeMap, texelCoord).rgb, 1.0);
    color += deriv;

    imageStore(heightMap, texelCoord, color);
}
