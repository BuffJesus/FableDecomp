#include <cstdio>

struct CSoundThemeDef;

// Provide a definition for the extern so the test links; it just records the call.
static int g_called = 0;
extern "C" void mark_called() { g_called = 1; }

// Re-declare the thunk locally for the test harness.
extern void __fastcall CSoundThemeDef_Copy(CSoundThemeDef* thisptr, void* src);

// Stand-in impl the thunk jumps to.
extern void __fastcall CSoundThemeDef_Copy_impl(CSoundThemeDef* thisptr, void* src);
void __fastcall CSoundThemeDef_Copy_impl(CSoundThemeDef* /*thisptr*/, void* /*src*/) { mark_called(); }

int main()
{
    CSoundThemeDef* obj = (CSoundThemeDef*)0x1000;
    void* src = (void*)0x2000;
    CSoundThemeDef_Copy(obj, src);
    if (g_called == 1) {
        printf("OK_0045c549_THUNK\n");
    } else {
        printf("FAIL_0045c549\n");
    }
    return 0;
}