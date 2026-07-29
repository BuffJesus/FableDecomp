#include <cstdio>

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
        const CCharString& labelText,
        long value,
        float fadeTime);
};
}

struct CGSIAddQuestInfoCounterScalar_GuiOwner
{
    unsigned char m_Unused[0x1C8];
    NPlayerGui::CDrawQuestInfo* m_QuestInfo;
};

class CGameScriptInterface
{
public:
    virtual long AddQuestInfoCounter(
        const CCharString& labelText,
        long value,
        float fadeTime) const;
};

union FloatBits
{
    float value;
    unsigned long bits;
};

CGSIAddQuestInfoCounterScalar_GuiOwner*
    CGSIAddQuestInfoCounterScalar_Gui;
static NPlayerGui::CDrawQuestInfo* g_ObservedQuestInfo;
static const CCharString* g_ObservedLabelText;
static long g_ObservedValue;
static unsigned long g_ObservedFadeTimeBits;
static long g_Result;
static unsigned long g_CallCount;

long NPlayerGui::CDrawQuestInfo::AddCounter(
    const CCharString& labelText,
    long value,
    float fadeTime)
{
    FloatBits captured;
    captured.value = fadeTime;
    g_ObservedQuestInfo = this;
    g_ObservedLabelText = &labelText;
    g_ObservedValue = value;
    g_ObservedFadeTimeBits = captured.bits;
    ++g_CallCount;
    return g_Result;
}

static int CheckCall(
    CGameScriptInterface& interfaceObject,
    CGSIAddQuestInfoCounterScalar_GuiOwner& guiOwner,
    NPlayerGui::CDrawQuestInfo& questInfo,
    const CCharString& labelText,
    long value,
    unsigned long fadeTimeBits,
    long result)
{
    FloatBits input;
    input.bits = fadeTimeBits;
    guiOwner.m_QuestInfo = &questInfo;
    g_Result = result;

    const long observedResult =
        interfaceObject.AddQuestInfoCounter(
            labelText,
            value,
            input.value);
    if (observedResult != result ||
        g_ObservedQuestInfo != &questInfo ||
        g_ObservedLabelText != &labelText ||
        g_ObservedValue != value ||
        g_ObservedFadeTimeBits != fadeTimeBits)
        return 1;
    return 0;
}

int main()
{
    NPlayerGui::CDrawQuestInfo firstQuestInfo;
    NPlayerGui::CDrawQuestInfo secondQuestInfo;
    CGSIAddQuestInfoCounterScalar_GuiOwner guiOwner;
    CGameScriptInterface interfaceObject;
    CCharString firstLabel(0x11223344);
    CCharString secondLabel(0x55667788);
    CGSIAddQuestInfoCounterScalar_Gui = &guiOwner;

    if (CheckCall(
            interfaceObject,
            guiOwner,
            firstQuestInfo,
            firstLabel,
            static_cast<long>(0x80000000UL),
            0x80000000UL,
            0) != 0)
        return 1;
    if (g_CallCount != 1)
        return 2;

    if (CheckCall(
            interfaceObject,
            guiOwner,
            secondQuestInfo,
            secondLabel,
            0x7FFFFFFF,
            0xFFC12345UL,
            static_cast<long>(0xFEDCBA98UL)) != 0)
        return 3;
    if (g_CallCount != 2)
        return 4;

    std::printf("FSE2_00891b40_TEST PASS\n");
    return 0;
}
