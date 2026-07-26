#include <cstddef>
#include <cstdint>

class CCharString;
class CGameScriptInterface;
class CScriptThing;
class CTCBase;
class CTCParamsBase;
class CThing;

class CCharString
{
public:
    CCharString(const char* pString, int length);
    ~CCharString();
};

class CThing
{
public:
    CTCBase* __thiscall AddTC(const CCharString& tcName, bool enable, CTCParamsBase* pParams);
};

class CGameScriptInterface
{
public:
    void __thiscall EntitySetAsMarryable(
        const CScriptThing& entity,
        bool marryable) const;
};

namespace
{
struct CScriptThingVTableOverlay
{
    std::byte m_Pad00[0x2C];
    CThing* (__thiscall* GetThing)(const CScriptThing* pThis); // 0x2C
};

static_assert(offsetof(CScriptThingVTableOverlay, GetThing) == 0x2C);

struct CScriptThingOverlay
{
    const CScriptThingVTableOverlay* m_pVTable; // 0x00
};

static_assert(offsetof(CScriptThingOverlay, m_pVTable) == 0x00);

struct CThingMarryableOverlay
{
    std::byte m_Pad00[0x10];
    std::uint8_t m_Byte10;             // 0x10
    std::byte m_Pad11[0x38 - 0x11];
    std::uint32_t m_Flags38;           // 0x38
    std::byte m_Pad3C[0x91 - 0x3C];
    std::uint8_t m_Flags91;            // 0x91
    std::byte m_Pad92[0x1C9 - 0x92];
    std::uint8_t m_Byte1C9;            // 0x1C9
};

static_assert(offsetof(CThingMarryableOverlay, m_Byte10) == 0x10);
static_assert(offsetof(CThingMarryableOverlay, m_Flags38) == 0x38);
static_assert(offsetof(CThingMarryableOverlay, m_Flags91) == 0x91);
static_assert(offsetof(CThingMarryableOverlay, m_Byte1C9) == 0x1C9);
}

void __thiscall CGameScriptInterface::EntitySetAsMarryable(
    const CScriptThing& entity,
    bool marryable) const
{
    const auto* const entityOverlay = reinterpret_cast<const CScriptThingOverlay*>(&entity);
    auto* const pThing = reinterpret_cast<CThingMarryableOverlay*>(
        entityOverlay->m_pVTable->GetThing(&entity));

    if ((pThing != nullptr) &&
        ((pThing->m_Flags91 & 0x01) == 0) &&
        (pThing->m_Byte10 == 1))
    {
        const std::uint8_t marryableByte = static_cast<std::uint8_t>(marryable);
        pThing->m_Byte1C9 = static_cast<std::uint8_t>(
            (pThing->m_Byte1C9 & 0xE7) |
            ((marryableByte & 0x01) << 3) |
            ((marryableByte & 0x01) << 4));

        if (marryable && ((pThing->m_Flags38 & 0x00800000) == 0))
        {
            CCharString tcName("CTCWife", -1);
            reinterpret_cast<CThing*>(pThing)->AddTC(tcName, false, nullptr);
        }
    }
}