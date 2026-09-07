#include <cstddef>
#include <cstdint>

struct VectorEraseOverlay {
    std::byte gap0[4];
    std::uint32_t* end;
};

static_assert(offsetof(VectorEraseOverlay, end) == 0x4);

std::uint32_t* __thiscall Vector_Erase(void* self, std::uint32_t* param_2, int param_3)
{
    auto* const overlay = reinterpret_cast<VectorEraseOverlay*>(self);

    int iVar3 = (reinterpret_cast<int>(overlay->end) - param_3) / 0xC;
    std::uint32_t* puVar4 = param_2;

    if (0 < iVar3) {
        do {
            std::uint32_t* puVar1 =
                reinterpret_cast<std::uint32_t*>((param_3 - reinterpret_cast<int>(param_2)) +
                                                 reinterpret_cast<int>(puVar4));
            *puVar4 = *puVar1;
            std::uint32_t* puVar2 = puVar4 + 2;
            puVar4[1] = puVar1[1];
            puVar4 = puVar4 + 3;
            iVar3 = iVar3 + -1;
            *puVar2 = puVar1[2];
        } while (iVar3 != 0);
    }

    overlay->end = puVar4;
    return param_2;
}