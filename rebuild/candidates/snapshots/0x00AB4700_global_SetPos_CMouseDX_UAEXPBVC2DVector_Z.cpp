#include <cstddef>
#include <cstdint>
#include <windows.h>

struct C2DVector_Overlay {
    std::uint32_t x_bits;
    std::uint32_t y_bits;
};

struct CMouseDX_SetPos_Overlay {
    std::byte pad_0000[0x3414];
    std::uint32_t pos_x_bits;
    std::uint32_t pos_y_bits;
    std::byte pad_341C[0x18];
    std::uint32_t cached_x_bits;
    std::uint32_t cached_y_bits;
    std::uint8_t disable_cursor_warp;
};

struct PrimitiveOwner_Overlay {
    std::byte pad_0000[0x94];
    HWND hwnd;
};

struct CEnginePrimitive;
struct CEngineInternalPrimitiveBase;

// Observed call shape is:
//   CEngineInternalPrimitiveBase::AddChildPrimitive(this, unaff_EDI, unaff_ESI)
// This wrapper is intentionally non-standard and must preserve the incoming
// carried EDI/ESI register state at the callsite rather than inventing values.
extern PrimitiveOwner_Overlay* __thiscall AddChildPrimitive_WithCarriedRegisters(
    CEngineInternalPrimitiveBase* primitive);

extern "C" int __cdecl __ftol2();

static_assert(sizeof(C2DVector_Overlay) == 0x8);
static_assert(offsetof(CMouseDX_SetPos_Overlay, pos_x_bits) == 0x3414);
static_assert(offsetof(CMouseDX_SetPos_Overlay, pos_y_bits) == 0x3418);
static_assert(offsetof(CMouseDX_SetPos_Overlay, cached_x_bits) == 0x3434);
static_assert(offsetof(CMouseDX_SetPos_Overlay, cached_y_bits) == 0x3438);
static_assert(offsetof(CMouseDX_SetPos_Overlay, disable_cursor_warp) == 0x343C);
static_assert(offsetof(PrimitiveOwner_Overlay, hwnd) == 0x94);

void CMouseDX::SetPos(const C2DVector* pos)
{
    auto* self = reinterpret_cast<CMouseDX_SetPos_Overlay*>(this);
    auto* in = reinterpret_cast<const C2DVector_Overlay*>(pos);

    self->pos_x_bits = in->x_bits;
    self->cached_x_bits = in->x_bits;
    self->pos_y_bits = in->y_bits;
    self->cached_y_bits = in->y_bits;

    if (self->disable_cursor_warp == 0) {
        auto* primitive = reinterpret_cast<CEngineInternalPrimitiveBase*>(
            static_cast<std::uintptr_t>(in->y_bits));

        PrimitiveOwner_Overlay* const owner =
            AddChildPrimitive_WithCarriedRegisters(primitive);

        POINT clientOrigin;
        clientOrigin.x = 0;
        clientOrigin.y = 0;

        RECT clientRect;

        ClientToScreen(owner->hwnd, &clientOrigin);
        GetClientRect(owner->hwnd, &clientRect);
        OffsetRect(&clientRect, clientOrigin.x, clientOrigin.y);

        int x;
        int y;

        __asm {
            fild dword ptr [clientRect.left]
            mov eax, self
            fadd dword ptr [eax + 0x3414]
            call __ftol2
            mov x, eax
        }

        __asm {
            fild dword ptr [clientRect.top]
            mov eax, self
            fadd dword ptr [eax + 0x3418]
            call __ftol2
            mov y, eax
        }

        SetCursorPos(x, y);
    }
}