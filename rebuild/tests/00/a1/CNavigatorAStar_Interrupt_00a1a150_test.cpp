#include <cstdio>
#include <cstring>

static void* g_notify_ecx = 0;
static void* g_notify_arg = 0;
static int   g_notify_calls = 0;

struct SubObject {
    void Notify(void* nav);
};

struct CNavigatorAStar {
    char pad0[0x19];
    unsigned char flag19;      // +0x19
    char pad1[0x1c - 0x1a];
    char* sub;                 // +0x1c
    char pad2[0x88 - 0x20];
    int field88;               // +0x88

    void Interrupt();
};

void SubObject::Notify(void* nav)
{
    g_notify_ecx = this;
    g_notify_arg = nav;
    ++g_notify_calls;
}

void CNavigatorAStar::Interrupt()
{
    this->flag19 = 1;
    this->field88 = 0;
    SubObject* s = (SubObject*)(this->sub + 0x2c);
    s->Notify(this);
}

int main()
{
    CNavigatorAStar nav;
    memset(&nav, 0xAB, sizeof(nav));
    nav.flag19 = 0;
    nav.field88 = 12345;

    char block[0x100];
    memset(block, 0, sizeof(block));
    nav.sub = block;

    nav.Interrupt();

    bool ok = true;
    if (nav.flag19 != 1) ok = false;
    if (nav.field88 != 0) ok = false;
    if (g_notify_calls != 1) ok = false;
    if (g_notify_ecx != (void*)(block + 0x2c)) ok = false;
    if (g_notify_arg != (void*)&nav) ok = false;

    if (ok) { printf("INTERRUPT_OK\n"); return 0; }
    printf("INTERRUPT_FAIL flag=%d f88=%d calls=%d\n",
           (int)nav.flag19, nav.field88, g_notify_calls);
    return 1;
}