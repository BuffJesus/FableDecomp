#include <cstddef>
#include <cstdint>
#include <cstdlib>

class CGameScriptInterface;
class CDisplayEngine;
class CDefinitionManager;
class CThingPhysicalSwitchDef;
class CDefStringTable;

template <typename T>
class CDefPointer;

namespace NDisplayView
{
class CViewBase
{
public:
    static CDisplayEngine* __thiscall GetDisplayEngine(CViewBase* pThis);
};
}

extern CDefStringTable DAT_013ca828;

namespace
{
struct CGameScriptInterface_SetOpinionReactionMask_Overlay
{
    std::byte pad_00[0x30];
    std::uint8_t* field_30;
};

static_assert(
    offsetof(CGameScriptInterface_SetOpinionReactionMask_Overlay, field_30) == 0x30);

struct CDisplayEngine_SetOpinionReactionMask_Overlay
{
    std::byte pad_00[0x10];
    CDefinitionManager* definitionManager;
};

static_assert(
    offsetof(CDisplayEngine_SetOpinionReactionMask_Overlay, definitionManager) == 0x10);

struct OpinionReactionMaskTreeNodeOverlay
{
    std::byte pad_00[0x04];
    OpinionReactionMaskTreeNodeOverlay* parent; // +0x04
    OpinionReactionMaskTreeNodeOverlay* left;   // +0x08
    OpinionReactionMaskTreeNodeOverlay* right;  // +0x0C
    std::int32_t index;                         // +0x10
    std::uint8_t value;                         // +0x14
};

static_assert(offsetof(OpinionReactionMaskTreeNodeOverlay, parent) == 0x04);
static_assert(offsetof(OpinionReactionMaskTreeNodeOverlay, left) == 0x08);
static_assert(offsetof(OpinionReactionMaskTreeNodeOverlay, right) == 0x0C);
static_assert(offsetof(OpinionReactionMaskTreeNodeOverlay, index) == 0x10);
static_assert(offsetof(OpinionReactionMaskTreeNodeOverlay, value) == 0x14);

struct OpinionReactionMaskTreeHeaderOverlay
{
    std::byte pad_00[0x08];
    OpinionReactionMaskTreeNodeOverlay* begin; // +0x08
};

static_assert(offsetof(OpinionReactionMaskTreeHeaderOverlay, begin) == 0x08);

struct CThingPhysicalSwitchDef_SetOpinionReactionMask_Overlay
{
    void** vftable;                                  // +0x00
    std::int32_t refCount;                           // +0x04
    std::byte pad_08[0x39 - 0x08];
    std::uint8_t field_39;                           // +0x39
    std::byte pad_3A[0x3C - 0x3A];
    OpinionReactionMaskTreeHeaderOverlay* field_3C; // +0x3C
};

static_assert(
    offsetof(CThingPhysicalSwitchDef_SetOpinionReactionMask_Overlay, refCount) == 0x04);
static_assert(
    offsetof(CThingPhysicalSwitchDef_SetOpinionReactionMask_Overlay, field_39) == 0x39);
static_assert(
    offsetof(CThingPhysicalSwitchDef_SetOpinionReactionMask_Overlay, field_3C) == 0x3C);

struct CDefPointer_SetOpinionReactionMask_Overlay
{
    CThingPhysicalSwitchDef_SetOpinionReactionMask_Overlay* ptr;
};

static_assert(sizeof(CDefPointer_SetOpinionReactionMask_Overlay) == 0x04);

using DestroyDefFn =
    void(__thiscall*)(CThingPhysicalSwitchDef_SetOpinionReactionMask_Overlay*);
}

