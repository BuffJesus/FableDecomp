class CCharString;
class CGameScriptInterface;

extern int DAT_013baf9c;
extern CGameScriptInterface* DAT_013b89fc;

void __thiscall CGameScriptInterface_AddQuestCardEntry(
    CGameScriptInterface* self,
    const CCharString& questName,
    const CCharString& questDescription,
    bool addToFront);

void __thiscall CGameScriptInterface::AddQuestCard(
    const CCharString& questName,
    const CCharString& questDescription,
    bool addToFront,
    bool suppressNotification) const
{
    using VFuncA34 = int(__thiscall*)(const CGameScriptInterface*);
    using VFunc448 = void(__thiscall*)(
        const CGameScriptInterface*,
        const CCharString*,
        const CCharString*,
        bool,
        bool);

    const int currentValue =
        reinterpret_cast<VFuncA34>((*reinterpret_cast<void* const* const*>(this))[0xA34 / sizeof(void*)])(this);

    if (DAT_013baf9c != currentValue)
    {
        if (!suppressNotification)
        {
            CCharString hudOrbQuestCore("HUD_ORB_QUEST_CORE", -1);
            CCharString newQuestMessage("TEXT_QST_078_GM_MSG_NEW_QUEST", -1);

            reinterpret_cast<VFunc448>((*reinterpret_cast<void* const* const*>(this))[0x448 / sizeof(void*)])(
                this,
                &newQuestMessage,
                &hudOrbQuestCore,
                true,
                false);
        }

        DAT_013baf9c =
            reinterpret_cast<VFuncA34>((*reinterpret_cast<void* const* const*>(this))[0xA34 / sizeof(void*)])(this);
    }

    CGameScriptInterface_AddQuestCardEntry(DAT_013b89fc, questName, questDescription, addToFront);
}