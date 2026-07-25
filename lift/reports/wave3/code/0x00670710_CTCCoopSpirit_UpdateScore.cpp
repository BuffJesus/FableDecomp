#include <cstddef>
#include <cstdint>

class CTCBase;

enum ETCInterfaceType : std::int32_t
{
};

template <typename TKey, typename TValue>
class CKeyPairCompareLess;

template <typename TKey, typename TValue>
struct CKeyPair
{
    TKey m_Key;     // 0x00
    TValue m_Value; // 0x04
};

static_assert(sizeof(CKeyPair<ETCInterfaceType, CTCBase*>) == 0x08);
static_assert(offsetof(CKeyPair<ETCInterfaceType, CTCBase*>, m_Key) == 0x00);
static_assert(offsetof(CKeyPair<ETCInterfaceType, CTCBase*>, m_Value) == 0x04);

template <typename TKey, typename TValue, typename TCompare>
class CVectorMap
{
public:
    CKeyPair<TKey, TValue>* m_pBegin; // 0x00
    CKeyPair<TKey, TValue>* m_pEnd;   // 0x04

    CKeyPair<TKey, TValue>* __thiscall LowerBound(const TKey* pKey);
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct ResolvedPlayerScoreOverlay
{
    std::byte Pad00[0x38];
    std::int8_t Field38;
    std::byte Pad39[0x44 - 0x39];
    TCInterfaceMap InterfaceMap44;
};

static_assert(offsetof(ResolvedPlayerScoreOverlay, Field38) == 0x38);
static_assert(offsetof(ResolvedPlayerScoreOverlay, InterfaceMap44) == 0x44);

struct ScoreInterfaceOverlay
{
    std::byte Pad00[0x10];
    std::int32_t Field10;
};

static_assert(offsetof(ScoreInterfaceOverlay, Field10) == 0x10);

using ResolvePlayerRaw = ResolvedPlayerScoreOverlay*(__thiscall*)(CPlayer*);

inline constexpr ResolvePlayerRaw kResolvePlayerRaw =
    reinterpret_cast<ResolvePlayerRaw>(0x00487DC0);
}

void __thiscall CTCCoopSpirit::UpdateScore()
{
    static_assert(sizeof(CTCCoopSpirit) == 0x34);
    static_assert(offsetof(CTCCoopSpirit, NoMovementTimer) == 0x0C);
    static_assert(offsetof(CTCCoopSpirit, Score) == 0x10);
    static_assert(offsetof(CTCCoopSpirit, OffscreenCounter) == 0x14);
    static_assert(offsetof(CTCCoopSpirit, MovementVector) == 0x18);
    static_assert(offsetof(CTCCoopSpirit, Master) == 0x24);
    static_assert(offsetof(CTCCoopSpirit, PCoopSpiritDef) == 0x28);
    static_assert(offsetof(CTCCoopSpirit, PParticleEmitter) == 0x2C);
    static_assert(sizeof(void*) == 4);

    if (0x13 < this->Score)
    {
        for (long playerSlot = 0; playerSlot < 4; ++playerSlot)
        {
            CDisplayEngine* const displayEngine =
                NDisplayView::CViewBase::GetDisplayEngine(
                    reinterpret_cast<NDisplayView::CViewBase*>(this));
            CIDrawEnvironment* const drawEnvironment =
                CWorld::DrawGetEnvironment(reinterpret_cast<CWorld*>(displayEngine));

            if (reinterpret_cast<CIDrawEnvironment*>(playerSlot) != drawEnvironment)
            {
                CDisplayEngine* const displayEngineForPlayer =
                    NDisplayView::CViewBase::GetDisplayEngine(
                        reinterpret_cast<NDisplayView::CViewBase*>(this));
                CPlayer* const player =
                    CPlayerManager::GetPlayer(
                        reinterpret_cast<CPlayerManager*>(displayEngineForPlayer),
                        playerSlot);
                ResolvedPlayerScoreOverlay* const resolvedPlayer =
                    kResolvePlayerRaw(player);

                if ((resolvedPlayer != nullptr) && (resolvedPlayer->Field38 < 0))
                {
                    ETCInterfaceType interfaceType =
                        static_cast<ETCInterfaceType>(0xC7);
                    CKeyPair<ETCInterfaceType, CTCBase*>* pEntry =
                        resolvedPlayer->InterfaceMap44.LowerBound(&interfaceType);

                    if ((pEntry == resolvedPlayer->InterfaceMap44.m_pEnd) ||
                        (0xC7 < static_cast<std::int32_t>(pEntry->m_Key)))
                    {
                        pEntry = resolvedPlayer->InterfaceMap44.m_pEnd;
                    }

                    reinterpret_cast<ScoreInterfaceOverlay*>(pEntry->m_Value)->Field10 = 0;
                }
            }
        }

        SwapToHero();
    }
}