#include <cstdio>

unsigned char g_questScreenFlag;

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

int main()
{
    CQuestManager m;
    m.screenPtr = (void*)0x1234;

    g_questScreenFlag = 0;
    if (CQuestManager_IsBetweenQuestStartAndFinishScreens(&m) != false) { std::printf("FAIL flag0\n"); return 1; }

    g_questScreenFlag = 1;
    if (CQuestManager_IsBetweenQuestStartAndFinishScreens(&m) != true) { std::printf("FAIL flag1 ptr\n"); return 1; }

    m.screenPtr = 0;
    if (CQuestManager_IsBetweenQuestStartAndFinishScreens(&m) != false) { std::printf("FAIL flag1 null\n"); return 1; }

    std::printf("CQuestManager_004af290_TEST PASS\n");
    return 0;
}