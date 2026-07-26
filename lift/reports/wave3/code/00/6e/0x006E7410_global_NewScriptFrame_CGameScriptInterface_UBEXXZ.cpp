#include <cstddef>
#include <cstdint>

class CGameScriptInterface
{
public:
    void __thiscall NewScriptFrame() const;
};

extern "C" long __cdecl GFGetPreMainMemoryUsed();

extern void* DAT_013d2838;
extern void* DAT_013b89fc;

namespace
{
struct DAT_013d2838_VTableOverlay
{
    std::byte m_Pad00[0x08];
    void (__thiscall* m_RawSlot08)(void* pThis); // 0x08
};

static_assert(offsetof(DAT_013d2838_VTableOverlay, m_RawSlot08) == 0x08);

struct DAT_013d2838_Overlay
{
    const DAT_013d2838_VTableOverlay* m_pVTable; // 0x00
    std::byte m_Pad04[0x01];
    std::uint8_t m_Flag05; // 0x05
};

static_assert(offsetof(DAT_013d2838_Overlay, m_pVTable) == 0x00);
static_assert(offsetof(DAT_013d2838_Overlay, m_Flag05) == 0x05);

struct CGameScriptInterface_NewScriptFrame_GlobalsOverlay
{
    std::byte m_Pad00[0x88];
    void* m_Field88; // 0x88
};

static_assert(
    offsetof(CGameScriptInterface_NewScriptFrame_GlobalsOverlay, m_Field88) == 0x88);

struct CGameScriptInterface_NewScriptFrame_Field88Overlay
{
    std::byte m_Pad00[0x08];
    void* m_Field08; // 0x08
};

static_assert(
    offsetof(CGameScriptInterface_NewScriptFrame_Field88Overlay, m_Field08) == 0x08);

struct CGameScriptInterface_NewScriptFrame_Field08Overlay
{
    std::byte m_Pad00[0x2C];
    void* m_Field2C; // 0x2C
};

static_assert(
    offsetof(CGameScriptInterface_NewScriptFrame_Field08Overlay, m_Field2C) == 0x2C);

struct CGameScriptInterface_NewScriptFrame_MemorySampleOverlay
{
    std::byte m_Pad00[0x18];
    std::uint32_t m_PreviousValue; // 0x18
    long m_CurrentValue;           // 0x1C
};

static_assert(
    offsetof(
        CGameScriptInterface_NewScriptFrame_MemorySampleOverlay,
        m_PreviousValue) == 0x18);
static_assert(
    offsetof(
        CGameScriptInterface_NewScriptFrame_MemorySampleOverlay,
        m_CurrentValue) == 0x1C);
}

void __thiscall CGameScriptInterface::NewScriptFrame() const
{
    (void)this;

    auto* const global013d2838BeforeCall =
        reinterpret_cast<DAT_013d2838_Overlay*>(DAT_013d2838);
    if (global013d2838BeforeCall->m_Flag05 == 0)
    {
        global013d2838BeforeCall->m_pVTable->m_RawSlot08(DAT_013d2838);

        auto* const global013d2838AfterCall =
            reinterpret_cast<DAT_013d2838_Overlay*>(DAT_013d2838);
        if (global013d2838AfterCall->m_Flag05 == 0)
        {
            auto* const globals =
                reinterpret_cast<CGameScriptInterface_NewScriptFrame_GlobalsOverlay*>(
                    DAT_013b89fc);
            auto* const field88 =
                reinterpret_cast<CGameScriptInterface_NewScriptFrame_Field88Overlay*>(
                    globals->m_Field88);
            auto* const field08 =
                reinterpret_cast<CGameScriptInterface_NewScriptFrame_Field08Overlay*>(
                    field88->m_Field08);
            auto* const memorySample =
                reinterpret_cast<CGameScriptInterface_NewScriptFrame_MemorySampleOverlay*>(
                    field08->m_Field2C);

            const long preMainMemoryUsed = GFGetPreMainMemoryUsed();
            memorySample->m_PreviousValue = memorySample->m_CurrentValue;
            memorySample->m_CurrentValue = preMainMemoryUsed;
        }
    }
}