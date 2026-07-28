#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <memory>

struct CGuiControlTreeNode;
template <typename T, typename Allocator>
class list;

struct CBase {
    void** vftable;
};

struct CTexture {
    static void Uninitialise(CTexture* texture);
};

struct CGuiControlTreePane {
    static void SortTreeRecursively(
        CGuiControlTreePane* pane,
        list<CGuiControlTreeNode, std::allocator<CGuiControlTreeNode>>* nodes);
};

struct CNavigatorQueueManager {
    static void FrameUpdate(CNavigatorQueueManager* manager);
};

namespace NHeroInformationScreens {
struct CBase : ::CBase {
    static void CBase(::CBase* self);
};
}

extern void TextureCache_Destructor();
extern void operator_delete(void* ptr);

extern int* DAT_013b878c;
extern void* PTR__vector_deleting_destructor__0123177c;
extern void* PTR__scalar_deleting_destructor__0122f598;
extern void* PTR_LAB_01231720;

namespace {

using FreeThunk = void(__cdecl*)();
using ThisThunk = void(__thiscall*)(void*);
using ThisThunkWithArg = void(__thiscall*)(void*, void*);

struct HeroInfoTempAllocation {
    std::uint8_t pad_00[0x04];
    void* field_04;
    std::uint8_t pad_08[0x08];
    void* field_10;
    std::uint8_t pad_14[0x08];
    void* field_1C;
};
static_assert(offsetof(HeroInfoTempAllocation, field_04) == 0x04);
static_assert(offsetof(HeroInfoTempAllocation, field_10) == 0x10);
static_assert(offsetof(HeroInfoTempAllocation, field_1C) == 0x1C);

struct RawCallbackEntry;
using CallbackThunk = void(__thiscall*)(RawCallbackEntry*, void*);

struct RawCallbackEntry {
    CallbackThunk callback;
    std::uint32_t unused_04;
};
static_assert(sizeof(RawCallbackEntry) == 0x08);

struct RawRefCountedBlock {
    std::int32_t refCount;
    FreeThunk destroy;
};
static_assert(offsetof(RawRefCountedBlock, refCount) == 0x00);
static_assert(offsetof(RawRefCountedBlock, destroy) == 0x04);

struct VftableRefObject {
    void** vftable;
    std::int32_t refCount;
};
static_assert(offsetof(VftableRefObject, vftable) == 0x00);
static_assert(offsetof(VftableRefObject, refCount) == 0x04);

struct HeroInformationScreensOverlay {
    void* vftable;                 // 0x000
    std::uint8_t pad_004[0x134];
    void* field_138;               // 0x138
    void* field_13C;               // 0x13C
    void* field_140;               // 0x140
    void* field_144;               // 0x144
    void* field_148;               // 0x148
    void* field_14C;               // 0x14C
    void* field_150;               // 0x150
    void* field_154;               // 0x154
    void* field_158;               // 0x158
    void* field_15C;               // 0x15C
    void* field_160;               // 0x160
    void* field_164;               // 0x164
    void* field_168;               // 0x168
    void* field_16C;               // 0x16C
    void* field_170;               // 0x170
    void* field_174;               // 0x174
    void* field_178;               // 0x178
    void* field_17C;               // 0x17C
    void* field_180;               // 0x180
    void* field_184;               // 0x184
    void* field_188;               // 0x188
    void* field_18C;               // 0x18C
    void* field_190;               // 0x190
    void* field_194;               // 0x194
    void* field_198;               // 0x198
    void* field_19C;               // 0x19C
    void* field_1A0;               // 0x1A0
    void* field_1A4;               // 0x1A4
    void* field_1A8;               // 0x1A8
    void* field_1AC;               // 0x1AC
    void* field_1B0;               // 0x1B0
    void* field_1B4;               // 0x1B4
    void* field_1B8;               // 0x1B8
    void* field_1BC;               // 0x1BC
    void* field_1C0;               // 0x1C0
    void* field_1C4;               // 0x1C4
    void* field_1C8;               // 0x1C8
    void* field_1CC;               // 0x1CC
    void* field_1D0;               // 0x1D0
    void* field_1D4;               // 0x1D4
    void* field_1D8;               // 0x1D8
    void* field_1DC;               // 0x1DC
    void* field_1E0;               // 0x1E0
    void* field_1E4;               // 0x1E4
    void* field_1E8;               // 0x1E8
    void* field_1EC;               // 0x1EC
    void* field_1F0;               // 0x1F0
    void* field_1F4;               // 0x1F4
    void* field_1F8;               // 0x1F8
    void* field_1FC;               // 0x1FC
    void* field_200;               // 0x200
    void* field_204;               // 0x204
    void* field_208;               // 0x208
    void* field_20C;               // 0x20C
    void* field_210;               // 0x210
    void* field_214;               // 0x214
    void* field_218;               // 0x218
    void* field_21C;               // 0x21C
    void* field_220;               // 0x220
    void* field_224;               // 0x224
    void* field_228;               // 0x228
    void* field_22C;               // 0x22C
    void* field_230;               // 0x230
    void* field_234;               // 0x234
    void* field_238;               // 0x238
    void* field_23C;               // 0x23C
    void* field_240;               // 0x240
    void* field_244;               // 0x244
    void* field_248;               // 0x248
    void* field_24C;               // 0x24C
    void* field_250;               // 0x250
    void* field_254;               // 0x254
    void* field_258;               // 0x258
    void* field_25C;               // 0x25C
    void* field_260;               // 0x260
    void* field_264;               // 0x264
    void* field_268;               // 0x268
    void* field_26C;               // 0x26C
    void* field_270;               // 0x270
    void* field_274;               // 0x274
    void* field_278;               // 0x278
    void* field_27C;               // 0x27C
    void* field_280;               // 0x280
    void* field_284;               // 0x284
    void* field_288;               // 0x288
    void* field_28C;               // 0x28C
    void* field_290;               // 0x290
    void* field_294;               // 0x294
    void* field_298;               // 0x298
    void* field_29C;               // 0x29C
    void* field_2A0;               // 0x2A0
    void* field_2A4;               // 0x2A4
    void* field_2A8;               // 0x2A8
    void* field_2AC;               // 0x2AC
    void* field_2B0;               // 0x2B0
    CTexture texture_2B4;          // 0x2B4
    std::uint8_t pad_2B8[0x08];
    void* field_2C0;               // 0x2C0
    std::uint8_t pad_2C4[0x08];
    void* field_2CC;               // 0x2CC
    void* field_2D0;               // 0x2D0
    void* field_2D4;               // 0x2D4
    void* field_2D8;               // 0x2D8
    void* field_2DC;               // 0x2DC
    void* field_2E0;               // 0x2E0
    void* field_2E4;               // 0x2E4
    void* field_2E8;               // 0x2E8
    void* field_2EC;               // 0x2EC
    void* field_2F0;               // 0x2F0
    void* field_2F4;               // 0x2F4
    void* field_2F8;               // 0x2F8
    void* field_2FC;               // 0x2FC
    void* field_300;               // 0x300
    RawCallbackEntry* callbackArrayBegin_304; // 0x304
    RawCallbackEntry* callbackArrayEnd_308;   // 0x308
    void* field_30C;               // 0x30C
    RawCallbackEntry* callbackArrayBegin_310; // 0x310
    RawCallbackEntry* callbackArrayEnd_314;   // 0x314
    ::CBase base_31C;              // 0x31C
    void* baseOwner_320;           // 0x320
    void* queueStorage_324;        // 0x324
    void* treeStorage_328;         // 0x328
    HeroInfoTempAllocation* tempAlloc_32C; // 0x32C
};
static_assert(offsetof(HeroInformationScreensOverlay, field_138) == 0x138);
static_assert(offsetof(HeroInformationScreensOverlay, field_13C) == 0x13C);
static_assert(offsetof(HeroInformationScreensOverlay, field_140) == 0x140);
static_assert(offsetof(HeroInformationScreensOverlay, field_14C) == 0x14C);
static_assert(offsetof(HeroInformationScreensOverlay, texture_2B4) == 0x2B4);
static_assert(offsetof(HeroInformationScreensOverlay, field_2C0) == 0x2C0);
static_assert(offsetof(HeroInformationScreensOverlay, field_2CC) == 0x2CC);
static_assert(offsetof(HeroInformationScreensOverlay, field_300) == 0x300);
static_assert(offsetof(HeroInformationScreensOverlay, callbackArrayBegin_304) == 0x304);
static_assert(offsetof(HeroInformationScreensOverlay, callbackArrayBegin_310) == 0x310);
static_assert(offsetof(HeroInformationScreensOverlay, base_31C) == 0x31C);
static_assert(offsetof(HeroInformationScreensOverlay, baseOwner_320) == 0x320);
static_assert(offsetof(HeroInformationScreensOverlay, queueStorage_324) == 0x324);
static_assert(offsetof(HeroInformationScreensOverlay, treeStorage_328) == 0x328);
static_assert(offsetof(HeroInformationScreensOverlay, tempAlloc_32C) == 0x32C);

} // namespace

