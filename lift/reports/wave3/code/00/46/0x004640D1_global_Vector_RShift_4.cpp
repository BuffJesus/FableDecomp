#include <cstddef>
#include <cstdint>

struct Vector_RShift_4_Overlay {
    std::byte pad_0[4];
    std::uint32_t* end;
};

static_assert(offsetof(Vector_RShift_4_Overlay, end) == 4);

std::uint32_t* __thiscall Vector_RShift_4(int param_1, std::uint32_t* param_2, int param_3)
{
    auto* const self = reinterpret_cast<Vector_RShift_4_Overlay*>(param_1);
    int iVar1 = (static_cast<int>(reinterpret_cast<std::intptr_t>(self->end)) - param_3) >> 2;
    std::uint32_t* puVar2 = param_2;

    if (0 < iVar1) {
        do {
            *puVar2 = *reinterpret_cast<std::uint32_t*>(
                (param_3 - static_cast<int>(reinterpret_cast<std::intptr_t>(param_2))) +
                static_cast<int>(reinterpret_cast<std::intptr_t>(puVar2))
            );
            puVar2 = puVar2 + 1;
            iVar1 = iVar1 + -1;
        } while (iVar1 != 0);
    }

    self->end = puVar2;
    return param_2;
}