extern CQuestManager* DAT_013b89fc;

struct alignas(4) CCharString_StackLocalOverlay
{
    std::byte storage[4];
};

static_assert(sizeof(CCharString_StackLocalOverlay) == 4);
static_assert(alignof(CCharString_StackLocalOverlay) == 4);

void CGameScriptInterface::SetBoastAsCompleted(long boastId, const CCharString& boastName) const
{
    CQuestManager* const questManager = DAT_013b89fc;
    CCharString_StackLocalOverlay localCopy;
    CCharString::CCharString(reinterpret_cast<CCharString*>(&localCopy), &boastName);
    CQuestManager::SetBoastAsCompleted(questManager, boastId);
}