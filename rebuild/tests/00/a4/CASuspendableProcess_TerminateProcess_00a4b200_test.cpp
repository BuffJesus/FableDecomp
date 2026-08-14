#include <cstdio>

struct CASuspendableProcess;

struct CASuspendableProcessVtbl {
    void (__fastcall *slot0)(CASuspendableProcess* self);
    void (__fastcall *Pump)(CASuspendableProcess* self);
};

#pragma pack(push,1)
struct CASuspendableProcess {
    CASuspendableProcessVtbl* vtbl;
    unsigned char active;
    unsigned char terminateRequested;
};
#pragma pack(pop)

void __fastcall TerminateProcess(CASuspendableProcess* self)
{
    self->terminateRequested = 1;
    while (self->active) {
        self->vtbl->Pump(self);
    }
}

static int g_pumps = 0;

// Pump decrements a countdown; when it hits 0, clears active.
void __fastcall TestPump(CASuspendableProcess* self)
{
    g_pumps++;
    if (g_pumps >= 3) self->active = 0;
}

int main()
{
    CASuspendableProcessVtbl vt;
    vt.slot0 = 0;
    vt.Pump = TestPump;

    // Case 1: active from the start -> pumps until cleared.
    CASuspendableProcess p;
    p.vtbl = &vt;
    p.active = 1;
    p.terminateRequested = 0;
    g_pumps = 0;
    TerminateProcess(&p);
    int ok1 = (p.terminateRequested == 1) && (p.active == 0) && (g_pumps == 3);

    // Case 2: already inactive -> sets flag, no pump.
    CASuspendableProcess q;
    q.vtbl = &vt;
    q.active = 0;
    q.terminateRequested = 0;
    g_pumps = 0;
    TerminateProcess(&q);
    int ok2 = (q.terminateRequested == 1) && (q.active == 0) && (g_pumps == 0);

    if (ok1 && ok2) printf("PARITY_OK\n");
    else printf("PARITY_FAIL %d %d\n", ok1, ok2);
    return 0;
}