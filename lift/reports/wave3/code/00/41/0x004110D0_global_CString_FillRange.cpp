#include <cstddef>
#include <cstdint>
#include <cstring>

struct CStringOverlay {
    std::uint8_t _pad0[0x4];
    std::uint8_t* gap_end;    // +0x04
    std::uint8_t* buffer_end; // +0x08
};

static_assert(offsetof(CStringOverlay, gap_end) == 0x04);
static_assert(offsetof(CStringOverlay, buffer_end) == 0x08);

// Observed raw helper call from the insufficient-capacity branch.
// Only the call-site shape is recoverable from the supplied decompile.
void CString_FillRange(
    std::uint8_t* param_2,
    const std::uint8_t* param_4,
    std::uint8_t** param_2_address,
    std::uint32_t param_3,
    int param_5);

void __thiscall CString_FillRange(
    CStringOverlay* self,
    std::uint8_t* param_2,
    std::uint32_t param_3,
    const std::uint8_t* param_4)
{
    std::uint8_t bVar1;
    std::uint8_t* _Dst;
    std::uint32_t uVar2;
    std::uint8_t* pbVar3;
    std::uint32_t uVar4;
    void* _Dst_00;
    std::uint8_t* _Src;

    uVar4 = param_3;
    _Src = param_2;
    if (param_3 != 0) {
        if (static_cast<std::uint32_t>(
                reinterpret_cast<std::uintptr_t>(self->buffer_end) -
                reinterpret_cast<std::uintptr_t>(self->gap_end)) < param_3) {
            CString_FillRange(param_2, param_4, &param_2, param_3, 0);
            return;
        }

        bVar1 = *param_4;
        uVar2 = static_cast<std::uint32_t>(
            reinterpret_cast<std::uintptr_t>(self->gap_end) -
            reinterpret_cast<std::uintptr_t>(param_2));
        _Dst = self->gap_end;
        param_4 = reinterpret_cast<const std::uint8_t*>(
            (reinterpret_cast<std::uintptr_t>(param_4) & ~static_cast<std::uintptr_t>(0xff)) |
            static_cast<std::uintptr_t>(bVar1));

        if (param_3 < uVar2) {
            pbVar3 = _Dst - param_3;
            param_3 = uVar2;
            if (_Dst != pbVar3) {
                std::memmove(
                    _Dst,
                    pbVar3,
                    static_cast<std::size_t>(
                        reinterpret_cast<std::uintptr_t>(_Dst) -
                        reinterpret_cast<std::uintptr_t>(pbVar3)));
            }

            self->gap_end = self->gap_end + uVar4;
            pbVar3 = reinterpret_cast<std::uint8_t*>(
                reinterpret_cast<std::uintptr_t>(_Dst) +
                (-static_cast<std::intptr_t>(uVar4) -
                 static_cast<std::intptr_t>(reinterpret_cast<std::uintptr_t>(_Src))));
            if (0 < static_cast<std::intptr_t>(reinterpret_cast<std::uintptr_t>(pbVar3))) {
                std::memmove(
                    _Dst - static_cast<std::ptrdiff_t>(reinterpret_cast<std::uintptr_t>(pbVar3)),
                    _Src,
                    static_cast<std::size_t>(reinterpret_cast<std::uintptr_t>(pbVar3)));
            }
        } else {
            param_4 = reinterpret_cast<const std::uint8_t*>(static_cast<std::uintptr_t>(bVar1));
            pbVar3 = _Dst;
            for (uVar4 = (param_3 - uVar2) >> 2; uVar4 != 0; --uVar4) {
                *reinterpret_cast<std::uint32_t*>(pbVar3) =
                    static_cast<std::uint32_t>(
                        static_cast<std::uint8_t>(reinterpret_cast<std::uintptr_t>(param_4))) |
                    (static_cast<std::uint32_t>(
                         static_cast<std::uint8_t>(reinterpret_cast<std::uintptr_t>(param_4)))
                     << 8) |
                    (static_cast<std::uint32_t>(
                         static_cast<std::uint8_t>(reinterpret_cast<std::uintptr_t>(param_4)))
                     << 16) |
                    (static_cast<std::uint32_t>(
                         static_cast<std::uint8_t>(reinterpret_cast<std::uintptr_t>(param_4)))
                     << 24);
                pbVar3 = pbVar3 + 4;
            }
            for (uVar4 = (param_3 - uVar2) & 3; uVar4 != 0; --uVar4) {
                *pbVar3 = static_cast<std::uint8_t>(reinterpret_cast<std::uintptr_t>(param_4));
                pbVar3 = pbVar3 + 1;
            }

            _Dst_00 = self->gap_end + (param_3 - uVar2);
            self->gap_end = static_cast<std::uint8_t*>(_Dst_00);
            if (_Dst != param_2) {
                param_3 = uVar2;
                std::memmove(
                    _Dst_00,
                    param_2,
                    static_cast<std::size_t>(
                        reinterpret_cast<std::uintptr_t>(_Dst) -
                        reinterpret_cast<std::uintptr_t>(param_2)));
            }

            self->gap_end = self->gap_end + uVar2;
            uVar4 = static_cast<std::uint32_t>(
                reinterpret_cast<std::uintptr_t>(_Dst) -
                reinterpret_cast<std::uintptr_t>(_Src));
        }

        for (uVar2 = uVar4 >> 2; uVar2 != 0; --uVar2) {
            *reinterpret_cast<std::uint32_t*>(_Src) =
                static_cast<std::uint32_t>(
                    static_cast<std::uint8_t>(reinterpret_cast<std::uintptr_t>(param_4))) |
                (static_cast<std::uint32_t>(
                     static_cast<std::uint8_t>(reinterpret_cast<std::uintptr_t>(param_4)))
                 << 8) |
                (static_cast<std::uint32_t>(
                     static_cast<std::uint8_t>(reinterpret_cast<std::uintptr_t>(param_4)))
                 << 16) |
                (static_cast<std::uint32_t>(
                     static_cast<std::uint8_t>(reinterpret_cast<std::uintptr_t>(param_4)))
                 << 24);
            _Src = _Src + 4;
        }
        for (uVar4 = uVar4 & 3; uVar4 != 0; --uVar4) {
            *_Src = static_cast<std::uint8_t>(reinterpret_cast<std::uintptr_t>(param_4));
            _Src = _Src + 1;
        }
    }
}