#include <cstddef>
#include <cstdint>
#include <utility>

namespace
{
struct CTCCoopSpiritField04Overlay
{
    std::byte Pad00[0x04];
    std::uint32_t Field04;
};

static_assert(offsetof(CTCCoopSpiritField04Overlay, Field04) == 0x04);

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

using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CThingPhysicsOwnerOverlay
{
    std::byte Pad00[0x20];
    std::uint32_t Flags20;      // 0x20
    std::byte Pad24[0x20];
    TCInterfaceMap Interfaces44; // 0x44
};

static_assert(offsetof(CThingPhysicsOwnerOverlay, Flags20) == 0x20);
static_assert(offsetof(CThingPhysicsOwnerOverlay, Interfaces44) == 0x44);

using PlayerGetThingFn = CVertexBufferWin32*(__thiscall*)(CPlayer* pThis);
using VectorMetricFn = float(__thiscall*)(const C3DVector* pThis);

extern std::int32_t _DAT_01375550;
extern float _DAT_0137ea20;
extern float _DAT_0125aadc;
extern float _DAT_01241d68;
extern float _DAT_0125b250;
}

void __thiscall CTCCoopSpirit::UpdateAttractionToMaster()
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

    if (this->Master == nullptr)
    {
        return;
    }

    if (!(static_cast<long>(static_cast<float>(_DAT_01375550) * _DAT_0137ea20) < this->NoMovementTimer))
    {
        return;
    }

    static const auto GetThingFromPlayer =
        reinterpret_cast<PlayerGetThingFn>(0x00487DC0);

    CVertexBufferWin32* const pMasterThing = GetThingFromPlayer(this->Master);
    if (pMasterThing == nullptr)
    {
        return;
    }

    C3DVector* const pMasterPosition = CVertexBufferWin32::DoSizeof(pMasterThing);
    CVertexBufferWin32* const pSpiritThing =
        reinterpret_cast<CVertexBufferWin32*>(
            static_cast<std::uintptr_t>(
                reinterpret_cast<const CTCCoopSpiritField04Overlay*>(this)->Field04));
    C3DVector* const pSpiritPosition = CVertexBufferWin32::DoSizeof(pSpiritThing);

    C3DVector delta{};
    delta.x = pMasterPosition->x - pSpiritPosition->x;
    delta.y = pMasterPosition->y - pSpiritPosition->y;
    delta.z = pMasterPosition->z - pSpiritPosition->z;

    static const auto GetVectorMetric =
        reinterpret_cast<VectorMetricFn>(0x00A1436E);

    const float distanceToMaster = GetVectorMetric(&delta);
    if (!(_DAT_0125aadc < distanceToMaster))
    {
        return;
    }

    CTCPhysicsBase* pPhysics;
    auto* const pOwnerThing =
        reinterpret_cast<CThingPhysicsOwnerOverlay*>(
            static_cast<std::uintptr_t>(
                reinterpret_cast<const CTCCoopSpiritField04Overlay*>(this)->Field04));

    if ((pOwnerThing->Flags20 & 0x04u) != 0)
    {
        const ETCInterfaceType interfaceType = static_cast<ETCInterfaceType>(2);
        CKeyPair<ETCInterfaceType, CTCBase*>* pEntry =
            pOwnerThing->Interfaces44.LowerBound(&interfaceType);

        if ((pEntry == pOwnerThing->Interfaces44.m_pEnd) ||
            (2 < static_cast<std::int32_t>(pEntry->m_Key)))
        {
            pEntry = pOwnerThing->Interfaces44.m_pEnd;
        }

        pPhysics = reinterpret_cast<CTCPhysicsBase*>(pEntry->m_Value);
    }

    float attraction = distanceToMaster - _DAT_0125aadc;
    if (_DAT_01241d68 < distanceToMaster - _DAT_0125aadc)
    {
        attraction = _DAT_01241d68;
    }

    attraction = attraction * _DAT_0125b250;

    C3DVector acceleration{};
    acceleration.x = delta.x * attraction;
    acceleration.y = delta.y * attraction;
    acceleration.z = delta.z * attraction;

    pPhysics->ApplyAcceleration(&acceleration);
}