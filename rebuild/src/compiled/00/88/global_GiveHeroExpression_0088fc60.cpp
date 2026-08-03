#include <stddef.h>

typedef unsigned char uint8_t;

class CCharString;

class CThingPlayerCreature
{
public:
    bool LearnExpression(const CCharString& expression, long value, bool immediate);
};

struct CThingPlayerCreature_GiveHeroExpression_Overlay
{
    unsigned char m_Pad00[0x91];
    uint8_t m_Flags91; // 0x91
};

// engine callees (addresses reloc-masked by the parity checker)
extern "C" void*                 __fastcall GSI_GiveHeroExpression_Helper_A(void* p);   // 0x00449970
extern "C" CThingPlayerCreature* __fastcall GSI_GiveHeroExpression_Helper_B(void* p);   // 0x00487DC0

class CGameScriptInterface
{
public:
    void GiveHeroExpression(const CCharString& expression, long value, bool immediate) const;

private:
    unsigned char m_Pad00[0x14];
    void* m_Field14; // 0x14
};

void CGameScriptInterface::GiveHeroExpression(
    const CCharString& expression,
    long value,
    bool immediate) const
{
    CThingPlayerCreature* const hero =
        GSI_GiveHeroExpression_Helper_B(
            GSI_GiveHeroExpression_Helper_A(m_Field14));

    if ((hero != 0) &&
        ((reinterpret_cast<const CThingPlayerCreature_GiveHeroExpression_Overlay*>(hero)->m_Flags91 & 0x01) == 0))
    {
        hero->LearnExpression(expression, value, immediate);
    }
}