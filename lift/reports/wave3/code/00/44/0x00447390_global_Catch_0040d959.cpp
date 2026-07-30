#include <cstddef>
#include <cstdint>

void __fastcall Catch_0040d959(CGamePlayerInterface* self)
{
    struct CGamePlayerInterface_Overlay
    {
        std::byte pad_0000[0x7A4];
        std::int32_t field_7A4;
    };

    static_assert(offsetof(CGamePlayerInterface_Overlay, field_7A4) == 0x7A4);

    CGamePlayerInterface::SetControlScheme(
        self,
        reinterpret_cast<CGamePlayerInterface_Overlay*>(self)->field_7A4
    );
}