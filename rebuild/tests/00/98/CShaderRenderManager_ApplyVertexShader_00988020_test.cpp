#include <stdio.h>
#include <string.h>

#include "rebuild_abi.h"

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

static fable_i32 g_CallCount;
static fable_i32 g_CallKinds[4];
static void* g_CallValues[4];

static long __stdcall ObserveVertexShader(
    FableVertexShaderDevice*,
    void* shader)
{
    g_CallKinds[g_CallCount] = 1;
    g_CallValues[g_CallCount++] = shader;
    return 0;
}

static long __stdcall ObserveVertexDeclaration(
    FableVertexShaderDevice*,
    void* declaration)
{
    g_CallKinds[g_CallCount] = 2;
    g_CallValues[g_CallCount++] = declaration;
    return 0;
}

int main()
{
    void* vtable[0x5D];
    memset(vtable, 0, sizeof(vtable));
    vtable[0x5C] = reinterpret_cast<void*>(ObserveVertexShader);
    vtable[0x57] = reinterpret_cast<void*>(ObserveVertexDeclaration);
    FableVertexShaderDevice device = {vtable};

    CDisplayEngineApplyVertexShaderView engine;
    memset(&engine, 0, sizeof(engine));
    engine.vertexShaderMode2908 = 1;
    engine.vertexShaderDisabled3CFD = 1;

    CVertexShaderDataView data = {
        0,
        reinterpret_cast<void*>(0x22220000),
        reinterpret_cast<void*>(0x11110000)};
    CVertexShaderApplyView shader = {{0, 0, 0, 0}, &data};

    CShaderRenderManagerApplyVertexShaderView manager;
    memset(&manager, 0, sizeof(manager));
    manager.displayEngine1D0 = &engine;
    manager.displayDevice1D4 = &device;
    manager.currentVertexShader1D8 = data.vertexShader08;
    manager.currentVertexDeclaration1DC = data.vertexDeclaration04;
    manager.vertexShaderModeA393 = 0;

    manager.ApplyVertexShader(shader, false);
    if (
        g_CallCount != 2 ||
        g_CallKinds[0] != 1 ||
        g_CallValues[0] != data.vertexShader08 ||
        g_CallKinds[1] != 2 ||
        g_CallValues[1] != data.vertexDeclaration04 ||
        manager.currentVertexShader1D8 != data.vertexShader08 ||
        manager.currentVertexDeclaration1DC != data.vertexDeclaration04 ||
        engine.vertexShaderDisabled3CFD != 0)
    {
        return 1;
    }

    g_CallCount = 0;
    engine.vertexShaderDisabled3CFD = 1;
    manager.vertexShaderModeB394 = 1;
    manager.useVertexShaderModeB395 = 1;
    manager.ApplyVertexShader(shader, true);
    if (g_CallCount != 0 || engine.vertexShaderDisabled3CFD != 1)
    {
        return 2;
    }

    data.vertexShader08 = reinterpret_cast<void*>(0x33330000);
    data.vertexDeclaration04 = reinterpret_cast<void*>(0x44440000);
    manager.ApplyVertexShader(shader, false);
    if (
        g_CallCount != 2 ||
        g_CallKinds[0] != 1 ||
        g_CallValues[0] != data.vertexShader08 ||
        g_CallKinds[1] != 2 ||
        g_CallValues[1] != data.vertexDeclaration04 ||
        manager.currentVertexShader1D8 != data.vertexShader08 ||
        manager.currentVertexDeclaration1DC != data.vertexDeclaration04)
    {
        return 3;
    }

    printf("FABLETLC_APPLY_VERTEX_SHADER_BEHAVIOR PASS\n");
    return 0;
}
