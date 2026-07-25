#include <cstdio>

struct CDefClassBase;
struct CLensFlareElementDef;

// Stand-in impl for the test: record that the thunk forwarded correctly.
static int g_called = 0;
extern "C" void __fastcall CLensFlareElementDef_Copy_impl(CLensFlareElementDef* thisptr, const CDefClassBase* other)
{
    (void)thisptr; (void)other;
    g_called = 1;
}

void __fastcall CLensFlareElementDef_Copy(CLensFlareElementDef* thisptr, const CDefClassBase* other);

int main()
{
    CLensFlareElementDef* self = (CLensFlareElementDef*)0x1000;
    const CDefClassBase* src = (const CDefClassBase*)0x2000;
    CLensFlareElementDef_Copy(self, src);
    if (g_called == 1)
        printf("OK_046f77c\n");
    else
        printf("FAIL_046f77c\n");
    return 0;
}