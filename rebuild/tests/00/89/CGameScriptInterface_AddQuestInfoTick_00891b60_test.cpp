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
    long AddTick(
        const CCharString& labelText,
        bool completed,
        float fadeTime);
};
}

struct CGSIAddQuestInfoTickText_GuiOwner
{
    unsigned char m_Unused[0x1C8];
    NPlayerGui::CDrawQuestInfo* m_QuestInfo;
};

class CGameScriptInterface
{
public:
    virtual long AddQuestInfoTick(
        const CCharString& labelText,
        bool completed,
        float fadeTime) const;
};

union FloatBits
{
    float value;
    unsigned long bits;
};

CGSIAddQuestInfoTickText_GuiOwner*
    CGSIAddQuestInfoTickText_Gui;
static NPlayerGui::CDrawQuestInfo* g_ObservedQuestInfo;
static const CCharString* g_ObservedLabelText;
static bool g_ObservedCompleted;
static unsigned long g_ObservedFadeTimeBits;
static long g_Result;
static unsigned long g_CallCount;

long NPlayerGui::CDrawQuestInfo::AddTick(
    const CCharString& labelText,
    bool completed,
    float fadeTime)
{
    FloatBits captured;
    captured.value = fadeTime;
    g_ObservedQuestInfo = this;
    g_ObservedLabelText = &labelText;
    g_ObservedCompleted = completed;
    g_ObservedFadeTimeBits = captured.bits;
    ++g_CallCount;
    return g_Result;
}

static int CheckCall(
    CGameScriptInterface& interfaceObject,
    CGSIAddQuestInfoTickText_GuiOwner& guiOwner,
    NPlayerGui::CDrawQuestInfo& questInfo,
    const CCharString& labelText,
    bool completed,
    unsigned long fadeTimeBits,
    long result)
{
    FloatBits input;
    input.bits = fadeTimeBits;
    guiOwner.m_QuestInfo = &questInfo;
    g_Result = result;

    const long observedResult =
        interfaceObject.AddQuestInfoTick(
            labelText,
            completed,
            input.value);
    if (observedResult != result ||
        g_ObservedQuestInfo != &questInfo ||
        g_ObservedLabelText != &labelText ||
        g_ObservedCompleted != completed ||
        g_ObservedFadeTimeBits != fadeTimeBits)
        return 1;
    return 0;
}

int main()
{
    NPlayerGui::CDrawQuestInfo firstQuestInfo;
    NPlayerGui::CDrawQuestInfo secondQuestInfo;
    CGSIAddQuestInfoTickText_GuiOwner guiOwner;
    CGameScriptInterface interfaceObject;
    CCharString firstLabel(0x11223344);
    CCharString secondLabel(0x55667788);
    CGSIAddQuestInfoTickText_Gui = &guiOwner;

    if (CheckCall(
            interfaceObject,
            guiOwner,
            firstQuestInfo,
            firstLabel,
            false,
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
            true,
            0xFFC12345UL,
            static_cast<long>(0xFEDCBA98UL)) != 0)
        return 3;
    if (g_CallCount != 2)
        return 4;

    std::printf("FSE2_00891b60_TEST PASS\n");
    return 0;
}
