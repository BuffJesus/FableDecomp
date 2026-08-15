#include <cstdio>

struct Owner48; struct Sub3c;

static int g_freed = 0;
static int g_notified = 0;
static int g_basecalled = 0;

void __cdecl FreeOwner48(Owner48* p) { if (p) g_freed++; }
void __fastcall Sub3c_Notify(Sub3c* self) { g_notified++; }
void __fastcall Base_OnDie(void* self) { g_basecalled++; }

struct CTCInventoryItem {
    char pad[0x3c];
    Sub3c* sub3c_marker;
};

void __fastcall CTCInventoryItem_OnDie(CTCInventoryItem* self)
{
    Owner48* p = *(Owner48**)((char*)self + 0x48);
    if (p) FreeOwner48(p);
    Sub3c_Notify((Sub3c*)((char*)self + 0x3c));
    Base_OnDie(self);
}

int main() {
    char buf[0x60];
    // case 1: owner at +0x48 nonzero
    for (int i=0;i<0x60;i++) buf[i]=0;
    *(void**)(buf+0x48) = (void*)0x1234;
    CTCInventoryItem_OnDie((CTCInventoryItem*)buf);
    // case 2: owner null
    *(void**)(buf+0x48) = 0;
    CTCInventoryItem_OnDie((CTCInventoryItem*)buf);

    if (g_freed==1 && g_notified==2 && g_basecalled==2)
        printf("PARITY_OK\n");
    else
        printf("PARITY_FAIL f=%d n=%d b=%d\n", g_freed, g_notified, g_basecalled);
    return 0;
}