#include <cstdio>

namespace NPlayerGui
{
class CDrawQuestInfo
{
public:
    void UpdateBar(
        long questId,
        float value,
        float minimum,
        float maximum);
};
}

struct CGSIUpdateQuestInfoBar_GuiOwner
{
    unsigned char m_Unused[0x1C8];
    NPlayerGui::CDrawQuestInfo* m_QuestInfo;
};

class CGameScriptInterface
{
public:
    virtual void UpdateQuestInfoBar(
        long questId,
        float value,
        float minimum,
        float maximum) const;
};

union FloatBits
{
    float value;
    unsigned long bits;
};

CGSIUpdateQuestInfoBar_GuiOwner*
    CGSIUpdateQuestInfoBar_Gui;
static NPlayerGui::CDrawQuestInfo* g_ObservedQuestInfo;
static long g_ObservedQuestId;
static unsigned long g_ObservedValueBits;
static unsigned long g_ObservedMinimumBits;
static unsigned long g_ObservedMaximumBits;
static unsigned long g_CallCount;

void NPlayerGui::CDrawQuestInfo::UpdateBar(
    long questId,
    float value,
    float minimum,
    float maximum)
{
    FloatBits capturedValue;
    FloatBits capturedMinimum;
    FloatBits capturedMaximum;
    capturedValue.value = value;
    capturedMinimum.value = minimum;
    capturedMaximum.value = maximum;
    g_ObservedQuestInfo = this;
    g_ObservedQuestId = questId;
    g_ObservedValueBits = capturedValue.bits;
    g_ObservedMinimumBits = capturedMinimum.bits;
    g_ObservedMaximumBits = capturedMaximum.bits;
    ++g_CallCount;
}

static int CheckCall(
    CGameScriptInterface& interfaceObject,
    CGSIUpdateQuestInfoBar_GuiOwner& guiOwner,
    NPlayerGui::CDrawQuestInfo& questInfo,
    long questId,
    unsigned long valueBits,
    unsigned long minimumBits,
    unsigned long maximumBits)
{
    FloatBits value;
    FloatBits minimum;
    FloatBits maximum;
    value.bits = valueBits;
    minimum.bits = minimumBits;
    maximum.bits = maximumBits;
    guiOwner.m_QuestInfo = &questInfo;

    interfaceObject.UpdateQuestInfoBar(
        questId,
        value.value,
        minimum.value,
        maximum.value);
    if (g_ObservedQuestInfo != &questInfo ||
        g_ObservedQuestId != questId ||
        g_ObservedValueBits != valueBits ||
        g_ObservedMinimumBits != minimumBits ||
        g_ObservedMaximumBits != maximumBits)
        return 1;
    return 0;
}

int main()
{
    NPlayerGui::CDrawQuestInfo firstQuestInfo;
    NPlayerGui::CDrawQuestInfo secondQuestInfo;
    CGSIUpdateQuestInfoBar_GuiOwner guiOwner;
    CGameScriptInterface interfaceObject;
    CGSIUpdateQuestInfoBar_Gui = &guiOwner;

    if (CheckCall(
            interfaceObject,
            guiOwner,
            firstQuestInfo,
            0,
            0x00000000UL,
            0x80000000UL,
            0x3F800000UL) != 0)
        return 1;
    if (g_CallCount != 1)
        return 2;

    if (CheckCall(
            interfaceObject,
            guiOwner,
            secondQuestInfo,
            static_cast<long>(0x87654321UL),
            0xFFC12345UL,
            0x7F800000UL,
            0xFF800000UL) != 0)
        return 3;
    if (g_CallCount != 2)
        return 4;

    std::printf("FSE2_00891b80_TEST PASS\n");
    return 0;
}
