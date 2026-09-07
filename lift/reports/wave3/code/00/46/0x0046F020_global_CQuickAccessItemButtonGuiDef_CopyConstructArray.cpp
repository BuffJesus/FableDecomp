#include <cstdlib>
#include <new>

void* __cdecl CQuickAccessItemButtonGuiDef_CopyConstructArray(
    int count,
    CQuickAccessItemButtonGuiDef* first,
    CQuickAccessItemButtonGuiDef* last)
{
    void* result;
    int delta;

    if (count == 0) {
        result = nullptr;
    } else {
        result = std::malloc(count * 0x50);
    }

    if (first != last) {
        delta = static_cast<int>(reinterpret_cast<char*>(result) - reinterpret_cast<char*>(first));
        do {
            auto* const dest =
                reinterpret_cast<CQuickAccessItemButtonGuiDef*>(
                    reinterpret_cast<char*>(first) + delta);

            if (dest != nullptr) {
                ::new (dest) CQuickAccessItemButtonGuiDef(*first);
            }

            first = reinterpret_cast<CQuickAccessItemButtonGuiDef*>(
                reinterpret_cast<char*>(first) + 0x50);
        } while (first != last);
    }

    return result;
}