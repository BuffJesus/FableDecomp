#include <cstddef>
#include <cstdint>

struct CIPhysicsFacesSegmentBase;
struct CCreatureActionBase;

template <typename T>
class CCountedPointer;

namespace NHeroInformationScreens
{
    class CBase;
}

namespace NTCScriptedControl
{
    class CActionDoCreatureAction;
}

extern void* PTR__scalar_deleting_destructor__01265838[];

namespace NTCScriptedControl
{
#pragma pack(push, 1)
    struct CActionDoCreatureAction_Layout
    {
        void** vftable;          // 0x00
        std::uint32_t field_04;  // 0x04
        std::uint8_t field_08;   // 0x08
        std::uint8_t field_09;   // 0x09
        std::byte pad0A[0x02];   // 0x0A
        std::uint32_t field_0C;  // 0x0C
        std::uint8_t field_10;   // 0x10
        std::byte pad11[0x03];   // 0x11
        std::uint32_t field_14;  // 0x14
        std::uint32_t field_18;  // 0x18
    };
#pragma pack(pop)

    static_assert(offsetof(CActionDoCreatureAction_Layout, vftable) == 0x00);
    static_assert(offsetof(CActionDoCreatureAction_Layout, field_04) == 0x04);
    static_assert(offsetof(CActionDoCreatureAction_Layout, field_08) == 0x08);
    static_assert(offsetof(CActionDoCreatureAction_Layout, field_09) == 0x09);
    static_assert(offsetof(CActionDoCreatureAction_Layout, field_0C) == 0x0C);
    static_assert(offsetof(CActionDoCreatureAction_Layout, field_10) == 0x10);
    static_assert(offsetof(CActionDoCreatureAction_Layout, field_14) == 0x14);
    static_assert(offsetof(CActionDoCreatureAction_Layout, field_18) == 0x18);

    CActionDoCreatureAction::CActionDoCreatureAction(CCreatureActionBase const& action)
    {
        NHeroInformationScreens::CBase::CBase(static_cast<NHeroInformationScreens::CBase*>(this));

        auto& self = *reinterpret_cast<CActionDoCreatureAction_Layout*>(this);
        self.field_04 = 0;
        self.field_08 = 0;
        self.field_09 = 0;
        self.vftable = PTR__scalar_deleting_destructor__01265838;
        self.field_14 = 0;
        self.field_18 = 0;
        self.field_0C = 0;
        self.field_10 = 0;

        using Slot2CFn = CIPhysicsFacesSegmentBase* (__thiscall*)(CCreatureActionBase*);
        auto* const action_ptr = const_cast<CCreatureActionBase*>(&action);
        auto** const action_vftable = *reinterpret_cast<void***>(action_ptr);
        auto* const slot_2c = reinterpret_cast<Slot2CFn>(action_vftable[0x2C / sizeof(void*)]);
        CIPhysicsFacesSegmentBase* const segment = slot_2c(action_ptr);

        auto* const counted_ptr =
            reinterpret_cast<CCountedPointer<CIPhysicsFacesSegmentBase const>*>(
                reinterpret_cast<std::byte*>(this) + 0x14);
        counted_ptr->Reset(segment);
    }
}