#include <cstddef>
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

void* CDialogueLayerDef_AllocateAndConstruct(
    int count,
    NSpeechGainManager::CDialogueLayerDef* first,
    NSpeechGainManager::CDialogueLayerDef* last)
{
    void* result;
    int delta;

    if (count == 0) {
        result = nullptr;
    } else {
        result = std::malloc(count << 6);
    }

    if (first != last) {
        delta =
            static_cast<int>(reinterpret_cast<std::uintptr_t>(result)) -
            static_cast<int>(reinterpret_cast<std::uintptr_t>(first));

        do {
            auto* const destination =
                reinterpret_cast<NSpeechGainManager::CDialogueLayerDef*>(
                    reinterpret_cast<std::byte*>(first) + delta);

            if (destination != nullptr) {
                ::new (destination) NSpeechGainManager::CDialogueLayerDef(*first);
            }

            first = reinterpret_cast<NSpeechGainManager::CDialogueLayerDef*>(
                reinterpret_cast<std::byte*>(first) + 0x40);
        } while (first != last);
    }

    return result;
}