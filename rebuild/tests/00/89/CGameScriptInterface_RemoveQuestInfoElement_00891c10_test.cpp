#include <cstdio>

namespace NPlayerGui
{
class CDrawQuestInfo
{
public:
    void RemoveElement(long elementId);
};
}

struct CGSIRemoveQuestInfoElement_GuiOwner
{
    unsigned char m_Unused[0x1C8];
    NPlayerGui::CDrawQuestInfo* m_QuestInfo;
};

class CGameScriptInterface
{
public:
    virtual void RemoveQuestInfoElement(long elementId) const;
};

CGSIRemoveQuestInfoElement_GuiOwner*
    CGSIRemoveQuestInfoElement_Gui;
static NPlayerGui::CDrawQuestInfo* g_ObservedQuestInfo;
static long g_ObservedElementId;
static unsigned long g_CallCount;

void NPlayerGui::CDrawQuestInfo::RemoveElement(long elementId)
{
    g_ObservedQuestInfo = this;
    g_ObservedElementId = elementId;
    ++g_CallCount;
}

int main()
{
    NPlayerGui::CDrawQuestInfo firstQuestInfo;
    NPlayerGui::CDrawQuestInfo secondQuestInfo;
    CGSIRemoveQuestInfoElement_GuiOwner guiOwner;
    CGameScriptInterface interfaceObject;
    CGSIRemoveQuestInfoElement_Gui = &guiOwner;

    guiOwner.m_QuestInfo = &firstQuestInfo;
    interfaceObject.RemoveQuestInfoElement(0);
    if (g_CallCount != 1 ||
        g_ObservedQuestInfo != &firstQuestInfo ||
        g_ObservedElementId != 0)
        return 1;

    guiOwner.m_QuestInfo = &secondQuestInfo;
    interfaceObject.RemoveQuestInfoElement(
        static_cast<long>(0x87654321UL));
    if (g_CallCount != 2 ||
        g_ObservedQuestInfo != &secondQuestInfo ||
        g_ObservedElementId != static_cast<long>(0x87654321UL))
        return 2;

    std::printf("FSE2_00891c10_TEST PASS\n");
    return 0;
}
