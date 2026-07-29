#include <cstdio>
#include <cstring>

#include "../../../src/compiled/00/49/CFrontendGameComponent_SetActiveInputSlot_00494440.cpp"

static long* ActiveSlot(CFrontendGameComponent_00494440* component)
{
    return *reinterpret_cast<long**>(
        reinterpret_cast<unsigned char*>(component) + 0x1ec);
}

static bool CheckIndex(long index)
{
    CFrontendGameComponent_00494440 component;
    unsigned char* bytes = reinterpret_cast<unsigned char*>(&component);
    unsigned char before[sizeof(component)];
    std::memset(bytes, 0x5a, sizeof(component));
    std::memcpy(before, bytes, sizeof(before));

    component.SetActiveInputSlot(index);

    if (ActiveSlot(&component) !=
        reinterpret_cast<long*>(bytes + 0x28) + index)
        return false;

    return std::memcmp(bytes, before, 0x1ec) == 0;
}

int main()
{
    if (!CheckIndex(-1) || !CheckIndex(0) ||
        !CheckIndex(7) || !CheckIndex(112))
    {
        std::printf(
            "CFrontendGameComponent_SetActiveInputSlot_00494440_test FAIL\n");
        return 1;
    }

    std::printf(
        "CFrontendGameComponent_SetActiveInputSlot_00494440_test PASS\n");
    return 0;
}
