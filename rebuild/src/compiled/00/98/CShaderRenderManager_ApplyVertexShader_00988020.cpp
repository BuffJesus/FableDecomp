#include "rebuild_abi.h"

struct FableVertexShaderDevice;
typedef long (__stdcall *FableSetVertexShaderCall)(
    FableVertexShaderDevice* device,
    void* vertexShader);
typedef long (__stdcall *FableSetVertexDeclarationCall)(
    FableVertexShaderDevice* device,
    void* vertexDeclaration);

struct FableVertexShaderDevice
{
    void** vtable;
};

struct CDisplayEngineApplyVertexShaderView
{
    fable_u8 unknown0000[0x2908];
    fable_i32 vertexShaderMode2908;
    fable_u8 unknown290C[0x13F1];
    fable_u8 vertexShaderDisabled3CFD;
};

struct CVertexShaderDataView
{
    void* unknown00;
    void* vertexDeclaration04;
    void* vertexShader08;
};

struct CVertexShaderApplyView
{
    fable_u8 unknown00[4];
    CVertexShaderDataView* data04;
};

struct CShaderRenderManagerApplyVertexShaderView
{
    fable_u8 unknown000[0x1D0];
    CDisplayEngineApplyVertexShaderView* displayEngine1D0;
    FableVertexShaderDevice* displayDevice1D4;
    void* currentVertexShader1D8;
    void* currentVertexDeclaration1DC;
    fable_u8 unknown1E0[0x1B3];
    fable_u8 vertexShaderModeA393;
    fable_u8 vertexShaderModeB394;
    fable_u8 useVertexShaderModeB395;

    void ApplyVertexShader(
        const CVertexShaderApplyView& shader,
        bool force);
};

void CShaderRenderManagerApplyVertexShaderView::ApplyVertexShader(
    const CVertexShaderApplyView& shader,
    bool)
{
    const fable_u8 expectedMode =
        useVertexShaderModeB395
            ? vertexShaderModeB394
            : vertexShaderModeA393;
    const fable_u8 actualMode =
        static_cast<fable_u8>(
            displayEngine1D0->vertexShaderMode2908 != 0);
    if (
        expectedMode != actualMode)
    {
        currentVertexShader1D8 = 0;
    }

    if (
        currentVertexShader1D8 == shader.data04->vertexShader08 &&
        currentVertexDeclaration1DC ==
            shader.data04->vertexDeclaration04)
    {
        return;
    }

    currentVertexShader1D8 = shader.data04->vertexShader08;
    currentVertexDeclaration1DC =
        shader.data04->vertexDeclaration04;
    displayEngine1D0->vertexShaderDisabled3CFD = 0;

    reinterpret_cast<FableSetVertexShaderCall>(
        displayDevice1D4->vtable[0x5C])(
            displayDevice1D4,
            currentVertexShader1D8);
    reinterpret_cast<FableSetVertexDeclarationCall>(
        displayDevice1D4->vtable[0x57])(
            displayDevice1D4,
            currentVertexDeclaration1DC);
}
