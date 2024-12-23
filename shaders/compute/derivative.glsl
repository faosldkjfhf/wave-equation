#version 430 core
layout(local_size_x = 8, local_size_y = 8, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform image2D heightMap;
layout(rgba32f, binding = 1) uniform image2D derivativeMap;

const float tau = 0.2; // time step size
const float alpha = 0.985; // dampening

bool outOfBounds(ivec2 pos) {
    return pos.x < 0 || pos.x >= int(gl_NumWorkGroups.x * gl_WorkGroupSize.x)
        || pos.y < 0 || pos.y >= int(gl_NumWorkGroups.y * gl_WorkGroupSize.y);
}

void main() {
    ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);
    int width = int(gl_NumWorkGroups.x * gl_WorkGroupSize.x);
    int height = int(gl_NumWorkGroups.y * gl_WorkGroupSize.y);

    vec4 finalColor = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 currentPixelColor = imageLoad(heightMap, texelCoord);

    vec4 up = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 down = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 left = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 right = vec4(0.0, 0.0, 0.0, 1.0);

    // neighboring points
    if (!outOfBounds(ivec2(texelCoord.x - 1, texelCoord.y))) {
        up = imageLoad(heightMap, ivec2(texelCoord.x - 1, texelCoord.y));
    }

    if (!outOfBounds(ivec2(texelCoord.x + 1, texelCoord.y))) {
        down = imageLoad(heightMap, ivec2(texelCoord.x + 1, texelCoord.y));
    }

    if (!outOfBounds(ivec2(texelCoord.x, texelCoord.y - 1))) {
        left = imageLoad(heightMap, ivec2(texelCoord.x, texelCoord.y - 1));
    }

    if (!outOfBounds(ivec2(texelCoord.x, texelCoord.y + 1))) {
        right = imageLoad(heightMap, ivec2(texelCoord.x, texelCoord.y + 1));
    }

    finalColor = tau * (up + down + left + right - 4 * currentPixelColor);
    finalColor *= alpha;

    vec4 currentDerivative = imageLoad(derivativeMap, texelCoord);
    imageStore(derivativeMap, texelCoord, vec4(currentDerivative.rgb + finalColor.rgb, 1.0));
}
