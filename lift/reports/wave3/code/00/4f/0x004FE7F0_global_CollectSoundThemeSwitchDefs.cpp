#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <new>

namespace
{
struct SoundThemeEntityListOverlay
{
    std::int32_t* begin; // +0x00
    std::int32_t* end;   // +0x04
    std::byte pad08[0x50];
};
static_assert(sizeof(SoundThemeEntityListOverlay) == 0x58);
static_assert(offsetof(SoundThemeEntityListOverlay, begin) == 0x00);
static_assert(offsetof(SoundThemeEntityListOverlay, end) == 0x04);

struct CollectSoundThemeSwitchDefs_ContextOverlay
{
    void* vftable; // +0x00
    std::byte pad04[0x28];
    SoundThemeEntityListOverlay* entityLists; // +0x2C
    std::byte pad30[0x68];
    std::int32_t activeEntityListIndex; // +0x9C
};
static_assert(offsetof(CollectSoundThemeSwitchDefs_ContextOverlay, entityLists) == 0x2C);
static_assert(offsetof(CollectSoundThemeSwitchDefs_ContextOverlay, activeEntityListIndex) == 0x9C);

struct CollectSoundThemeSwitchDefs_ThingOverlay
{
    std::byte pad00[0x1A78];
    void* quarterResThemeGrid; // +0x1A78
};
static_assert(offsetof(CollectSoundThemeSwitchDefs_ThingOverlay, quarterResThemeGrid) == 0x1A78);

struct QuarterResThemeGridOverlay
{
    std::byte pad00[0x04];
    std::byte* cells; // +0x04
    std::byte pad08[0x14];
    std::int32_t width; // +0x1C
    std::int32_t height; // +0x20
};
static_assert(offsetof(QuarterResThemeGridOverlay, cells) == 0x04);
static_assert(offsetof(QuarterResThemeGridOverlay, width) == 0x1C);
static_assert(offsetof(QuarterResThemeGridOverlay, height) == 0x20);

struct SoundThemeValueSetNode
{
    std::byte color; // +0x00
    std::byte pad01[0x03];
    SoundThemeValueSetNode* parent; // +0x04
    SoundThemeValueSetNode* left;   // +0x08
    SoundThemeValueSetNode* right;  // +0x0C
    std::int8_t value;              // +0x10
    std::byte pad11[0x03];
};
static_assert(sizeof(SoundThemeValueSetNode) == 0x14);
static_assert(offsetof(SoundThemeValueSetNode, parent) == 0x04);
static_assert(offsetof(SoundThemeValueSetNode, left) == 0x08);
static_assert(offsetof(SoundThemeValueSetNode, right) == 0x0C);
static_assert(offsetof(SoundThemeValueSetNode, value) == 0x10);

struct SoundThemeValueSetHeader
{
    std::byte color; // +0x00
    std::byte pad01[0x03];
    void* rootListHead;          // +0x04
    SoundThemeValueSetNode* begin; // +0x08
    SoundThemeValueSetNode* end;   // +0x0C
};
static_assert(sizeof(SoundThemeValueSetHeader) == 0x14);
static_assert(offsetof(SoundThemeValueSetHeader, rootListHead) == 0x04);
static_assert(offsetof(SoundThemeValueSetHeader, begin) == 0x08);
static_assert(offsetof(SoundThemeValueSetHeader, end) == 0x0C);

struct SoundThemeValueSetState
{
    SoundThemeValueSetHeader* header; // local_14
    std::int32_t count;               // local_10
};
static_assert(sizeof(SoundThemeValueSetState) == 0x08);
static_assert(offsetof(SoundThemeValueSetState, header) == 0x00);
static_assert(offsetof(SoundThemeValueSetState, count) == 0x04);

struct ThingPhysicalSwitchDefOverlay
{
    void* vftable;          // +0x00
    std::int32_t refCount;  // +0x04
    std::byte pad08[0x38];
    std::int32_t switchId;  // +0x40
};
static_assert(offsetof(ThingPhysicalSwitchDefOverlay, refCount) == 0x04);
static_assert(offsetof(ThingPhysicalSwitchDefOverlay, switchId) == 0x40);

using EntityLookupFn =
    void* (__thiscall*)(CollectSoundThemeSwitchDefs_ContextOverlay* self, std::int32_t entityId);

extern "C" void __thiscall CMemoryAllocatorVariableSize_FindInsertPosition(
    SoundThemeValueSetState* self,
    void* result,
    const void* key);

extern "C" COptimisedPrimitive* __cdecl CollectSoundThemeSwitchDefs_PreLookup(
    CCharString* symbol,
    std::int32_t value);

extern "C" long __thiscall CollectSoundThemeSwitchDefs_GetDefinitionId(
    COptimisedPrimitive* self);

extern "C" void __thiscall CollectSoundThemeSwitchDefs_PostLookup(
    CCharString* symbol);

extern "C" CDefinitionManager* GFGetPreMainMemoryUsed();

extern "C" bool __thiscall std__map__find(
    CollectSoundThemeSwitchDefs_ContextOverlay* self,
    void* result,
    const void* key);

extern "C" void __thiscall LTextBinTree<LTextGroup*>::LTextTreeWalkThrough::BuildTreeArray(
    LTextTreeWalkThrough* self,
    LTextBinNode* node);
}

