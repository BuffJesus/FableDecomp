// Behaviour of CCountedPointer<CStartupDefinitionManager>::ReleaseData 0x00433CC0:
// with an info block the count drops by one, and at zero the deleter runs on
// the block's object and the block is freed; either way Data and Info end null.
#include "fable_counted_pointer.h"
#include "fable_definition_manager.h"
#include <stdio.h>
#include <string.h>

typedef CCountedPointer<CStartupDefinitionManager> Pointer;
static int g_deletes;
static void* g_deleted;
static void FABLE_FASTCALL CountDelete(void* data) { ++g_deletes; g_deleted = data; }

int main()
{
    unsigned char store[sizeof(Pointer)];
    memset(store, 0, sizeof(store));
    Pointer* p = reinterpret_cast<Pointer*>(store);
    CStartupDefinitionManager* a = reinterpret_cast<CStartupDefinitionManager*>(0x1000);

    p->Data = a;
    p->ReleaseData();
    if (p->Data != 0 || p->Info != 0 || g_deletes != 0) { puts("no info"); return 1; }

    CCPPointerInfo* info = new CCPPointerInfo(&CountDelete, (void*)0x3000);
    info->RefCount = 2;
    p->Data = a;
    p->Info = info;
    p->ReleaseData();
    if (info->RefCount != 1 || g_deletes != 0 || p->Data != 0 || p->Info != 0) { puts("shared"); return 1; }

    p->Data = a;
    p->Info = info;
    p->ReleaseData();
    if (g_deletes != 1 || g_deleted != (void*)0x3000 || p->Data != 0 || p->Info != 0) { puts("last"); return 1; }
    puts("FABLETLC_CCOUNTEDPOINTER_STARTUPDEFMGR_RELEASEDATA_00433CC0_PASS");
    return 0;
}
