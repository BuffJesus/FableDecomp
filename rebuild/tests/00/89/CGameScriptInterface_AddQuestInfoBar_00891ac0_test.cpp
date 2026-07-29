#include <cstdio>

class CRGBColour
{
public:
    explicit CRGBColour(unsigned long identity)
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
    long AddBar(
        float value,
        float maxValue,
        const CRGBColour& filledColour,
        const CRGBColour& emptyColour,
        const CCharString& labelText,
        const CCharString& valueText,
        float fadeTime);
};
}

struct CGSIAddQuestInfoBar_GuiOwner
{
    unsigned char m_Unused[0x1C8];
    NPlayerGui::CDrawQuestInfo* m_QuestInfo;
};

class CGameScriptInterface
{
public:
    virtual long AddQuestInfoBar(
        float value,
        float maxValue,
        const CRGBColour& filledColour,
        const CRGBColour& emptyColour,
        const CCharString& labelText,
        const CCharString& valueText,
        float fadeTime) const;
};

union FloatBits
{
    float value;
    unsigned long bits;
};

CGSIAddQuestInfoBar_GuiOwner*
    CGSIAddQuestInfoBar_Gui;
static NPlayerGui::CDrawQuestInfo* g_ObservedQuestInfo;
static unsigned long g_ObservedValueBits;
static unsigned long g_ObservedMaxValueBits;
static const CRGBColour* g_ObservedFilledColour;
static const CRGBColour* g_ObservedEmptyColour;
static const CCharString* g_ObservedLabelText;
static const CCharString* g_ObservedValueText;
static unsigned long g_ObservedFadeTimeBits;
static long g_Result;
static unsigned long g_CallCount;

long NPlayerGui::CDrawQuestInfo::AddBar(
    float value,
    float maxValue,
    const CRGBColour& filledColour,
    const CRGBColour& emptyColour,
    const CCharString& labelText,
    const CCharString& valueText,
    float fadeTime)
{
    FloatBits capturedValue;
    FloatBits capturedMaxValue;
    FloatBits capturedFadeTime;
    capturedValue.value = value;
    capturedMaxValue.value = maxValue;
    capturedFadeTime.value = fadeTime;
    g_ObservedQuestInfo = this;
    g_ObservedValueBits = capturedValue.bits;
    g_ObservedMaxValueBits = capturedMaxValue.bits;
    g_ObservedFilledColour = &filledColour;
    g_ObservedEmptyColour = &emptyColour;
    g_ObservedLabelText = &labelText;
    g_ObservedValueText = &valueText;
    g_ObservedFadeTimeBits = capturedFadeTime.bits;
    ++g_CallCount;
    return g_Result;
}

static int CheckCall(
    CGameScriptInterface& interfaceObject,
    CGSIAddQuestInfoBar_GuiOwner& guiOwner,
    NPlayerGui::CDrawQuestInfo& questInfo,
    unsigned long valueBits,
    unsigned long maxValueBits,
    const CRGBColour& filledColour,
    const CRGBColour& emptyColour,
    const CCharString& labelText,
    const CCharString& valueText,
    unsigned long fadeTimeBits,
    long result)
{
    FloatBits value;
    FloatBits maxValue;
    FloatBits fadeTime;
    value.bits = valueBits;
    maxValue.bits = maxValueBits;
    fadeTime.bits = fadeTimeBits;
    guiOwner.m_QuestInfo = &questInfo;
    g_Result = result;

    const long observedResult =
        interfaceObject.AddQuestInfoBar(
            value.value,
            maxValue.value,
            filledColour,
            emptyColour,
            labelText,
            valueText,
            fadeTime.value);
    if (observedResult != result ||
        g_ObservedQuestInfo != &questInfo ||
        g_ObservedValueBits != valueBits ||
        g_ObservedMaxValueBits != maxValueBits ||
        g_ObservedFilledColour != &filledColour ||
        g_ObservedEmptyColour != &emptyColour ||
        g_ObservedLabelText != &labelText ||
        g_ObservedValueText != &valueText ||
        g_ObservedFadeTimeBits != fadeTimeBits)
        return 1;
    return 0;
}

int main()
{
    NPlayerGui::CDrawQuestInfo firstQuestInfo;
    NPlayerGui::CDrawQuestInfo secondQuestInfo;
    CGSIAddQuestInfoBar_GuiOwner guiOwner;
    CGameScriptInterface interfaceObject;
    CRGBColour firstFilledColour(0x10111213);
    CRGBColour firstEmptyColour(0x20212223);
    CRGBColour secondFilledColour(0x30313233);
    CRGBColour secondEmptyColour(0x40414243);
    CCharString firstLabelText(0x50515253);
    CCharString firstValueText(0x60616263);
    CCharString secondLabelText(0x70717273);
    CCharString secondValueText(0x80818283);
    CGSIAddQuestInfoBar_Gui = &guiOwner;

    if (CheckCall(
            interfaceObject,
            guiOwner,
            firstQuestInfo,
            0x00000000UL,
            0x80000000UL,
            firstFilledColour,
            firstEmptyColour,
            firstLabelText,
            firstValueText,
            0x3F800000UL,
            0) != 0)
        return 1;
    if (g_CallCount != 1)
        return 2;

    if (CheckCall(
            interfaceObject,
            guiOwner,
            secondQuestInfo,
            0xFFC12345UL,
            0x7F800000UL,
            secondFilledColour,
            secondEmptyColour,
            secondLabelText,
            secondValueText,
            0xFF800000UL,
            static_cast<long>(0xFEDCBA98UL)) != 0)
        return 3;
    if (g_CallCount != 2)
        return 4;

    std::printf("FSE2_00891ac0_TEST PASS\n");
    return 0;
}
