// CGameScriptInterface::DeactivateQuestLater @ 0x00892F10
// VC7.1, x86, /O2 /Oy.
//
// Retail ignores the script-interface object, loads the global quest manager,
// and tail-dispatches the existing two stack arguments unchanged.

struct CCharString;

struct CQuestManager
{
    void DeactivateQuestLater(
        const CCharString& questName,
        unsigned long delay);
};

extern CQuestManager* FableGlobalQuestManager;

void __fastcall CGameScriptInterface_DeactivateQuestLater(
    const void*,
    void*,
    const CCharString& questName,
    unsigned long delay)
{
    FableGlobalQuestManager->DeactivateQuestLater(questName, delay);
}
