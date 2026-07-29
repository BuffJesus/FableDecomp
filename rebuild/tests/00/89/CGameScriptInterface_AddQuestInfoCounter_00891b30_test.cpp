#include <cstdio>

class CCounter
{
public:
    explicit CCounter(unsigned long identity)
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
        const CCounter& counter,
        const CCharString& labelText,
        float fadeTime);
};
}

struct CGSIAddQuestInfoCounter_GuiOwner
{
    unsigned char m_Unused[0x1C8];
    NPlayerGui::CDrawQuestInfo* m_QuestInfo;
};

class CGameScriptInterface
{
public:
    virtual long AddQuestInfoCounter(
        const CCounter& counter,
        const CCharString& labelText,
        float fadeTime) const;
};

union FloatBits
{
    float value;
    unsigned long bits;
};

CGSIAddQuestInfoCounter_GuiOwner*
    CGSIAddQuestInfoCounter_Gui;
static NPlayerGui::CDrawQuestInfo* g_ObservedQuestInfo;
static const CCounter* g_ObservedCounter;
static const CCharString* g_ObservedLabelText;
static unsigned long g_ObservedFadeTimeBits;
static long g_Result;
static unsigned long g_CallCount;

long NPlayerGui::CDrawQuestInfo::AddCounter(
    const CCounter& counter,
    const CCharString& labelText,
    float fadeTime)
{
    FloatBits captured;
    captured.value = fadeTime;
    g_ObservedQuestInfo = this;
    g_ObservedCounter = &counter;
    g_ObservedLabelText = &labelText;
    g_ObservedFadeTimeBits = captured.bits;
    ++g_CallCount;
    return g_Result;
}

static int CheckCall(
    CGameScriptInterface& interfaceObject,
    CGSIAddQuestInfoCounter_GuiOwner& guiOwner,
    NPlayerGui::CDrawQuestInfo& questInfo,
    const CCounter& counter,
    const CCharString& labelText,
    unsigned long fadeTimeBits,
    long result)
{
    FloatBits input;
    input.bits = fadeTimeBits;
    guiOwner.m_QuestInfo = &questInfo;
    g_Result = result;

    const long observedResult =
        interfaceObject.AddQuestInfoCounter(
            counter,
            labelText,
            input.value);
    if (observedResult != result ||
        g_ObservedQuestInfo != &questInfo ||
        g_ObservedCounter != &counter ||
        g_ObservedLabelText != &labelText ||
        g_ObservedFadeTimeBits != fadeTimeBits)
        return 1;
    return 0;
}

int main()
{
    NPlayerGui::CDrawQuestInfo firstQuestInfo;
    NPlayerGui::CDrawQuestInfo secondQuestInfo;
    CGSIAddQuestInfoCounter_GuiOwner guiOwner;
    CGameScriptInterface interfaceObject;
    CCounter firstCounter(0x10203040);
    CCounter secondCounter(0x50607080);
    CCharString firstLabel(0x11223344);
    CCharString secondLabel(0x55667788);
    CGSIAddQuestInfoCounter_Gui = &guiOwner;

    if (CheckCall(
            interfaceObject,
            guiOwner,
            firstQuestInfo,
            firstCounter,
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
            secondCounter,
            secondLabel,
            0xFFC12345UL,
            static_cast<long>(0xFEDCBA98UL)) != 0)
        return 3;
    if (g_CallCount != 2)
        return 4;

    std::printf("FSE2_00891b30_TEST PASS\n");
    return 0;
}
