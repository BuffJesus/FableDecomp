#include <stdio.h>

struct CInputProcessJumpingAndRolling {
    char pad[0x4c];
    char field_4c;
};

struct CProcessedInput { int dummy; };

extern "C" char __stdcall JR_Helper(int a, int b);
void __fastcall Update(CInputProcessJumpingAndRolling* self, int edx_ignored, CProcessedInput& in);

static int g_a, g_b, g_calls;
static char g_ret;
extern "C" char __stdcall JR_Helper(int a, int b){ g_a=a; g_b=b; g_calls++; return g_ret; }

int main(){
    CInputProcessJumpingAndRolling o;
    CProcessedInput in;
    // case: helper returns 0 -> field_4c set to 0
    o.field_4c = 0x55;
    g_ret = 0; g_calls = 0;
    Update(&o, 0, in);
    if (g_calls != 1) { printf("bad calls\n"); return 1; }
    if (g_a != 0x3c || g_b != 2) { printf("bad args %d %d\n", g_a, g_b); return 1; }
    if (o.field_4c != 0) { printf("field not cleared\n"); return 1; }
    // case: helper returns nonzero -> field untouched
    o.field_4c = 0x55;
    g_ret = 1;
    Update(&o, 0, in);
    if (o.field_4c != 0x55) { printf("field wrongly touched\n"); return 1; }
    printf("OK_006a40b0\n");
    return 0;
}