#include <cstdio>

struct SubObj { unsigned int field0; };
struct CMemoryDataOutputStream { char pad[8]; SubObj sub; };

static int g_called = 0;
void __fastcall SubHelper(SubObj* sub) { g_called++; (void)sub; }

void __fastcall Clear(CMemoryDataOutputStream* self)
{
    SubHelper(&self->sub);
    self->sub.field0 = 0;
}

int main()
{
    CMemoryDataOutputStream obj;
    obj.sub.field0 = 0xDEADBEEF;
    Clear(&obj);
    if (g_called == 1 && obj.sub.field0 == 0) {
        printf("CLEAR_OK\n");
    } else {
        printf("CLEAR_FAIL called=%d f0=%08x\n", g_called, obj.sub.field0);
    }
    return 0;
}