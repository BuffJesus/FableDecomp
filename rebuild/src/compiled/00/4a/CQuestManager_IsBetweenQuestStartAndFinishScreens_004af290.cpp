extern unsigned char g_questScreenFlag;

struct CQuestManager {
    char pad[0x98];
    void* screenPtr;
};

bool __fastcall CQuestManager_IsBetweenQuestStartAndFinishScreens(const CQuestManager* self)
{
    if (g_questScreenFlag)
        return self->screenPtr != 0;
    return false;
}