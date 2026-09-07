#include <cstddef>
#include <cstdint>

struct VectorCopyElementOverlay {
    std::byte prefix[0x3C];
    std::uint32_t dword3C;
    std::uint32_t dword40;
    std::uint32_t dword44;
    std::uint32_t dword48;
    std::uint32_t dword4C;
};

static_assert(sizeof(VectorCopyElementOverlay) == 0x50);
static_assert(offsetof(VectorCopyElementOverlay, dword3C) == 0x3C);
static_assert(offsetof(VectorCopyElementOverlay, dword40) == 0x40);
static_assert(offsetof(VectorCopyElementOverlay, dword44) == 0x44);
static_assert(offsetof(VectorCopyElementOverlay, dword48) == 0x48);
static_assert(offsetof(VectorCopyElementOverlay, dword4C) == 0x4C);

void* __fastcall Vector_CopyElements(const void* first, const void* last, void* dest)
{
    auto count = static_cast<int>(
        (static_cast<const std::byte*>(last) - static_cast<const std::byte*>(first)) / 0x50
    );
    auto* out = static_cast<std::byte*>(dest);

    if (0 < count) {
        const auto* in = reinterpret_cast<const VectorCopyElementOverlay*>(first);
        using AssignPrefixFn = void(__thiscall*)(void*, const void*);
        const auto assignPrefix = reinterpret_cast<AssignPrefixFn>(0x00431ED0);

        do {
            assignPrefix(out, in);

            auto* outElement = reinterpret_cast<VectorCopyElementOverlay*>(out);
            out = out + 0x50;

            outElement->dword3C = in->dword3C;
            outElement->dword40 = in->dword40;
            outElement->dword44 = in->dword44;
            outElement->dword48 = in->dword48;
            outElement->dword4C = in->dword4C;

            ++in;
            --count;
        } while (count != 0);
    }

    return out;
}