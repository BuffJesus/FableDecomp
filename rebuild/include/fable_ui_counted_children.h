#pragma once
#include "fable_ui_component_draw.h"

// Retail operator delete boundary, 00BFE9BC. The fixture supplies an observer;
// integration must supply the engine allocator's matching deallocation service.
void __cdecl FableUiDeleteReference(FableReferenceCount*);
// Same retail malloc/free services as deletion lists, with vector-sized blocks.
void* __cdecl FableUiAllocateChildStorage(unsigned bytes);
void __cdecl FableUiFreeChildStorage(void*);

inline void FableUiReleaseChildReference(FableUiComponentCountedStorage* entry)
{
    if (entry->Info)
    {
        --entry->Info->owners;
        if (entry->Info->owners == 0)
        {
            entry->Info->destroy(entry->Info->object);
            FableUiDeleteReference(entry->Info);
        }
    }
}

FableUiComponentCountedStorage* __fastcall FableUiFindCountedChild(
    FableUiComponentCountedStorage* begin, FableUiComponentCountedStorage* end,
    const FableUiComponentCountedStorage* value, void* iteratorTag);
FableUiComponentCountedStorage* __fastcall FableUiMoveCountedChildren(
    FableUiComponentCountedStorage* begin, FableUiComponentCountedStorage* end,
    FableUiComponentCountedStorage* destination, void* iteratorTag, unsigned);
FableUiComponentCountedStorage* __fastcall FableUiEraseCountedChild(
    FableUiComponentChildrenStorage*, void*, FableUiComponentCountedStorage*);
void __fastcall FableUiReallocateCountedChildren(FableUiComponentChildrenStorage*, void*,
    FableUiComponentCountedStorage* position, const FableUiComponentCountedStorage* value,
    void* iteratorTag, unsigned count, bool omitSuffix);
void __fastcall FableUiRemoveChildAt(FableUiComponentDrawView*, void*, unsigned index);

inline void FableUiConstructCountedChild(FableUiComponentCountedStorage* output, const FableUiComponentCountedStorage* source)
{
    if (output)
    {
        output->Data = source->Data;
        output->Info = source->Info;
        if (output->Info) ++output->Info->owners;
    }
}

// Completion/removal tail of base Update's retiring-child loop, 00532C0D..CD9.
// The caller must first complete this child's colour/update pass. Retail's outer
// loop increments its index even after erasure, skipping the shifted successor.
inline bool FableUiFinishRetiringChild(FableUiComponentDrawView* parent, unsigned index)
{
    FableUiComponentDrawView* child = parent->ChildrenToDelete.Begin[index].Data;
    if (!child->Vtable->HasCompletedStateChange(child, 0)) return false;
    child = parent->ChildrenToDelete.Begin[index].Data;
    if (child->Vtable->GetCurrentState(child, 0) != 2) return false;
    FableUiComponentCountedStorage* entry = FableUiFindCountedChild(parent->ChildrenToDelete.Begin,
        parent->ChildrenToDelete.End, &parent->ChildrenToDelete.Begin[index], 0);
    child = entry->Data;
    if (child->Deletion.Method == 0) child->Vtable->RequestState(child, 0, 0);
    child = entry->Data;
    child->Vtable->SetParent(child, 0, 0);
    if (entry + 1 != parent->ChildrenToDelete.End)
        FableUiMoveCountedChildren(entry + 1, parent->ChildrenToDelete.End, entry, 0, 0);
    entry = --parent->ChildrenToDelete.End;
    FableUiReleaseChildReference(entry);
    entry->Data = 0;
    entry->Info = 0;
    return true;
}
