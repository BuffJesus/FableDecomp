#include <cstdio>

namespace NPlayerGui
{
class CDrawQuestInfo
{
public:
    void UpdateTimer(long questId, float timeValue);
};
}

struct CGSIUpdateQuestInfoTimer_GuiOwner
{
    unsigned char m_Unused[0x1C8];
    NPlayerGui::CDrawQuestInfo* m_QuestInfo;
};

class CGameScriptInterface
{
public:
    virtual void UpdateQuestInfoTimer(
        long questId,
        float timeValue) const;
};

union FloatBits
{
    float value;
    unsigned long bits;
};

CGSIUpdateQuestInfoTimer_GuiOwner*
    CGSIUpdateQuestInfoTimer_Gui;
static NPlayerGui::CDrawQuestInfo* g_ObservedQuestInfo;
static long g_ObservedQuestId;
static unsigned long g_ObservedTimeBits;
static unsigned long g_CallCount;

void NPlayerGui::CDrawQuestInfo::UpdateTimer(
    long questId,
    float timeValue)
{
    FloatBits captured;
    captured.value = timeValue;
    g_ObservedQuestInfo = this;
    g_ObservedQuestId = questId;
    g_ObservedTimeBits = captured.bits;
    ++g_CallCount;
}

static int CheckCall(
    CGameScriptInterface& interfaceObject,
    CGSIUpdateQuestInfoTimer_GuiOwner& guiOwner,
    NPlayerGui::CDrawQuestInfo& questInfo,
    long questId,
    unsigned long timeBits)
{
    FloatBits input;
    input.bits = timeBits;
    guiOwner.m_QuestInfo = &questInfo;
    interfaceObject.UpdateQuestInfoTimer(questId, input.value);

    if (g_ObservedQuestInfo != &questInfo ||
        g_ObservedQuestId != questId ||
        g_ObservedTimeBits != timeBits)
        return 1;
    return 0;
}

int main()
{
    NPlayerGui::CDrawQuestInfo firstQuestInfo;
    NPlayerGui::CDrawQuestInfo secondQuestInfo;
    CGSIUpdateQuestInfoTimer_GuiOwner guiOwner;
    CGameScriptInterface interfaceObject;
    CGSIUpdateQuestInfoTimer_Gui = &guiOwner;

    if (CheckCall(
            interfaceObject,
            guiOwner,
            firstQuestInfo,
            0,
            0x80000000UL) != 0)
        return 1;
    if (g_CallCount != 1)
        return 2;

    if (CheckCall(
            interfaceObject,
            guiOwner,
            secondQuestInfo,
            static_cast<long>(0x87654321UL),
            0xFFC12345UL) != 0)
        return 3;
    if (g_CallCount != 2)
        return 4;

    std::printf("FSE2_00891bb0_TEST PASS\n");
    return 0;
}
