// Behaviour: Apply calls ApplyValues on `this`, then ApplyVideoValues on `this`
// only when ApplyVideoValuesOnLoad (+0x33) is set; the rest of the object is untouched.
#include "fable_user_profile.h"
#include <stdio.h>
#include <string.h>

static const void* g_calls[4];
static int g_callKinds[4];
static int g_count;

void CUserProfileManager::ApplyValues() { if (g_count < 4) { g_calls[g_count] = this; g_callKinds[g_count] = 1; } ++g_count; }
void CUserProfileManager::ApplyVideoValues() { if (g_count < 4) { g_calls[g_count] = this; g_callKinds[g_count] = 2; } ++g_count; }

static int Check(unsigned char flag, int expectVideo)
{
    unsigned char storage[sizeof(CUserProfileManager)];
    unsigned char before[sizeof(CUserProfileManager)];
    memset(storage, 0x5A, sizeof(storage));
    storage[0x33] = flag;
    memcpy(before, storage, sizeof(storage));
    CUserProfileManager* manager = reinterpret_cast<CUserProfileManager*>(storage);
    g_count = 0;
    manager->Apply();
    if (g_count != 1 + expectVideo) return 1;
    if (g_calls[0] != manager || g_callKinds[0] != 1) return 2;
    if (expectVideo && (g_calls[1] != manager || g_callKinds[1] != 2)) return 3;
    if (memcmp(before, storage, sizeof(storage)) != 0) return 4;
    return 0;
}

int main()
{
    int r;
    if ((r = Check(1, 1)) != 0) { printf("FAIL video path %d\n", r); return 1; }
    if ((r = Check(0, 0)) != 0) { printf("FAIL no-video path %d\n", r); return 1; }
    if ((r = Check(0x80, 1)) != 0) { printf("FAIL nonzero flag %d\n", r); return 1; }
    printf("CUserProfileManager_0040bc80_TEST PASS\n");
    return 0;
}