void __fastcall CollectSoundThemeSwitchDefs(CollectSoundThemeSwitchDefs_ContextOverlay* param_1)
{
    auto* const entityList = reinterpret_cast<SoundThemeEntityListOverlay*>(
        reinterpret_cast<std::byte*>(param_1->entityLists) +
        static_cast<std::size_t>(param_1->activeEntityListIndex) * 0x58);

    SoundThemeValueSetState setState{};
    setState.header = static_cast<SoundThemeValueSetHeader*>(std::malloc(0x14));
    setState.count = 0;

    *reinterpret_cast<std::byte*>(setState.header) = 0;
    setState.header->rootListHead = nullptr;
    setState.header->begin = reinterpret_cast<SoundThemeValueSetNode*>(setState.header);
    setState.header->end = reinterpret_cast<SoundThemeValueSetNode*>(setState.header);

    const std::int32_t entityCount =
        (static_cast<std::int32_t>(reinterpret_cast<std::intptr_t>(entityList->end)) -
         static_cast<std::int32_t>(reinterpret_cast<std::intptr_t>(entityList->begin))) >>
        2;

    std::int32_t entityIndex = 0;
    if (entityCount != 0) {
        do {
            std::int32_t x = 0;

            auto* const thing = reinterpret_cast<CollectSoundThemeSwitchDefs_ThingOverlay*>(
                reinterpret_cast<EntityLookupFn>((*reinterpret_cast<void***>(param_1))[0x13])(
                    param_1,
                    entityList->begin[entityIndex]));

            auto* const grid =
                reinterpret_cast<QuarterResThemeGridOverlay*>(thing->quarterResThemeGrid);

            const std::int32_t width = grid->width;
            const std::int32_t height = grid->height;

            if (0 < width) {
                do {
                    std::int32_t y = 0;
                    if (0 < height) {
                        do {
                            std::int8_t value = *reinterpret_cast<std::int8_t*>(
                                grid->cells +
                                (((grid->height - y) + -1) * grid->width + x) * 7 + 5);

                            std::byte insertResult[8];
                            CMemoryAllocatorVariableSize_FindInsertPosition(
                                &setState,
                                insertResult,
                                &value);

                            y = y + 1;
                        } while (y < height);
                    }
                    x = x + 1;
                } while (x < width);
            }

            entityIndex = entityIndex + 1;
        } while (entityIndex < entityCount);
    }

    auto* node = setState.header->begin;
    if (node != reinterpret_cast<SoundThemeValueSetNode*>(setState.header)) {
        do {
            ThingPhysicalSwitchDefOverlay* def = nullptr;

            alignas(CCharString) std::byte soundThemeStorage[sizeof(CCharString)];
            auto* const soundTheme =
                ::new (soundThemeStorage) CCharString("SOUND_THEME", -1);

            const long definitionId =
                CollectSoundThemeSwitchDefs_GetDefinitionId(
                    CollectSoundThemeSwitchDefs_PreLookup(
                        soundTheme,
                        static_cast<std::int32_t>(node->value)));

            CollectSoundThemeSwitchDefs_PostLookup(soundTheme);

            if (0 < definitionId) {
                auto* const defPointer =
                    reinterpret_cast<CDefPointer<CThingPhysicalSwitchDef const>*>(&def);

                CDefinitionManager* const definitionManager = GFGetPreMainMemoryUsed();
                const bool found =
                    CDefinitionManager::GetDef<CThingPhysicalSwitchDef>(
                        definitionManager,
                        definitionId,
                        defPointer);

                if (found) {
                    const std::int32_t switchId = def->switchId;
                    std::byte findResult[8];
                    std__map__find(param_1, findResult, &switchId);
                }

                if (def != nullptr) {
                    def->refCount = def->refCount + -1;
                    if (def->refCount == 0) {
                        reinterpret_cast<void(__thiscall*)(void*)>(
                            (*reinterpret_cast<void***>(def))[1])(def);
                    }
                }
            }

            auto* next = node->right;
            if (next == nullptr) {
                next = node->parent;
                if (node == next->right) {
                    do {
                        node = next;
                        next = node->parent;
                    } while (node == next->right);
                }
                if (node->right != next) {
                    node = next;
                }
            } else {
                auto* left = next->left;
                node = next;
                while (left != nullptr) {
                    next = left;
                    left = left->left;
                    node = next;
                }
            }
        } while (node != reinterpret_cast<SoundThemeValueSetNode*>(setState.header));
    }

    if (setState.count != 0) {
        void* memory = setState.header->rootListHead;
        while (memory != nullptr) {
            LTextBinTree<LTextGroup*>::LTextTreeWalkThrough::BuildTreeArray(
                reinterpret_cast<LTextTreeWalkThrough*>(&setState.header),
                *reinterpret_cast<LTextBinNode**>(reinterpret_cast<std::byte*>(memory) + 0x0C));

            void* const next = *reinterpret_cast<void**>(reinterpret_cast<std::byte*>(memory) + 0x08);
            std::free(memory);
            memory = next;
        }

        setState.header->begin = reinterpret_cast<SoundThemeValueSetNode*>(setState.header);
        setState.header->rootListHead = nullptr;
        setState.header->end = reinterpret_cast<SoundThemeValueSetNode*>(setState.header);
        setState.count = 0;
    }

    if (setState.header != nullptr) {
        std::free(setState.header);
    }
}