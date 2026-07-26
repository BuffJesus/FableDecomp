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

void CShaderRenderManagerEnableVertexView::EnableVertexShaders()
{
    vertexShadersEnabled390 = 1;
    currentVertexShader1D8 = 0;
    vertexShaderDirty392 = 0;
    displayEngine1D0->vertexShaderDisabled3CFD = 0;
    displayEngine1D0->SetVertexShaderMode(true);
    shaderStateMask3D8 = 0xFFFFFFFF;
}
