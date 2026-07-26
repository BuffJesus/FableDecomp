#include "rebuild_abi.h"

struct CDisplayEngineVertexShaderView
{
    fable_u8 unknown0000[0x3CFD];
    fable_u8 vertexShaderDisabled3CFD;

    void SetVertexShaderMode(bool enabled);
};

struct CShaderRenderManagerDisableVertexView
{
    fable_u8 unknown000[0x1D0];
    CDisplayEngineVertexShaderView* displayEngine1D0;
    fable_u8 unknown1D4[4];
    void* currentVertexShader1D8;
    fable_u8 unknown1DC[0x1B4];
    fable_u8 vertexShadersEnabled390;

    void DisableVertexShaders();
};

void CShaderRenderManagerDisableVertexView::DisableVertexShaders()
{
    vertexShadersEnabled390 = 0;
    currentVertexShader1D8 = 0;
    displayEngine1D0->SetVertexShaderMode(false);
    displayEngine1D0->vertexShaderDisabled3CFD = 1;
}
