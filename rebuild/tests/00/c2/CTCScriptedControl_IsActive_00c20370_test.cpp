#include <cstdio>

struct CTCScriptedControl {
    char pad[0xc];
    char m_field0c;
};

static bool g_compareResult = false;
static int g_compareCalls = 0;

bool __fastcall CTCScriptedControl_Compare(CTCScriptedControl* self, CTCScriptedControl* other)
{
    (void)self; (void)other;
    g_compareCalls++;
    return g_compareResult;
}

int __fastcall CTCScriptedControl_IsActive(CTCScriptedControl* self, CTCScriptedControl* other);

int main()
{
    CTCScriptedControl a, b;
    a.m_field0c = 5;
    b.m_field0c = 7;

    // Fields differ: returns 0, helper NOT called.
    g_compareResult = true; g_compareCalls = 0;
    int r1 = CTCScriptedControl_IsActive(&a, &b);

    // Fields equal, helper false -> 0.
    b.m_field0c = 5;
    g_compareResult = false; g_compareCalls = 0;
    int r2 = CTCScriptedControl_IsActive(&a, &b);
    int calls2 = g_compareCalls;

    // Fields equal, helper true -> 1.
    g_compareResult = true; g_compareCalls = 0;
    int r3 = CTCScriptedControl_IsActive(&a, &b);
    int calls3 = g_compareCalls;

    if (r1 == 0 && r2 == 0 && calls2 == 1 && r3 == 1 && calls3 == 1) {
        printf("ISACTIVE_OK\n");
        return 0;
    }
    printf("ISACTIVE_FAIL r1=%d r2=%d c2=%d r3=%d c3=%d\n", r1, r2, calls2, r3, calls3);
    return 1;
}