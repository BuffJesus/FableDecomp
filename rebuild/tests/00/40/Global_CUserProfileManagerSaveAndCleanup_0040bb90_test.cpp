#include <stdio.h>
#include <stdlib.h>

class CUserProfileManager
{
public:
    void Save();
};

struct CUserProfileManagerSaveOverlay
{
    unsigned char pad00[0x8c];
    unsigned long sourceCursor;
    unsigned long savedCursor;
    unsigned char pad94[4];
    unsigned long cursor;
    void* activeBlock;
    unsigned long blockEnd;
    unsigned long blockLinks;
    unsigned long endCursor;
};

static int g_saveCalls;
void CUserProfileManager::Save() { ++g_saveCalls; }
void __fastcall CUserProfileManager_SaveAndCleanup(CUserProfileManager*);

static void clear_overlay(CUserProfileManagerSaveOverlay& value)
{
    unsigned char* bytes = reinterpret_cast<unsigned char*>(&value);
    for (unsigned long i = 0; i < sizeof(value); ++i) bytes[i] = 0;
}

int main()
{
    CUserProfileManagerSaveOverlay value;
    clear_overlay(value);
    value.sourceCursor = 0x1234;
    value.cursor = 8;
    value.endCursor = 20;
    value.blockEnd = 0x1000;
    CUserProfileManager_SaveAndCleanup(reinterpret_cast<CUserProfileManager*>(&value));
    if (g_saveCalls != 1 || value.savedCursor != 0x1234 || value.cursor != 20) return 1;

    clear_overlay(value);
    unsigned long* links = static_cast<unsigned long*>(malloc(2 * sizeof(unsigned long)));
    void* oldBlock = malloc(16);
    void* nextBlock = malloc(0x80);
    if (!links || !oldBlock || !nextBlock) return 2;
    links[1] = reinterpret_cast<unsigned long>(nextBlock);
    value.sourceCursor = 0x5678;
    value.cursor = 0x2000;
    value.blockEnd = 0x2004;
    value.activeBlock = oldBlock;
    value.blockLinks = reinterpret_cast<unsigned long>(links);
    value.endCursor = reinterpret_cast<unsigned long>(nextBlock);
    CUserProfileManager_SaveAndCleanup(reinterpret_cast<CUserProfileManager*>(&value));
    if (g_saveCalls != 2 || value.savedCursor != 0x5678) return 3;
    if (value.cursor != reinterpret_cast<unsigned long>(nextBlock) ||
        value.activeBlock != nextBlock ||
        value.blockEnd != reinterpret_cast<unsigned long>(nextBlock) + 0x80 ||
        value.blockLinks != reinterpret_cast<unsigned long>(links) + 4) return 4;
    free(nextBlock);
    free(links);

    printf("USER_PROFILE_SAVE_CLEANUP_TEST PASS\n");
    return 0;
}
