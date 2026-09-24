#include "fable_ui_counted_children.h"

FableUiComponentCountedStorage* __fastcall FableUiEraseCountedChild(
    FableUiComponentChildrenStorage* entries, void*, FableUiComponentCountedStorage* position)
{
    if (position + 1 != entries->End)
        FableUiMoveCountedChildren(position + 1, entries->End, position, 0, 0);
    FableUiComponentCountedStorage* last = --entries->End;
    FableUiReleaseChildReference(last);
    last->Data = 0;
    last->Info = 0;
    return position;
}
