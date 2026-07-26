#include <stdio.h>
#include <string.h>

#include "rebuild_abi.h"

struct CDisplayEngineVertexShaderView
{
    fable_u8 unknown0000[0x3CFD];
    fable_u8 vertexShaderDisabled3CFD;

    void SetVertexShaderMode(bool enabled);
};

struct CShaderRenderManagerEnableVertexView
{
    fable_u8 unknown000[0x1D0];
    CDisplayEngineVertexShaderView* displayEngine1D0;
    fable_u8 unknown1D4[4];
    void* currentVertexShader1D8;
    fable_u8 unknown1DC[0x1B4];
    fable_u8 vertexShadersEnabled390;
    fable_u8 unknown391;
    fable_u8 vertexShaderDirty392;
    fable_u8 unknown393[0x45];
    fable_u32 shaderStateMask3D8;

    void EnableVertexShaders();
};

static int g_SetModeCalls = 0;
static bool g_LastMode = false;

void CDisplayEngineVertexShaderView::SetVertexShaderMode(bool enabled)
{
    ++g_SetModeCalls;
    g_LastMode = enabled;
}

int main()
{
    CDisplayEngineVertexShaderView displayEngine;
    CShaderRenderManagerEnableVertexView manager;
    memset(&displayEngine, 0xFF, sizeof(displayEngine));
    memset(&manager, 0xFF, sizeof(manager));
    manager.displayEngine1D0 = &displayEngine;

    manager.EnableVertexShaders();
    if (
        manager.vertexShadersEnabled390 != 1 ||
        manager.currentVertexShader1D8 != 0 ||
        manager.vertexShaderDirty392 != 0 ||
        displayEngine.vertexShaderDisabled3CFD != 0 ||
        manager.shaderStateMask3D8 != 0xFFFFFFFF ||
        g_SetModeCalls != 1 ||
        !g_LastMode)
    {
        return 1;
    }

    printf("FABLETLC_ENABLE_VERTEX_SHADERS_BEHAVIOR PASS\n");
    return 0;
}
