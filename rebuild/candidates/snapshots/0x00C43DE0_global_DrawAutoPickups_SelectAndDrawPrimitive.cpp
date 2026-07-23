#include <cstddef>
#include <cstdint>

namespace
{
struct DrawContext50_Overlay
{
    std::uint8_t bytes[0x04];
};

struct DrawAutoPickups_Overlay
{
    void** vftable; // +0x00
    std::uint8_t pad04[0x50 - 0x04];
    DrawContext50_Overlay drawContext50;   // +0x50
    std::int32_t* primitiveArray54;        // +0x54
    std::uint8_t pad58[0x5C - 0x58];
    std::int32_t primitiveArrayGuard5C;    // +0x5C
    std::uint8_t pad60[0x64 - 0x60];
    std::int32_t primitiveIndex64;         // +0x64
    std::uint8_t pad68[0x78 - 0x68];
    std::int32_t selectedPrimitive78;      // +0x78
};

using SelectAndDrawPrimitiveIndirectFn =
    void(__thiscall*)(void* self, std::int32_t primitive, DrawContext50_Overlay* drawContext);

static_assert(offsetof(DrawAutoPickups_Overlay, drawContext50) == 0x50);
static_assert(offsetof(DrawAutoPickups_Overlay, primitiveArray54) == 0x54);
static_assert(offsetof(DrawAutoPickups_Overlay, primitiveArrayGuard5C) == 0x5C);
static_assert(offsetof(DrawAutoPickups_Overlay, primitiveIndex64) == 0x64);
static_assert(offsetof(DrawAutoPickups_Overlay, selectedPrimitive78) == 0x78);
}

void __fastcall DrawAutoPickups_SelectAndDrawPrimitive(DrawAutoPickups_Overlay* self)
{
    std::int32_t primitive = self->selectedPrimitive78;
    if (primitive == 0)
    {
        // Intentionally preserves Ghidra's duplicated +0x5C nonzero test.
        if ((self->primitiveArrayGuard5C != 0) &&
            (self->primitiveArrayGuard5C != 0))
        {
            const auto drawPrimitive =
                reinterpret_cast<SelectAndDrawPrimitiveIndirectFn>(
                    self->vftable[0x20 / sizeof(void*)]);

            primitive = self->primitiveArray54[self->primitiveIndex64];
            drawPrimitive(self, primitive, &self->drawContext50);
            return;
        }

        primitive = 0;
    }

    const auto drawPrimitive =
        reinterpret_cast<SelectAndDrawPrimitiveIndirectFn>(
            self->vftable[0x20 / sizeof(void*)]);

    drawPrimitive(self, primitive, &self->drawContext50);
}