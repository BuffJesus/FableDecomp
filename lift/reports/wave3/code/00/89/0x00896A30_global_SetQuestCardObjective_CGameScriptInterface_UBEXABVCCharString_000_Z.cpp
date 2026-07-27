#include <cstddef>
#include <cstdint>

namespace
{
    using InterfaceMap =
        CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

    struct InterfaceMapBounds
    {
        std::int32_t* begin_0;
        std::int32_t* end_4;
    };
    static_assert(offsetof(InterfaceMapBounds, end_4) == 0x04);

    struct QuestCardOverlay
    {
        std::byte pad_00[0x2C];
        std::uint32_t flags_2C;
        std::byte pad_30[0x14];
        InterfaceMap interfaces_44;
        std::byte pad_after_44[0x91 - 0x44 - sizeof(InterfaceMap)];
        std::uint8_t flags_91;
    };
    static_assert(offsetof(QuestCardOverlay, flags_2C) == 0x2C);
    static_assert(offsetof(QuestCardOverlay, interfaces_44) == 0x44);
    static_assert(offsetof(QuestCardOverlay, flags_91) == 0x91);

    struct ThingOverlay
    {
        std::byte pad_00[0x20];
        std::uint32_t flags_20;
        std::byte pad_24[0x20];
        InterfaceMap interfaces_44;
    };
    static_assert(offsetof(ThingOverlay, flags_20) == 0x20);
    static_assert(offsetof(ThingOverlay, interfaces_44) == 0x44);

    struct GameScriptInterfaceOverlay
    {
        std::byte pad_00[0x14];
        void* field_14;
    };
    static_assert(offsetof(GameScriptInterfaceOverlay, field_14) == 0x14);
}

void CGameScriptInterface::SetQuestCardObjective(
    const CCharString& arg1,
    const CCharString& arg2,
    const CCharString& arg3,
    const CCharString& arg4) const
{
    if (!CQuestManager::SetQuestCardObjective(DAT_013B89FC, arg1, arg2, arg3, arg4))
    {
        return;
    }

    if (!CQuest_Array_FindByName(arg1))
    {
        return;
    }

    CThing* const activeQuestThing =
        CQuestManager::GetActiveQuestCardFromScriptName(DAT_013B89FC, arg1);
    if (activeQuestThing == nullptr)
    {
        return;
    }

    auto* const questCard = reinterpret_cast<QuestCardOverlay*>(activeQuestThing);
    if ((questCard->flags_91 & 0x01) != 0)
    {
        return;
    }

    if ((questCard->flags_2C & 0x1000) == 0)
    {
        return;
    }

    ETCInterfaceType tcType = static_cast<ETCInterfaceType>(0x6C);
    auto* tcIt = reinterpret_cast<std::int32_t*>(
        InterfaceMap::LowerBound(&questCard->interfaces_44, &tcType));
    auto* const tcEnd =
        reinterpret_cast<InterfaceMapBounds*>(&questCard->interfaces_44)->end_4;

    if (tcIt == tcEnd || 0x6C < *tcIt)
    {
        tcIt = tcEnd;
    }

    if (NThingShot::CTCShotDefinitionBase::CanBeHitByMeleeStrike(
            reinterpret_cast<CTCShotDefinitionBase*>(tcIt[1])))
    {
        return;
    }

    auto* const iface = reinterpret_cast<const GameScriptInterfaceOverlay*>(this)->field_14;
    CThingBuilding* thing = sub_00487D50(sub_00449A60(iface));

    if (!IS_THING_ALIVE<CThingBuilding*>(&thing))
    {
        return;
    }

    auto* const thingOverlay = reinterpret_cast<ThingOverlay*>(thing);
    if ((thingOverlay->flags_20 & 0x10000) == 0)
    {
        return;
    }

    ETCInterfaceType soundType = static_cast<ETCInterfaceType>(0x10);
    auto* soundIt = reinterpret_cast<std::int32_t*>(
        InterfaceMap::LowerBound(&thingOverlay->interfaces_44, &soundType));
    auto* const soundEnd =
        reinterpret_cast<InterfaceMapBounds*>(&thingOverlay->interfaces_44)->end_4;

    if (soundIt == soundEnd || 0x10 < *soundIt)
    {
        soundIt = soundEnd;
    }

    CCharString soundName("LOG_ENTRY", -1);
    CSoundInfo__PlaySound(
        reinterpret_cast<void*>(soundIt[1]),
        soundName,
        0,
        1,
        0,
        0xFFFFFFFF);
}