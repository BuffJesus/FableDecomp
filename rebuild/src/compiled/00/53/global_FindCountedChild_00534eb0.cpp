#include "fable_ui_counted_children.h"

FableUiComponentCountedStorage* __fastcall FableUiFindCountedChild(
    FableUiComponentCountedStorage* begin, FableUiComponentCountedStorage* end,
    const FableUiComponentCountedStorage* value, void*)
{
    while (begin != end && begin->Data != value->Data) ++begin;
    return begin;
}
