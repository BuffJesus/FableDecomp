#include "fable_ui_counted_children.h"

void __fastcall FableUiReallocateCountedChildren(FableUiComponentChildrenStorage* entries, void*,
    FableUiComponentCountedStorage* position, const FableUiComponentCountedStorage* value,
    void*, unsigned count, bool omitSuffix)
{
    const unsigned size = entries->Size();
    const unsigned capacity = size + (size > count ? size : count);
    FableUiComponentCountedStorage* buffer = capacity ? static_cast<FableUiComponentCountedStorage*>(
        FableUiAllocateChildStorage(capacity * sizeof(FableUiComponentCountedStorage))) : 0;
    FableUiComponentCountedStorage* output = buffer;
    for (FableUiComponentCountedStorage* input = entries->Begin; input != position; ++input, ++output)
        FableUiConstructCountedChild(output, input);
    for (unsigned i = 0; i < count; ++i, ++output) FableUiConstructCountedChild(output, value);
    if (!omitSuffix)
    {
        for (FableUiComponentCountedStorage* input = position; input != entries->End; ++input, ++output)
            FableUiConstructCountedChild(output, input);
    }
    FableUiComponentCountedStorage* oldEnd = entries->End;
    for (FableUiComponentCountedStorage* old = entries->Begin; old != oldEnd; ++old)
    {
        FableUiReleaseChildReference(old);
        old->Data = 0;
        old->Info = 0;
    }
    if (entries->Begin) FableUiFreeChildStorage(entries->Begin);
    entries->Begin = buffer;
    entries->End = output;
    entries->CapacityEnd = buffer ? buffer + capacity : 0;
}
