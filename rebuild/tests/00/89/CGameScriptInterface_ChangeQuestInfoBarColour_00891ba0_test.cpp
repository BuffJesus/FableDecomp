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

namespace NPlayerGui
{
class CDrawQuestInfo
{
public:
    void ChangeBarColour(
        long questId,
        const CRGBColour& primaryColour,
        const CRGBColour& secondaryColour);
};
}

struct CGSIChangeQuestInfoBarColour_GuiOwner
{
    unsigned char m_Unused[0x1C8];
    NPlayerGui::CDrawQuestInfo* m_QuestInfo;
};

class CGameScriptInterface
{
public:
    virtual void ChangeQuestInfoBarColour(
        long questId,
        const CRGBColour& primaryColour,
        const CRGBColour& secondaryColour) const;
};

CGSIChangeQuestInfoBarColour_GuiOwner*
    CGSIChangeQuestInfoBarColour_Gui;
static NPlayerGui::CDrawQuestInfo* g_ObservedQuestInfo;
static long g_ObservedQuestId;
static const CRGBColour* g_ObservedPrimaryColour;
static const CRGBColour* g_ObservedSecondaryColour;
static unsigned long g_CallCount;

void NPlayerGui::CDrawQuestInfo::ChangeBarColour(
    long questId,
    const CRGBColour& primaryColour,
    const CRGBColour& secondaryColour)
{
    g_ObservedQuestInfo = this;
    g_ObservedQuestId = questId;
    g_ObservedPrimaryColour = &primaryColour;
    g_ObservedSecondaryColour = &secondaryColour;
    ++g_CallCount;
}

static int CheckCall(
    CGameScriptInterface& interfaceObject,
    CGSIChangeQuestInfoBarColour_GuiOwner& guiOwner,
    NPlayerGui::CDrawQuestInfo& questInfo,
    long questId,
    const CRGBColour& primaryColour,
    const CRGBColour& secondaryColour)
{
    guiOwner.m_QuestInfo = &questInfo;
    interfaceObject.ChangeQuestInfoBarColour(
        questId,
        primaryColour,
        secondaryColour);
    if (g_ObservedQuestInfo != &questInfo ||
        g_ObservedQuestId != questId ||
        g_ObservedPrimaryColour != &primaryColour ||
        g_ObservedSecondaryColour != &secondaryColour)
        return 1;
    return 0;
}

int main()
{
    NPlayerGui::CDrawQuestInfo firstQuestInfo;
    NPlayerGui::CDrawQuestInfo secondQuestInfo;
    CGSIChangeQuestInfoBarColour_GuiOwner guiOwner;
    CGameScriptInterface interfaceObject;
    CRGBColour firstPrimaryColour(0x10111213);
    CRGBColour firstSecondaryColour(0x20212223);
    CRGBColour secondPrimaryColour(0x30313233);
    CRGBColour secondSecondaryColour(0x40414243);
    CGSIChangeQuestInfoBarColour_Gui = &guiOwner;

    if (CheckCall(
            interfaceObject,
            guiOwner,
            firstQuestInfo,
            0,
            firstPrimaryColour,
            firstSecondaryColour) != 0)
        return 1;
    if (g_CallCount != 1)
        return 2;

    if (CheckCall(
            interfaceObject,
            guiOwner,
            secondQuestInfo,
            static_cast<long>(0x87654321UL),
            secondPrimaryColour,
            secondSecondaryColour) != 0)
        return 3;
    if (g_CallCount != 2)
        return 4;

    std::printf("FSE2_00891ba0_TEST PASS\n");
    return 0;
}