void __fastcall NHeroInformationScreens_Destructor(void* selfVoid) {
    auto* self = static_cast<HeroInformationScreensOverlay*>(selfVoid);

    self->vftable = &PTR__vector_deleting_destructor__0123177c;

    auto* globalRef = reinterpret_cast<VftableRefObject*>(DAT_013b878c);
    if (globalRef != nullptr) {
        const int oldCount = globalRef->refCount;
        globalRef->refCount = oldCount - 1;
        if (oldCount - 1 == 0) {
            reinterpret_cast<ThisThunk>(globalRef->vftable[1])(globalRef);
        }
    }
    DAT_013b878c = nullptr;

    HeroInfoTempAllocation* tempAlloc = self->tempAlloc_32C;
    if (tempAlloc != nullptr) {
        if (tempAlloc->field_1C != nullptr) {
            std::free(tempAlloc->field_1C);
        }
        if (tempAlloc->field_10 != nullptr) {
            std::free(tempAlloc->field_10);
        }
        if (tempAlloc->field_04 != nullptr) {
            std::free(tempAlloc->field_04);
        }
        operator_delete(tempAlloc);
    }

    CGuiControlTreePane::SortTreeRecursively(
        reinterpret_cast<CGuiControlTreePane*>(&self->treeStorage_328),
        static_cast<list<CGuiControlTreeNode, std::allocator<CGuiControlTreeNode>>*>(nullptr));

    if (self->treeStorage_328 != nullptr) {
        std::free(self->treeStorage_328);
    }

    CNavigatorQueueManager::FrameUpdate(
        reinterpret_cast<CNavigatorQueueManager*>(&self->queueStorage_324));

    if (self->queueStorage_324 != nullptr) {
        std::free(self->queueStorage_324);
    }

    self->base_31C.vftable = reinterpret_cast<void**>(&PTR__scalar_deleting_destructor__0122f598);

    auto* baseOwner = reinterpret_cast<VftableRefObject*>(self->baseOwner_320);
    if (baseOwner != nullptr && baseOwner->refCount == 1) {
        reinterpret_cast<ThisThunkWithArg>(baseOwner->vftable[3])(baseOwner, &self->base_31C);
    }

    baseOwner = reinterpret_cast<VftableRefObject*>(self->baseOwner_320);
    if (baseOwner != nullptr) {
        --baseOwner->refCount;
        if (baseOwner->refCount == 0) {
            reinterpret_cast<ThisThunk>(baseOwner->vftable[1])(baseOwner);
        }
        self->baseOwner_320 = nullptr;
    }
    self->baseOwner_320 = nullptr;

    NHeroInformationScreens::CBase::CBase(&self->base_31C);

    {
        RawCallbackEntry* it = self->callbackArrayBegin_310;
        RawCallbackEntry* end = self->callbackArrayEnd_314;
        while (it != end) {
            it->callback(it, nullptr);
            ++it;
        }
    }
    if (self->callbackArrayBegin_310 != nullptr) {
        std::free(self->callbackArrayBegin_310);
    }

    {
        RawCallbackEntry* it = self->callbackArrayBegin_304;
        RawCallbackEntry* end = self->callbackArrayEnd_308;
        while (it != end) {
            it->callback(it, nullptr);
            ++it;
        }
    }
    if (self->callbackArrayBegin_304 != nullptr) {
        std::free(self->callbackArrayBegin_304);
    }

    {
        auto* block = reinterpret_cast<RawRefCountedBlock*>(self->field_300);
        if (block != nullptr) {
            --block->refCount;
            if (block->refCount == 0) {
                block->destroy();
                operator_delete(self->field_300);
            }
        }
        self->field_2FC = nullptr;
        self->field_300 = nullptr;
    }

    {
        auto* block = reinterpret_cast<RawRefCountedBlock*>(self->field_2F8);
        if (block != nullptr) {
            --block->refCount;
            if (block->refCount == 0) {
                block->destroy();
                operator_delete(self->field_2F8);
            }
        }
        self->field_2F4 = nullptr;
        self->field_2F8 = nullptr;
    }

    {
        auto* block = reinterpret_cast<RawRefCountedBlock*>(self->field_2F0);
        if (block != nullptr) {
            --block->refCount;
            if (block->refCount == 0) {
                block->destroy();
                operator_delete(self->field_2F0);
            }
        }
        self->field_2EC = nullptr;
        self->field_2F0 = nullptr;
    }

    {
        auto* block = reinterpret_cast<RawRefCountedBlock*>(self->field_2E8);
        if (block != nullptr) {
            --block->refCount;
            if (block->refCount == 0) {
                block->destroy();
                operator_delete(self->field_2E8);
            }
        }
        self->field_2E4 = nullptr;
        self->field_2E8 = nullptr;
    }

    {
        auto* block = reinterpret_cast<RawRefCountedBlock*>(self->field_2E0);
        if (block != nullptr) {
            --block->refCount;
            if (block->refCount == 0) {
                block->destroy();
                operator_delete(self->field_2E0);
            }
        }
        self->field_2DC = nullptr;
        self->field_2E0 = nullptr;
    }

    {
        auto* block = reinterpret_cast<RawRefCountedBlock*>(self->field_2D8);
        if (block != nullptr) {
            --block->refCount;
            if (block->refCount == 0) {
                block->destroy();
                operator_delete(self->field_2D8);
            }
        }
        self->field_2D4 = nullptr;
        self->field_2D8 = nullptr;
    }

    {
        auto* block = reinterpret_cast<RawRefCountedBlock*>(self->field_2D0);
        if (block != nullptr) {
            --block->refCount;
            if (block->refCount == 0) {
                block->destroy();
                operator_delete(self->field_2D0);
            }
        }
        self->field_2CC = nullptr;
        self->field_2D0 = nullptr;
    }

    if (self->field_2C0 != nullptr) {
        std::free(self->field_2C0);
    }

    CTexture::Uninitialise(&self->texture_2B4);

    if (self->field_26C != nullptr) {
        std::free(self->field_26C);
    }
    if (self->field_260 != nullptr) {
        std::free(self->field_260);
    }
    if (self->field_254 != nullptr) {
        std::free(self->field_254);
    }

    {
        auto* block = reinterpret_cast<RawRefCountedBlock*>(self->field_24C);
        if (block != nullptr) {
            --block->refCount;
            if (block->refCount == 0) {
                block->destroy();
                operator_delete(self->field_24C);
            }
        }
        self->field_248 = nullptr;
        self->field_24C = nullptr;
    }

    {
        auto* block = reinterpret_cast<RawRefCountedBlock*>(self->field_244);
        if (block != nullptr) {
            --block->refCount;
            if (block->refCount == 0) {
                block->destroy();
                operator_delete(self->field_244);
            }
        }
        self->field_240 = nullptr;
        self->field_244 = nullptr;
    }

    {
        auto* block = reinterpret_cast<RawRefCountedBlock*>(self->field_23C);
        if (block != nullptr) {
            --block->refCount;
            if (block->refCount == 0) {
                block->destroy();
                operator_delete(self->field_23C);
            }
        }
        self->field_238 = nullptr;
        self->field_23C = nullptr;
    }

    {
        auto* block = reinterpret_cast<RawRefCountedBlock*>(self->field_234);
        if (block != nullptr) {
            --block->refCount;
            if (block->refCount == 0) {
                block->destroy();
                operator_delete(self->field_234);
            }
        }
        self->field_230 = nullptr;
        self->field_234 = nullptr;
    }

    {
        auto* block = reinterpret_cast<RawRefCountedBlock*>(self->field_22C);
        if (block != nullptr) {
            --block->refCount;
            if (block->refCount == 0) {
                block->destroy();
                operator_delete(self->field_22C);
            }
        }
        self->field_228 = nullptr;
        self->field_22C = nullptr;
    }

    {
        auto* block = reinterpret_cast<RawRefCountedBlock*>(self->field_224);
        if (block != nullptr) {
            --block->refCount;
            if (block->refCount == 0) {
                block->destroy();
                operator_delete(self->field_224);
            }
        }
        self->field_220 = nullptr;
        self->field_224 = nullptr;
    }

    {
        auto* block = reinterpret_cast<RawRefCountedBlock*>(self->field_21C);
        if (block != nullptr) {
            --block->refCount;
            if (block->refCount == 0) {
                block->destroy();
                operator_delete(self->field_21C);
            }
        }
        self->field_218 = nullptr;
        self->field_21C = nullptr;
    }

    {
        auto* block = reinterpret_cast<RawRefCountedBlock*>(self->field_214);
        if (block != nullptr) {
            --block->refCount;
            if (block->refCount == 0) {
                block->destroy();
                operator_delete(self->field_214);
            }
        }
        self->field_210 = nullptr;
        self->field_214 = nullptr;
    }

    {
        auto* block = reinterpret_cast<RawRefCountedBlock*>(self->field_20C);
        if (block != nullptr) {
            --block->refCount;
            if (block->refCount == 0) {
                block->destroy();
                operator_delete(self->field_20C);
            }
        }
        self->field_208 = nullptr;
        self->field_20C = nullptr;
    }

    {
        auto* block = reinterpret_cast<RawRefCountedBlock*>(self->field_204);
        if (block != nullptr) {
            --block->refCount;
            if (block->refCount == 0) {
                block->destroy();
                operator_delete(self->field_204);
            }
        }
        self->field_200 = nullptr;
        self->field_204 = nullptr;
    }

    {
        auto* block = reinterpret_cast<RawRefCountedBlock*>(self->field_1FC);
        if (block != nullptr) {
            --block->refCount;
            if (block->refCount == 0) {
                block->destroy();
                operator_delete(self->field_1FC);
            }
        }
        self->field_1F8 = nullptr;
        self->field_1FC = nullptr;
    }

    {
        auto* block = reinterpret_cast<RawRefCountedBlock*>(self->field_1F4);
        if (block != nullptr) {
            --block->refCount;
            if (block->refCount == 0) {
                block->destroy();
                operator_delete(self->field_1F4);
            }
        }
        self->field_1F0 = nullptr;
        self->field_1F4 = nullptr;
    }

    {
        auto* block = reinterpret_cast<RawRefCountedBlock*>(self->field_1EC);
        if (block != nullptr) {
            --block->refCount;
            if (block->refCount == 0) {
                block->destroy();
                operator_delete(self->field_1EC);
            }
        }
        self->field_1E8 = nullptr;
        self->field_1EC = nullptr;
    }

    {
        auto* block = reinterpret_cast<RawRefCountedBlock*>(self->field_1E4);
        if (block != nullptr) {
            --block->refCount;
            if (block->refCount == 0) {
                block->destroy();
                operator_delete(self->field_1E4);
            }
        }
        self->field_1E0 = nullptr;
        self->field_1E4 = nullptr;
    }

    {
        auto* block = reinterpret_cast<RawRefCountedBlock*>(self->field_1DC);
        if (block != nullptr) {
            --block->refCount;
            if (block->refCount == 0) {
                block->destroy();
                operator_delete(self->field_1DC);
            }
        }
        self->field_1D8 = nullptr;
        self->field_1DC = nullptr;
    }

    {
        auto* block = reinterpret_cast<RawRefCountedBlock*>(self->field_1D4);
        if (block != nullptr) {
            --block->refCount;
            if (block->refCount == 0) {
                block->destroy();
                operator_delete(self->field_1D4);
            }
        }
        self->field_1D0 = nullptr;
        self->field_1D4 = nullptr;
    }

    {
        auto* block = reinterpret_cast<RawRefCountedBlock*>(self->field_1CC);
        if (block != nullptr) {
            --block->refCount;
            if (block->refCount == 0) {
                block->destroy();
                operator_delete(self->field_1CC);
            }
        }
        self->field_1C8 = nullptr;
        self->field_1CC = nullptr;
    }

    {
        auto* block = reinterpret_cast<RawRefCountedBlock*>(self->field_1C4);
        if (block != nullptr) {
            --block->refCount;
            if (block->refCount == 0) {
                block->destroy();
                operator_delete(self->field_1C4);
            }
        }
        self->field_1C0 = nullptr;
        self->field_1C4 = nullptr;
    }

    {
        auto* block = reinterpret_cast<RawRefCountedBlock*>(self->field_1BC);
        if (block != nullptr) {
            --block->refCount;
            if (block->refCount == 0) {
                block->destroy();
                operator_delete(self->field_1BC);
            }
        }
        self->field_1B8 = nullptr;
        self->field_1BC = nullptr;
    }

    {
        auto* block = reinterpret_cast<RawRefCountedBlock*>(self->field_1B4);
        if (block != nullptr) {
            --block->refCount;
            if (block->refCount == 0) {
                block->destroy();
                operator_delete(self->field_1B4);
            }
        }
        self->field_1B0 = nullptr;
        self->field_1B4 = nullptr;
    }

    {
        auto* block = reinterpret_cast<RawRefCountedBlock*>(self->field_1AC);
        if (block != nullptr) {
            --block->refCount;
            if (block->refCount == 0) {
                block->destroy();
                operator_delete(self->field_1AC);
            }
        }
        self->field_1A8 = nullptr;
        self->field_1AC = nullptr;
    }

    {
        auto* block = reinterpret_cast<RawRefCountedBlock*>(self->field_1A4);
        if (block != nullptr) {
            --block->refCount;
            if (block->refCount == 0) {
                block->destroy();
                operator_delete(self->field_1A4);
            }
        }
        self->field_1A0 = nullptr;
        self->field_1A4 = nullptr;
    }

    {
        auto* block = reinterpret_cast<RawRefCountedBlock*>(self->field_19C);
        if (block != nullptr) {
            --block->refCount;
            if (block->refCount == 0) {
                block->destroy();
                operator_delete(self->field_19C);
            }
        }
        self->field_198 = nullptr;
        self->field_19C = nullptr;
    }

    {
        auto* block = reinterpret_cast<RawRefCountedBlock*>(self->field_194);
        if (block != nullptr) {
            --block->refCount;
            if (block->refCount == 0) {
                block->destroy();
                operator_delete(self->field_194);
            }
        }
        self->field_190 = nullptr;
        self->field_194 = nullptr;
    }

    {
        auto* block = reinterpret_cast<RawRefCountedBlock*>(self->field_18C);
        if (block != nullptr) {
            --block->refCount;
            if (block->refCount == 0) {
                block->destroy();
                operator_delete(self->field_18C);
            }
        }
        self->field_188 = nullptr;
        self->field_18C = nullptr;
    }

    {
        auto* block = reinterpret_cast<RawRefCountedBlock*>(self->field_184);
        if (block != nullptr) {
            --block->refCount;
            if (block->refCount == 0) {
                block->destroy();
                operator_delete(self->field_184);
            }
        }
        self->field_180 = nullptr;
        self->field_184 = nullptr;
    }

    {
        auto* block = reinterpret_cast<RawRefCountedBlock*>(self->field_17C);
        if (block != nullptr) {
            --block->refCount;
            if (block->refCount == 0) {
                block->destroy();
                operator_delete(self->field_17C);
            }
        }
        self->field_178 = nullptr;
        self->field_17C = nullptr;
    }

    {
        auto* block = reinterpret_cast<RawRefCountedBlock*>(self->field_174);
        if (block != nullptr) {
            --block->refCount;
            if (block->refCount == 0) {
                block->destroy();
                operator_delete(self->field_174);
            }
        }
        self->field_170 = nullptr;
        self->field_174 = nullptr;
    }

    {
        auto* block = reinterpret_cast<RawRefCountedBlock*>(self->field_16C);
        if (block != nullptr) {
            --block->refCount;
            if (block->refCount == 0) {
                block->destroy();
                operator_delete(self->field_16C);
            }
        }
        self->field_168 = nullptr;
        self->field_16C = nullptr;
    }

    {
        auto* block = reinterpret_cast<RawRefCountedBlock*>(self->field_164);
        if (block != nullptr) {
            --block->refCount;
            if (block->refCount == 0) {
                block->destroy();
                operator_delete(self->field_164);
            }
        }
        self->field_160 = nullptr;
        self->field_164 = nullptr;
    }

    {
        auto* block = reinterpret_cast<RawRefCountedBlock*>(self->field_15C);
        if (block != nullptr) {
            --block->refCount;
            if (block->refCount == 0) {
                block->destroy();
                operator_delete(self->field_15C);
            }
        }
        self->field_158 = nullptr;
        self->field_15C = nullptr;
    }

    {
        auto* block = reinterpret_cast<RawRefCountedBlock*>(self->field_154);
        if (block != nullptr) {
            --block->refCount;
            if (block->refCount == 0) {
                block->destroy();
                operator_delete(self->field_154);
            }
        }
        self->field_150 = nullptr;
        self->field_154 = nullptr;
    }

    {
        auto* block = reinterpret_cast<RawRefCountedBlock*>(self->field_14C);
        if (block != nullptr) {
            --block->refCount;
            if (block->refCount == 0) {
                block->destroy();
                operator_delete(self->field_14C);
            }
        }
        self->field_148 = nullptr;
        self->field_14C = nullptr;
    }

    {
        auto* block = reinterpret_cast<RawRefCountedBlock*>(self->field_144);
        if (block != nullptr) {
            --block->refCount;
            if (block->refCount == 0) {
                block->destroy();
                operator_delete(self->field_144);
            }
        }
        self->field_140 = nullptr;
        self->field_144 = nullptr;
    }

    {
        auto* block = reinterpret_cast<RawRefCountedBlock*>(self->field_13C);
        if (block != nullptr) {
            --block->refCount;
            if (block->refCount == 0) {
                block->destroy();
                operator_delete(self->field_13C);
            }
        }
        self->field_138 = nullptr;
        self->field_13C = nullptr;
    }

    TextureCache_Destructor();
    self->vftable = &PTR_LAB_01231720;
}