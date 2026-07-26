#include <stdio.h>

#include "rebuild_abi.h"

struct FableAssignmentTexture;

typedef fable_u32 (__stdcall *FableAssignmentTextureCall)(
    FableAssignmentTexture* texture);

struct FableAssignmentTextureVTable
{
    void* queryInterface00;
    FableAssignmentTextureCall addRef04;
    FableAssignmentTextureCall release08;
};

struct FableAssignmentTexture
{
    FableAssignmentTextureVTable* vtable;
};

struct CTextureAssignmentView
{
    FableAssignmentTexture* PD3DTexture;
    fable_u32 ByteLength : 28;
    fable_u32 AllocationSource : 4;

    CTextureAssignmentView& operator=(CTextureAssignmentView& rhs);
};

static fable_i32 g_AddRefCalls;
static fable_i32 g_ReleaseCalls;
static FableAssignmentTexture* g_AddedTexture;
static FableAssignmentTexture* g_ReleasedTexture;

static fable_u32 __stdcall AddTextureReference(
    FableAssignmentTexture* texture)
{
    ++g_AddRefCalls;
    g_AddedTexture = texture;
    return 2;
}

static fable_u32 __stdcall ReleaseTextureReference(
    FableAssignmentTexture* texture)
{
    ++g_ReleaseCalls;
    g_ReleasedTexture = texture;
    return 0;
}

int main()
{
    FableAssignmentTextureVTable textureVtable =
        {0, &AddTextureReference, &ReleaseTextureReference};
    FableAssignmentTexture oldTexture = {&textureVtable};
    FableAssignmentTexture newTexture = {&textureVtable};

    CTextureAssignmentView destination = {};
    destination.PD3DTexture = &oldTexture;
    destination.ByteLength = 0x00123456;
    destination.AllocationSource = 0xA;

    CTextureAssignmentView source = {};
    source.PD3DTexture = &newTexture;
    source.ByteLength = 0x07654321;
    source.AllocationSource = 0x3;

    CTextureAssignmentView* result = &(destination = source);
    if (result != &destination ||
        destination.PD3DTexture != &newTexture ||
        destination.ByteLength != 0x07654321 ||
        destination.AllocationSource != 0x3 ||
        source.PD3DTexture != &newTexture ||
        source.ByteLength != 0x07654321 ||
        source.AllocationSource != 0x3 ||
        g_ReleaseCalls != 1 ||
        g_ReleasedTexture != &oldTexture ||
        g_AddRefCalls != 1 ||
        g_AddedTexture != &newTexture)
    {
        printf("FABLETLC_TEXTURE_ASSIGNMENT_BEHAVIOR FAIL code=1\n");
        return 1;
    }

    CTextureAssignmentView emptySource = {};
    emptySource.ByteLength = 0x000ABCDE;
    emptySource.AllocationSource = 0x7;
    destination = emptySource;
    if (destination.PD3DTexture != 0 ||
        destination.ByteLength != 0x000ABCDE ||
        destination.AllocationSource != 0x7 ||
        g_ReleaseCalls != 2 ||
        g_ReleasedTexture != &newTexture ||
        g_AddRefCalls != 1)
    {
        printf("FABLETLC_TEXTURE_ASSIGNMENT_BEHAVIOR FAIL code=2\n");
        return 2;
    }

    CTextureAssignmentView self = {};
    self.PD3DTexture = &oldTexture;
    self.ByteLength = 0x00013579;
    self.AllocationSource = 0xB;
    self = self;
    if (self.PD3DTexture != 0 ||
        self.ByteLength != 0x00013579 ||
        self.AllocationSource != 0 ||
        g_ReleaseCalls != 3 ||
        g_ReleasedTexture != &oldTexture ||
        g_AddRefCalls != 1)
    {
        printf("FABLETLC_TEXTURE_ASSIGNMENT_BEHAVIOR FAIL code=3\n");
        return 3;
    }

    printf("FABLETLC_TEXTURE_ASSIGNMENT_BEHAVIOR PASS\n");
    return 0;
}
