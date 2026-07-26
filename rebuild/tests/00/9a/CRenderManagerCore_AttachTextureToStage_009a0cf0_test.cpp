#include <stdio.h>
#include <string.h>

#include "rebuild_abi.h"

struct FableTextureDevice
{
    void** vtable;
};

struct CTextureAttachView
{
    void* deviceTexture00;
};

struct CRenderManagerCoreAttachTextureView
{
    fable_u8 unknown0000[0x3CB8];
    fable_i32 activeTextureStageCount3CB8;
    fable_u8 unknown3CBC[0x34];
    FableTextureDevice* displayDevice3CF0;
    fable_u8 unknown3CF4[0x0C];
    void* attachedTextures3D00[16];

    void AttachTextureToStage(
        fable_i32 stage,
        const CTextureAttachView* texture);
};

static fable_i32 g_Calls;
static FableTextureDevice* g_Device;
static fable_u32 g_Stage;
static void* g_Texture;

static long __stdcall ObserveSetTexture(
    FableTextureDevice* device,
    fable_u32 stage,
    void* texture)
{
    ++g_Calls;
    g_Device = device;
    g_Stage = stage;
    g_Texture = texture;
    return 0;
}

int main()
{
    void* vtable[0x42] = {0};
    vtable[0x41] =
        reinterpret_cast<void*>(&ObserveSetTexture);
    FableTextureDevice device = {vtable};
    CRenderManagerCoreAttachTextureView manager;
    memset(&manager, 0, sizeof(manager));
    manager.displayDevice3CF0 = &device;

    fable_u32 textureStorage = 0x12345678;
    CTextureAttachView texture = {&textureStorage};
    manager.AttachTextureToStage(2, &texture);
    if (g_Calls != 1 ||
        g_Device != &device ||
        g_Stage != 2 ||
        g_Texture != &textureStorage ||
        manager.attachedTextures3D00[2] != &textureStorage ||
        manager.activeTextureStageCount3CB8 != 3)
    {
        printf(
            "FABLETLC_ATTACH_TEXTURE_TO_STAGE_BEHAVIOR "
            "FAIL code=1\n");
        return 1;
    }

    manager.AttachTextureToStage(2, &texture);
    if (g_Calls != 1 ||
        manager.activeTextureStageCount3CB8 != 3)
    {
        printf(
            "FABLETLC_ATTACH_TEXTURE_TO_STAGE_BEHAVIOR "
            "FAIL code=2\n");
        return 2;
    }

    manager.AttachTextureToStage(2, 0);
    if (g_Calls != 2 ||
        g_Stage != 2 ||
        g_Texture != 0 ||
        manager.attachedTextures3D00[2] != 0 ||
        manager.activeTextureStageCount3CB8 != 3)
    {
        printf(
            "FABLETLC_ATTACH_TEXTURE_TO_STAGE_BEHAVIOR "
            "FAIL code=3\n");
        return 3;
    }

    manager.AttachTextureToStage(5, &texture);
    if (g_Calls != 3 ||
        g_Stage != 5 ||
        g_Texture != &textureStorage ||
        manager.attachedTextures3D00[5] != &textureStorage ||
        manager.activeTextureStageCount3CB8 != 6)
    {
        printf(
            "FABLETLC_ATTACH_TEXTURE_TO_STAGE_BEHAVIOR "
            "FAIL code=4\n");
        return 4;
    }

    printf(
        "FABLETLC_ATTACH_TEXTURE_TO_STAGE_BEHAVIOR PASS\n");
    return 0;
}
