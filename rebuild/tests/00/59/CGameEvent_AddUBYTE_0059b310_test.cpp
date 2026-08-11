#include <stdio.h>

struct CGameEvent
{
    void Helper(void* p, int n);
    CGameEvent* AddUBYTE();
    int marker;
};

static void* g_seenPtr = 0;
static int   g_seenN   = -1;
static int   g_calls   = 0;

void CGameEvent::Helper(void* p, int n)
{
    g_seenPtr = p;
    g_seenN   = n;
    g_calls  += 1;
}

CGameEvent* CGameEvent::AddUBYTE()
{
    unsigned char buf;
    Helper(&buf, 0);
    return this;
}

int main()
{
    CGameEvent ev;
    ev.marker = 0x1234;

    CGameEvent* r = ev.AddUBYTE();

    int ok = 1;
    if (r != &ev)        ok = 0;   // returns this
    if (g_calls != 1)    ok = 0;   // Helper called exactly once
    if (g_seenN != 0)    ok = 0;   // n == 0
    if (g_seenPtr == 0)  ok = 0;   // a real buffer pointer was passed

    if (ok) printf("ADDUBYTE_OK marker=%x\n", ev.marker);
    else    printf("ADDUBYTE_FAIL calls=%d n=%d ptr=%p ret=%p this=%p\n",
                   g_calls, g_seenN, g_seenPtr, (void*)r, (void*)&ev);
    return ok ? 0 : 1;
}