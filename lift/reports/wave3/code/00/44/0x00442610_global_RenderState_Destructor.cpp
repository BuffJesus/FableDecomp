#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct CPatchTesselationEdgeStrip {
    ~CPatchTesselationEdgeStrip();
};

struct CEngineScreenEffectOutlineGlow {
    void FinishWithDynamicTextures();
};

struct CGuiControlTreePane {
    void SortTreeRecursively(void* sort_context);
};

namespace NHeroInformationScreens {
struct CBase;
}

extern void* PTR__vector_deleting_destructor__01231768;

struct RawPoly8 {
    void** vftable;
    std::uint32_t payload;
};

static_assert(sizeof(RawPoly8) == 0x8);

struct RenderStateOverlay {
    void** vftable;                             // 0x00
    RawPoly8* array0Begin;                      // 0x04
    RawPoly8* array0End;                        // 0x08
    std::byte gap0C[0x10 - 0x0C];
    RawPoly8* array1Begin;                      // 0x10
    RawPoly8* array1End;                        // 0x14
    std::byte gap18[0x20 - 0x18];
    alignas(void*) std::byte treePane[0x24];    // 0x20
    std::byte* outlineGlowBegin;                // 0x44
    std::byte* outlineGlowEnd;                  // 0x48
    std::byte gap4C[0x50 - 0x4C];
    std::uint32_t field_50;                     // 0x50
    alignas(void*) std::byte patchStrip[0x64];  // 0x54
    std::byte gapB8[0xB8 - 0x54 - 0x64];
    std::uint32_t field_B8;                     // 0xB8
};

static_assert(offsetof(RenderStateOverlay, vftable) == 0x00);
static_assert(offsetof(RenderStateOverlay, array0Begin) == 0x04);
static_assert(offsetof(RenderStateOverlay, array0End) == 0x08);
static_assert(offsetof(RenderStateOverlay, array1Begin) == 0x10);
static_assert(offsetof(RenderStateOverlay, array1End) == 0x14);
static_assert(offsetof(RenderStateOverlay, treePane) == 0x20);
static_assert(offsetof(RenderStateOverlay, outlineGlowBegin) == 0x44);
static_assert(offsetof(RenderStateOverlay, outlineGlowEnd) == 0x48);
static_assert(offsetof(RenderStateOverlay, field_50) == 0x50);
static_assert(offsetof(RenderStateOverlay, patchStrip) == 0x54);
static_assert(offsetof(RenderStateOverlay, field_B8) == 0xB8);

void __fastcall RenderState_Destructor(NHeroInformationScreens::CBase* param_1)
{
    using RawDeletingDtor = void(__thiscall*)(RawPoly8*, int);
    using BaseTailcall = void(__thiscall*)(NHeroInformationScreens::CBase*);

    auto* self = reinterpret_cast<RenderStateOverlay*>(param_1);

    self->vftable = reinterpret_cast<void**>(&PTR__vector_deleting_destructor__01231768);
    self->field_50 = 0;
    self->field_B8 = 0;

    reinterpret_cast<CPatchTesselationEdgeStrip*>(self->patchStrip)->~CPatchTesselationEdgeStrip();

    for (std::byte* current = reinterpret_cast<std::byte*>(self->outlineGlowBegin);
         current != reinterpret_cast<std::byte*>(self->outlineGlowEnd);
         current += 0x3C) {
        reinterpret_cast<CEngineScreenEffectOutlineGlow*>(current)->FinishWithDynamicTextures();
    }

    if (self->outlineGlowBegin != nullptr) {
        std::free(self->outlineGlowBegin);
    }

    void* unaff_EDI;
    reinterpret_cast<CGuiControlTreePane*>(self->treePane)->SortTreeRecursively(unaff_EDI);

    void* treeMemory = *reinterpret_cast<void**>(self->treePane);
    if (treeMemory != nullptr) {
        std::free(treeMemory);
    }

    for (RawPoly8* current = self->array1Begin; current != self->array1End; current += 1) {
        reinterpret_cast<RawDeletingDtor>(current->vftable[0])(current, 0);
    }

    if (self->array1Begin != nullptr) {
        std::free(self->array1Begin);
    }

    for (RawPoly8* current = self->array0Begin; current != self->array0End; current += 1) {
        reinterpret_cast<RawDeletingDtor>(current->vftable[0])(current, 0);
    }

    if (self->array0Begin != nullptr) {
        std::free(self->array0Begin);
    }

    reinterpret_cast<BaseTailcall>(0x0099A300)(param_1);
}