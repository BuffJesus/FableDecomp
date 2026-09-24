#include "fable_ui_counted_children.h"
#include <string.h>

void __fastcall FableUiRemoveChildAt(FableUiComponentDrawView* parent, void*, unsigned index)
{
    FableUiComponentCountedStorage* entry = FableUiFindCountedChild(parent->Children.Begin,
        parent->Children.End, &parent->Children.Begin[index], 0);
    entry->Data->Vtable->Die(entry->Data, 0);
    if (parent->ChildrenToDelete.End != parent->ChildrenToDelete.CapacityEnd)
    {
        FableUiConstructCountedChild(parent->ChildrenToDelete.End, entry);
        ++parent->ChildrenToDelete.End;
    }
    else
        FableUiReallocateCountedChildren(&parent->ChildrenToDelete, 0,
            parent->ChildrenToDelete.End, entry, 0, 1, true);
    FableUiEraseCountedChild(&parent->Children, 0, entry);

    unsigned* end = parent->ShapeChildren.End;
    unsigned* found = parent->ShapeChildren.Begin;
    while (found != end && *found != index) ++found;
    if (found != end)
    {
        if (found + 1 != end) memmove(found, found + 1, (end - found - 1) * sizeof(unsigned));
        --parent->ShapeChildren.End;
    }
    for (unsigned i = 0; i < parent->ShapeChildren.Size(); ++i)
        if (parent->ShapeChildren.Begin[i] > index) --parent->ShapeChildren.Begin[i];
}
