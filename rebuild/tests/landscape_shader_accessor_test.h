#ifndef FABLETLC_LANDSCAPE_SHADER_ACCESSOR_TEST_H
#define FABLETLC_LANDSCAPE_SHADER_ACCESSOR_TEST_H

#include <stdio.h>
#include <string.h>
#include "candidates/landscape_shader_accessors.h"

typedef CVertexShader& (CEngineLandscapeRenderer::*ShaderAccessor)(long);

static int TestEmbedded(ShaderAccessor accessor, int spot)
{
    CEngineLandscapeRenderer renderer;
    CVertexShader shaders[4];
    memset(&renderer, 0, sizeof(renderer));
    if (spot) {
        renderer.foreground_spot_begin = reinterpret_cast<long>(shaders);
        renderer.foreground_spot_end = reinterpret_cast<long>(shaders + 4);
    } else {
        renderer.foreground_begin = reinterpret_cast<long>(shaders);
        renderer.foreground_end = reinterpret_cast<long>(shaders + 4);
    }
    if (&((renderer.*accessor)(1)) != &shaders[1]) return 1;
    if (&((renderer.*accessor)(99)) != &shaders[3]) return 2;
    return 0;
}

static int TestIndirect(ShaderAccessor accessor, int field)
{
    CEngineLandscapeRenderer renderer;
    CVertexShader shaders[4];
    CVertexShaderArray array;
    memset(&renderer, 0, sizeof(renderer));
    array.begin = reinterpret_cast<long>(shaders);
    array.end = reinterpret_cast<long>(shaders + 4);
    if (field == 0) renderer.foreground_shadowed = &array;
    if (field == 1) renderer.foreground_bump_shadowed = &array;
    if (field == 2) renderer.foreground_spot_shadowed = &array;
    if (&((renderer.*accessor)(2)) != &shaders[2]) return 1;
    if (&((renderer.*accessor)(99)) != &shaders[3]) return 2;
    return 0;
}

static int ReportShaderTest(const char* label, int result)
{
    if (result != 0) {
        printf("%s FAIL result=%d\n", label, result);
        return result;
    }
    printf("%s PASS\n", label);
    return 0;
}

#endif
