#include <cstddef>
#include <cstdint>

extern "C" std::int32_t __cdecl __ftol2();

class CGameScriptInterface;
class CTCBase;

enum ETCInterfaceType : std::int32_t;

template <typename TKey, typename TValue>
class CKeyPairCompareLess;

template <typename TKey, typename TValue, typename TCompare>
class CVectorMap
{
public:
    static void* __thiscall LowerBound(CVectorMap* pThis, const TKey* pKey);
};

class CGameScriptInterface
{
public:
    long __thiscall GetBestScoreCoinGolfOakVale() const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct TCInterfaceMapEntry
{
    ETCInterfaceType m_Key; // 0x00
    CTCBase* m_Value;       // 0x04
};

static_assert(offsetof(TCInterfaceMapEntry, m_Value) == 0x04);

struct TCInterfaceMapOverlay
{
    TCInterfaceMapEntry* m_Begin; // 0x00
    TCInterfaceMapEntry* m_End;   // 0x04
};

static_assert(offsetof(TCInterfaceMapOverlay, m_End) == 0x04);

struct CoinGolfScoreOverlay
{
    std::byte gap_00[0x184];
    float m_Float184; // 0x184
};

static_assert(offsetof(CoinGolfScoreOverlay, m_Float184) == 0x184);

struct ScoreProviderOverlay
{
    std::byte gap_00[0x20];
    std::uint8_t m_Flags20;       // 0x20
    std::byte gap_21[0x23];
    TCInterfaceMapOverlay m_Map;  // 0x44
    std::byte gap_4c[0x45];
    std::uint8_t m_Flags91;       // 0x91
};

static_assert(offsetof(ScoreProviderOverlay, m_Flags20) == 0x20);
static_assert(offsetof(ScoreProviderOverlay, m_Map) == 0x44);
static_assert(offsetof(ScoreProviderOverlay, m_Flags91) == 0x91);

struct GameScriptInterfaceOverlay
{
    std::byte gap_00[0x14];
    void* m_Field14; // 0x14
};

static_assert(offsetof(GameScriptInterfaceOverlay, m_Field14) == 0x14);

using GetManagerFn = void* (__thiscall*)(void*);
using GetScoreProviderFn = ScoreProviderOverlay* (__thiscall*)(void*);

constexpr GetManagerFn kGetManager = reinterpret_cast<GetManagerFn>(0x00459971);
constexpr GetScoreProviderFn kGetScoreProvider = reinterpret_cast<GetScoreProviderFn>(0x00487DD1);
}

long CGameScriptInterface::GetBestScoreCoinGolfOakVale() const
{
    auto* const pThis = reinterpret_cast<const GameScriptInterfaceOverlay*>(this);
    void* const manager = kGetManager(pThis->m_Field14);
    ScoreProviderOverlay* const provider = kGetScoreProvider(manager);

    if (provider != nullptr && (provider->m_Flags91 & 0x01) == 0 && (provider->m_Flags20 & 0x10) != 0)
    {
        const ETCInterfaceType key = static_cast<ETCInterfaceType>(4);
        auto* const pMap = &provider->m_Map;
        auto* entry = static_cast<TCInterfaceMapEntry*>(
            TCInterfaceMap::LowerBound(reinterpret_cast<TCInterfaceMap*>(pMap), &key));
        TCInterfaceMapEntry* const end = pMap->m_End;

        if (entry == end || static_cast<std::int32_t>(entry->m_Key) > 4)
        {
            entry = end;
        }

        const auto* const scoreObject = reinterpret_cast<const CoinGolfScoreOverlay*>(entry->m_Value);
        return __ftol2(scoreObject->m_Float184);
    }

    return 0;
}