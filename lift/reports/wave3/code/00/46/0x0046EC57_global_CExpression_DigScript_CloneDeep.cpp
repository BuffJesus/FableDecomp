#include <cstddef>
#include <cstdint>

namespace
{
    struct DigScriptNodeOverlay
    {
        std::uint8_t headByte;            // 0x00
        std::uint8_t padding_01[3];       // 0x01
        CScriptBase* parentNode;          // 0x04
        CScriptBase* nextSibling;         // 0x08
        CScriptBase* firstChild;          // 0x0C
        std::byte allocPayload[1];        // 0x10
    };

    static_assert(offsetof(DigScriptNodeOverlay, parentNode) == 0x04);
    static_assert(offsetof(DigScriptNodeOverlay, nextSibling) == 0x08);
    static_assert(offsetof(DigScriptNodeOverlay, firstChild) == 0x0C);
    static_assert(offsetof(DigScriptNodeOverlay, allocPayload) == 0x10);

    struct PointerAndEdx
    {
        CScriptBase* pointer;
        CGameScriptInterfaceBase* edxValue;
    };

#if defined(_M_IX86)
    static PointerAndEdx CallDigScriptAllocObserved(
        CScriptDataBase* scriptData,
        CGameScriptInterfaceBase* gameScriptInterface,
        void* payload) noexcept
    {
        using AllocFn = CScriptBase* (__fastcall*)(CScriptDataBase*, CGameScriptInterfaceBase*, void*);
        AllocFn allocFn = &NScript::CExpression_DigScript::Alloc;

        PointerAndEdx result{};
        __asm
        {
            mov ecx, scriptData
            mov edx, gameScriptInterface
            push payload
            call allocFn
            mov result.pointer, eax
            mov result.edxValue, edx
        }
        return result;
    }

    static PointerAndEdx CallCloneDeepObserved(
        CScriptDataBase* scriptData,
        CGameScriptInterfaceBase* gameScriptInterface,
        CScriptBase* source,
        CScriptBase* parent) noexcept
    {
        using CloneFn = CScriptBase* (__fastcall*)(CScriptDataBase*, CGameScriptInterfaceBase*, CScriptBase*, CScriptBase*);
        CloneFn cloneFn = &CExpression_DigScript_CloneDeep;

        PointerAndEdx result{};
        __asm
        {
            mov ecx, scriptData
            mov edx, gameScriptInterface
            push parent
            push source
            call cloneFn
            mov result.pointer, eax
            mov result.edxValue, edx
        }
        return result;
    }
#endif
}

CScriptBase* __fastcall CExpression_DigScript_CloneDeep(
    CScriptDataBase* scriptData,
    CGameScriptInterfaceBase* gameScriptInterface,
    CScriptBase* source,
    CScriptBase* parent)
{
    auto* const sourceNode = reinterpret_cast<DigScriptNodeOverlay*>(source);

#if defined(_M_IX86)
    const PointerAndEdx rootAlloc = CallDigScriptAllocObserved(
        scriptData,
        gameScriptInterface,
        &sourceNode->allocPayload[0]);
#else
    const PointerAndEdx rootAlloc{
        NScript::CExpression_DigScript::Alloc(scriptData, gameScriptInterface, &sourceNode->allocPayload[0]),
        gameScriptInterface
    };
#endif

    auto* const clone = reinterpret_cast<DigScriptNodeOverlay*>(rootAlloc.pointer);
    clone->nextSibling = nullptr;
    clone->firstChild = nullptr;
    clone->headByte = sourceNode->headByte;
    clone->parentNode = parent;

    CGameScriptInterfaceBase* currentInterface = rootAlloc.edxValue;

    if (sourceNode->firstChild != nullptr)
    {
#if defined(_M_IX86)
        const PointerAndEdx childClone = CallCloneDeepObserved(
            scriptData,
            currentInterface,
            sourceNode->firstChild,
            reinterpret_cast<CScriptBase*>(clone));
#else
        const PointerAndEdx childClone{
            CExpression_DigScript_CloneDeep(
                scriptData,
                currentInterface,
                sourceNode->firstChild,
                reinterpret_cast<CScriptBase*>(clone)),
            currentInterface
        };
#endif
        currentInterface = childClone.edxValue;
        clone->firstChild = childClone.pointer;
    }

    CScriptBase* tail = reinterpret_cast<CScriptBase*>(clone);
    for (auto* sibling = reinterpret_cast<DigScriptNodeOverlay*>(sourceNode->nextSibling);
         sibling != nullptr;
         sibling = reinterpret_cast<DigScriptNodeOverlay*>(sibling->nextSibling))
    {
#if defined(_M_IX86)
        const PointerAndEdx siblingAlloc = CallDigScriptAllocObserved(
            scriptData,
            currentInterface,
            &sibling->allocPayload[0]);
#else
        const PointerAndEdx siblingAlloc{
            NScript::CExpression_DigScript::Alloc(scriptData, currentInterface, &sibling->allocPayload[0]),
            currentInterface
        };
#endif

        auto* const siblingClone = reinterpret_cast<DigScriptNodeOverlay*>(siblingAlloc.pointer);
        siblingClone->nextSibling = nullptr;
        siblingClone->firstChild = nullptr;
        siblingClone->headByte = sibling->headByte;
        reinterpret_cast<DigScriptNodeOverlay*>(tail)->nextSibling = reinterpret_cast<CScriptBase*>(siblingClone);
        siblingClone->parentNode = tail;

        currentInterface = siblingAlloc.edxValue;

        if (sibling->firstChild != nullptr)
        {
#if defined(_M_IX86)
            const PointerAndEdx siblingChildClone = CallCloneDeepObserved(
                scriptData,
                currentInterface,
                sibling->firstChild,
                reinterpret_cast<CScriptBase*>(siblingClone));
#else
            const PointerAndEdx siblingChildClone{
                CExpression_DigScript_CloneDeep(
                    scriptData,
                    currentInterface,
                    sibling->firstChild,
                    reinterpret_cast<CScriptBase*>(siblingClone)),
                currentInterface
            };
#endif
            currentInterface = siblingChildClone.edxValue;
            siblingClone->firstChild = siblingChildClone.pointer;
        }

        tail = reinterpret_cast<CScriptBase*>(siblingClone);
    }

    return reinterpret_cast<CScriptBase*>(clone);
}