void __thiscall CGameScriptInterface::SetOpinionReactionMask(int definitionGlobalIndex)
{
    auto* const self =
        reinterpret_cast<CGameScriptInterface_SetOpinionReactionMask_Overlay*>(this);

    int index = 0;
    do
    {
        self->field_30[index] = static_cast<std::uint8_t>(index < 0x39);
        index = index + 1;
    } while (index < 0x4F);

    if (definitionGlobalIndex != 0)
    {
        auto* const displayEngine = reinterpret_cast<CDisplayEngine_SetOpinionReactionMask_Overlay*>(
            NDisplayView::CViewBase::GetDisplayEngine(
                reinterpret_cast<NDisplayView::CViewBase*>(this)));
        CDefinitionManager* const definitionManager = displayEngine->definitionManager;

        CDefPointer_SetOpinionReactionMask_Overlay switchDef{};
        std::byte classTypeNameScratch[4];

        long* const classTypeName = CDefinitionManager::GetDefClassTypeNameFromGlobalIndex(
            definitionManager,
            static_cast<long>(reinterpret_cast<std::uintptr_t>(&classTypeNameScratch[0])));

        char* current =
            CDefStringTable::GetStringAsCharArray(&DAT_013ca828, *classTypeName);

        int remaining = 0x16;
        bool matches = true;
        const char* expected = "OPINION_REACTION_MASK";
        do
        {
            if (remaining == 0)
            {
                break;
            }

            remaining = remaining + -1;
            matches = *current == *expected;
            current = current + 1;
            expected = expected + 1;
        } while (matches);

        if (matches)
        {
            const bool found = CDefinitionManager::GetDef<CThingPhysicalSwitchDef>(
                definitionManager,
                definitionGlobalIndex,
                reinterpret_cast<CDefPointer<CThingPhysicalSwitchDef const>*>(&switchDef));
            auto* const switchDefPtr = switchDef.ptr;

            if (found)
            {
                int fillIndex = 0;
                do
                {
                    self->field_30[fillIndex] = switchDefPtr->field_39;
                    self->field_30[fillIndex + 1] = switchDefPtr->field_39;
                    self->field_30[fillIndex + 2] = switchDefPtr->field_39;
                    fillIndex = fillIndex + 3;
                } while (fillIndex < 0x39);

                auto* const maskBytes =
                    static_cast<std::uint8_t*>(std::malloc(0x4F));
                auto* zeroWords = reinterpret_cast<std::uint32_t*>(maskBytes);
                for (int zeroCount = 0x13; zeroCount != 0; zeroCount = zeroCount + -1)
                {
                    *zeroWords = 0;
                    zeroWords = zeroWords + 1;
                }
                *reinterpret_cast<std::uint16_t*>(zeroWords) = 0;
                *(reinterpret_cast<std::uint8_t*>(zeroWords) + 2) = 0;

                OpinionReactionMaskTreeHeaderOverlay* const treeHeader = switchDefPtr->field_3C;
                auto* node = treeHeader->begin;

                while (node != reinterpret_cast<OpinionReactionMaskTreeNodeOverlay*>(treeHeader))
                {
                    int maskIndex = node->index;
                    if (maskIndex < 0x4F)
                    {
                        maskBytes[maskIndex] = 1;
                        self->field_30[maskIndex] = node->value;
                    }

                    auto* next = node->right;
                    if (next == nullptr)
                    {
                        next = node->parent;
                        if (node == next->right)
                        {
                            do
                            {
                                node = next;
                                next = node->parent;
                            } while (node == next->right);
                        }
                        if (node->right != next)
                        {
                            node = next;
                        }
                    }
                    else
                    {
                        for (auto* left = next->left;; left = left->left)
                        {
                            node = next;
                            if (left == nullptr)
                            {
                                break;
                            }
                            next = left;
                        }
                    }
                }

                if (maskBytes != nullptr)
                {
                    std::free(maskBytes);
                }
            }

            if (switchDefPtr != nullptr)
            {
                std::int32_t* const refCount = &switchDefPtr->refCount;
                *refCount = *refCount + -1;
                if (*refCount == 0)
                {
                    reinterpret_cast<DestroyDefFn>(switchDefPtr->vftable[1])(switchDefPtr);
                }
            }
        }
    }
}