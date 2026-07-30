
#include <stdio.h>

struct Thread { char pad[0x18]; char terminating; };
struct CScriptBase { char pad[0x8]; Thread* activeThread; };

char __fastcall IsActiveThreadTerminating(CScriptBase* self)
{
    Thread* t = self->activeThread;
    if (t)
        return t->terminating;
    return 1;
}

static unsigned char call_it(CScriptBase* self)
{
    unsigned char r;
    __asm {
        mov ecx, self
        call IsActiveThreadTerminating
        mov r, al
    }
    return r;
}

int main()
{
    // Case 1: null thread -> returns 1 (true)
    CScriptBase a;
    a.activeThread = 0;
    if (call_it(&a) != 1) { printf("FAIL case1\n"); return 1; }

    // Case 2: thread terminating flag = 1 -> returns 1
    Thread th;
    th.terminating = 1;
    CScriptBase b;
    b.activeThread = &th;
    if (call_it(&b) != 1) { printf("FAIL case2\n"); return 1; }

    // Case 3: thread terminating flag = 0 -> returns 0
    th.terminating = 0;
    if (call_it(&b) != 0) { printf("FAIL case3\n"); return 1; }

    // Case 4: raw byte passthrough (retail returns raw byte, not normalized)
    th.terminating = (char)0x18;
    if (call_it(&b) != 0x18) { printf("FAIL case4\n"); return 1; }

    printf("OK_0x005bc746\n");
    return 0;
}