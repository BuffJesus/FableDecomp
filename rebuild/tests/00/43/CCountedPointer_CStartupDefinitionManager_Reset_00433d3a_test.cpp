// Behaviour of CCountedPointer<CStartupDefinitionManager>::Reset 0x00433D3A:
// the current reference is released through ReleaseData (the object deleted
// only with its last owner, its info block freed), then a non-null object gets
// a fresh info block (count 1, a deleter, the object); Reset(0) leaves the
// pointer empty.
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
    CStartupDefinitionManager* b = reinterpret_cast<CStartupDefinitionManager*>(0x2000);

    p->Reset(a);
    CCPPointerInfo* fresh = p->Info;
    if (p->Data != a || !fresh || fresh->RefCount != 1 || fresh->Data != a || !fresh->DeleteFunc) { puts("empty"); return 1; }
    delete fresh;

    // Shared: one reference dropped, nothing deleted.
    CCPPointerInfo* shared = new CCPPointerInfo(&CountDelete, a);
    shared->RefCount = 2;
    p->Info = shared;
    p->Reset(b);
    if (shared->RefCount != 1 || g_deletes != 0 || p->Data != b || p->Info == shared || p->Info->Data != b ||
        p->Info->RefCount != 1) { puts("shared"); return 1; }
    delete p->Info;

    // Last owner: the object goes through the old deleter; Reset(0) stays empty.
    p->Info = shared;
    p->Data = a;
    p->Reset(0);
    if (g_deletes != 1 || g_deleted != a || p->Data != 0 || p->Info != 0) { puts("last"); return 1; }
    puts("FABLETLC_CCOUNTEDPOINTER_STARTUPDEFMGR_RESET_00433D3A_PASS");
    return 0;
}
