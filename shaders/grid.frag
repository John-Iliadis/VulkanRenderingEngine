#version 460 core

layout (location = 0) in vec2 vTexCoords;
layout (location = 1) in vec2 vCameraPos;

layout (location = 0) out vec4 outFragColor;

layout (push_constant) uniform PushConstants
{
    vec4 gridColorThin;
    vec4 gridColorThick;
};

const float gridSize = 100.0;
const float gridCellSize = 1.0;
const float gridMinPixelsBetweenCells = 0.7;

float log10(float x) { return log(x) / log(10.0); }
float satf(float x) { return clamp(x, 0.0, 1.0); }
vec2 satv(vec2 x) { return clamp(x, vec2(0.0), vec2(1.0)); }
float max2(vec2 v) { return max(v.x, v.y); }

vec4 gridColor(vec2 uv, vec2 camPos)
{
    vec2 dudv = vec2(
    length(vec2(dFdx(uv.x), dFdy(uv.x))),
    length(vec2(dFdx(uv.y), dFdy(uv.y)))
    );

    float lodLevel = max(0.0, log10((length(dudv) * gridMinPixelsBetweenCells) / gridCellSize) + 1.0);
    float lodFade = fract(lodLevel);

    // cell sizes for lod0, lod1 and lod2
    float lod0 = gridCellSize * pow(10.0, floor(lodLevel));
    float lod1 = lod0 * 10.0;
    float lod2 = lod1 * 10.0;

    // each anti-aliased line covers up to 2 pixels
    dudv *= 2.0;

    // Update grid coordinates for subsequent alpha calculations (centers each anti-aliased line)
    uv += dudv / 2.0;

    // calculate absolute distances to cell line centers for each lod and pick max X/Y to get coverage alpha value
    float lod0a = max2(vec2(1.0) - abs(satv(mod(uv, lod0) / dudv) * 2.0 - vec2(1.0)));
    float lod1a = max2(vec2(1.0) - abs(satv(mod(uv, lod1) / dudv) * 2.0 - vec2(1.0)));
    float lod2a = max2(vec2(1.0) - abs(satv(mod(uv, lod2) / dudv) * 2.0 - vec2(1.0)));

    uv -= camPos;

    // blend between falloff colors to handle LOD transition
    vec4 c = lod2a > 0.0 ? gridColorThick : lod1a > 0.0 ? mix(gridColorThick, gridColorThin, lodFade) : gridColorThin;

    // calculate opacity falloff based on distance to grid extents
    float opacityFalloff = (1.0 - satf(length(uv) / gridSize));

    // blend between LOD level alphas and scale with opacity falloff
    c.a *= (lod2a > 0.0 ? lod2a : lod1a > 0.0 ? lod1a : (lod0a * (1.0-lodFade))) * opacityFalloff;

    return c;
}

void main()
{
    outFragColor = gridColor(vTexCoords, vCameraPos);
}
