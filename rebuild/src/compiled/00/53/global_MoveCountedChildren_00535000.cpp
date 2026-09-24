#include "fable_ui_counted_children.h"

FableUiComponentCountedStorage* __fastcall FableUiMoveCountedChildren(
    FableUiComponentCountedStorage* begin, FableUiComponentCountedStorage* end,
    FableUiComponentCountedStorage* destination, void*, unsigned)
{
    while (begin != end)
    {
        FableReferenceCount* incoming = begin->Info;
        FableUiComponentDrawView* object = begin->Data;
        // Retail compares control blocks, including null, before copying Data.
        if (destination->Info != incoming)
        {
            FableUiReleaseChildReference(destination);
            destination->Data = object;
            destination->Info = incoming;
            if (incoming) ++incoming->owners;
        }
        ++begin;
        ++destination;
    }
    return destination;
}
