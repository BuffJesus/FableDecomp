#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct CGuiControlTreePane;

struct Catch0047E8CCOverlay
{
    std::byte pad_00[0x1C];
    void* entriesBegin; // 0x1C
    void* entriesEnd;   // 0x20
    std::byte pad_24[0x04];
    std::byte treePane; // 0x28
};

static_assert(offsetof(Catch0047E8CCOverlay, entriesBegin) == 0x1C);
static_assert(offsetof(Catch0047E8CCOverlay, entriesEnd) == 0x20);
static_assert(offsetof(Catch0047E8CCOverlay, treePane) == 0x28);

// The call site only proves ECX is set to (param_1 + 0x28) before the call.
extern void __thiscall SortTreeRecursively(CGuiControlTreePane* self);

void* __thiscall Catch_0047e8cc(void* param_1, std::uint8_t param_2)
{
    auto* const self = static_cast<Catch0047E8CCOverlay*>(param_1);

    SortTreeRecursively(
        reinterpret_cast<CGuiControlTreePane*>(reinterpret_cast<std::byte*>(param_1) + 0x28));

    auto* const end = static_cast<std::uint32_t*>(self->entriesEnd);
    for (auto* current = static_cast<std::uint32_t*>(self->entriesBegin); current != end; current += 2)
    {
        const auto fn =
            reinterpret_cast<void(__thiscall*)(void*, int)>(*reinterpret_cast<void**>(*current));
        fn(current, 0);
    }

    if (self->entriesBegin != nullptr)
    {
        std::free(self->entriesBegin);
    }

    if ((param_2 & 1u) != 0)
    {
        ::operator delete(param_1);
    }

    return param_1;
}