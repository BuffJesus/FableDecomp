#include <cstdio>

namespace NPlayerGui
{
class CDrawQuestInfo
{
public:
    void UpdateCounterList(
        long questId,
        long value,
        long maximum);
};
}

struct CGSIUpdateQuestInfoCounterList_GuiOwner
{
    unsigned char m_Unused[0x1C8];
    NPlayerGui::CDrawQuestInfo* m_QuestInfo;
};

class CGameScriptInterface
{
public:
    virtual void UpdateQuestInfoCounterList(
        long questId,
        long value,
        long maximum) const;
};

CGSIUpdateQuestInfoCounterList_GuiOwner*
    CGSIUpdateQuestInfoCounterList_Gui;
static NPlayerGui::CDrawQuestInfo* g_ObservedQuestInfo;
static long g_ObservedQuestId;
static long g_ObservedValue;
static long g_ObservedMaximum;
static unsigned long g_CallCount;

void NPlayerGui::CDrawQuestInfo::UpdateCounterList(
    long questId,
    long value,
    long maximum)
{
    g_ObservedQuestInfo = this;
    g_ObservedQuestId = questId;
    g_ObservedValue = value;
    g_ObservedMaximum = maximum;
    ++g_CallCount;
}

static int CheckCall(
    CGameScriptInterface& interfaceObject,
    CGSIUpdateQuestInfoCounterList_GuiOwner& guiOwner,
    NPlayerGui::CDrawQuestInfo& questInfo,
    long questId,
    long value,
    long maximum)
{
    guiOwner.m_QuestInfo = &questInfo;
    interfaceObject.UpdateQuestInfoCounterList(
        questId,
        value,
        maximum);
    if (g_ObservedQuestInfo != &questInfo ||
        g_ObservedQuestId != questId ||
        g_ObservedValue != value ||
        g_ObservedMaximum != maximum)
        return 1;
    return 0;
}

int main()
{
    NPlayerGui::CDrawQuestInfo firstQuestInfo;
    NPlayerGui::CDrawQuestInfo secondQuestInfo;
    CGSIUpdateQuestInfoCounterList_GuiOwner guiOwner;
    CGameScriptInterface interfaceObject;
    CGSIUpdateQuestInfoCounterList_Gui = &guiOwner;

    if (CheckCall(
            interfaceObject,
            guiOwner,
            firstQuestInfo,
            0,
            1,
            2) != 0)
        return 1;
    if (g_CallCount != 1)
        return 2;

    if (CheckCall(
            interfaceObject,
            guiOwner,
            secondQuestInfo,
            static_cast<long>(0x87654321UL),
            static_cast<long>(0xFEDCBA98UL),
            static_cast<long>(0x7FFFFFFFUL)) != 0)
        return 3;
    if (g_CallCount != 2)
        return 4;

    std::printf("FSE2_00891be0_TEST PASS\n");
    return 0;
}
