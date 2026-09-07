#include <cstdint>
#include <cstdlib>
#include <new>

namespace NSpeechGainManager
{
    struct CDialogueLayerDef
    {
        std::byte _pad[0x40];

        CDialogueLayerDef(const CDialogueLayerDef&);
    };

    static_assert(sizeof(CDialogueLayerDef) == 0x40);
}

void* __cdecl CDialogueLayerDef_AllocateAndConstruct(
    int count,
    NSpeechGainManager::CDialogueLayerDef* first,
    NSpeechGainManager::CDialogueLayerDef* last)
{
    void* block;
    int delta;

    if (count == 0) {
        block = nullptr;
    } else {
        const int allocationSize = count << 6;
        block = std::malloc(allocationSize);
    }

    if (first != last) {
        delta =
            static_cast<int>(reinterpret_cast<std::uintptr_t>(block)) -
            static_cast<int>(reinterpret_cast<std::uintptr_t>(first));

        do {
            auto* const destination =
                reinterpret_cast<NSpeechGainManager::CDialogueLayerDef*>(
                    static_cast<int>(reinterpret_cast<std::uintptr_t>(first)) + delta);

            if (destination != nullptr) {
                new (destination) NSpeechGainManager::CDialogueLayerDef(*first);
            }

            first = reinterpret_cast<NSpeechGainManager::CDialogueLayerDef*>(
                static_cast<int>(reinterpret_cast<std::uintptr_t>(first)) + 0x40);
        } while (first != last);
    }

    return block;
}