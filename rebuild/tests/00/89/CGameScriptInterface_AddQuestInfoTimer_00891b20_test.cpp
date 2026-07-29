#include <cstdio>

class CTimer
{
public:
    explicit CTimer(unsigned long identity)
        : m_Identity(identity)
    {
    }

    unsigned long m_Identity;
};

class CCharString
{
public:
    explicit CCharString(unsigned long identity)
        : m_Identity(identity)
    {
    }

    unsigned long m_Identity;
};

namespace NPlayerGui
{
class CDrawQuestInfo
{
public:
    long AddCounter(
        const CTimer& timer,
        const CCharString& labelText,
        float fadeTime);
};
}

struct CGSIAddQuestInfoTimer_GuiOwner
{
    unsigned char m_Unused[0x1C8];
    NPlayerGui::CDrawQuestInfo* m_QuestInfo;
};

class CGameScriptInterface
{
public:
    virtual long AddQuestInfoTimer(
        const CTimer& timer,
        const CCharString& labelText,
        float fadeTime) const;
};

union FloatBits
{
    float value;
    unsigned long bits;
};

CGSIAddQuestInfoTimer_GuiOwner*
    CGSIAddQuestInfoTimer_Gui;
static NPlayerGui::CDrawQuestInfo* g_ObservedQuestInfo;
static const CTimer* g_ObservedTimer;
static const CCharString* g_ObservedLabelText;
static unsigned long g_ObservedFadeTimeBits;
static long g_Result;
static unsigned long g_CallCount;

long NPlayerGui::CDrawQuestInfo::AddCounter(
    const CTimer& timer,
    const CCharString& labelText,
    float fadeTime)
{
    FloatBits captured;
    captured.value = fadeTime;
    g_ObservedQuestInfo = this;
    g_ObservedTimer = &timer;
    g_ObservedLabelText = &labelText;
    g_ObservedFadeTimeBits = captured.bits;
    ++g_CallCount;
    return g_Result;
}

static int CheckCall(
    CGameScriptInterface& interfaceObject,
    CGSIAddQuestInfoTimer_GuiOwner& guiOwner,
    NPlayerGui::CDrawQuestInfo& questInfo,
    const CTimer& timer,
    const CCharString& labelText,
    unsigned long fadeTimeBits,
    long result)
{
    FloatBits input;
    input.bits = fadeTimeBits;
    guiOwner.m_QuestInfo = &questInfo;
    g_Result = result;

    const long observedResult =
        interfaceObject.AddQuestInfoTimer(
            timer,
            labelText,
            input.value);
    if (observedResult != result ||
        g_ObservedQuestInfo != &questInfo ||
        g_ObservedTimer != &timer ||
        g_ObservedLabelText != &labelText ||
        g_ObservedFadeTimeBits != fadeTimeBits)
        return 1;
    return 0;
}

int main()
{
    NPlayerGui::CDrawQuestInfo firstQuestInfo;
    NPlayerGui::CDrawQuestInfo secondQuestInfo;
    CGSIAddQuestInfoTimer_GuiOwner guiOwner;
    CGameScriptInterface interfaceObject;
    CTimer firstTimer(0x10203040);
    CTimer secondTimer(0x50607080);
    CCharString firstLabel(0x11223344);
    CCharString secondLabel(0x55667788);
    CGSIAddQuestInfoTimer_Gui = &guiOwner;

    if (CheckCall(
            interfaceObject,
            guiOwner,
            firstQuestInfo,
            firstTimer,
            firstLabel,
            0x80000000UL,
            0) != 0)
        return 1;
    if (g_CallCount != 1)
        return 2;

    if (CheckCall(
            interfaceObject,
            guiOwner,
            secondQuestInfo,
            secondTimer,
            secondLabel,
            0xFFC12345UL,
            static_cast<long>(0xFEDCBA98UL)) != 0)
        return 3;
    if (g_CallCount != 2)
        return 4;

    std::printf("FSE2_00891b20_TEST PASS\n");
    return 0;
}
