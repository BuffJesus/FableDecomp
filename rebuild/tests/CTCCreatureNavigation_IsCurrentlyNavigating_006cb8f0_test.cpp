#include <cstdio>
struct CTCCreatureNavigation {
    char pad[0x50];
    int cur;
};
static int g_a = 0;
static int g_b = 0;
extern "C" int helperA(void){ return g_a; }
extern "C" int helperB(void){ return g_b; }
int __fastcall CTCCreatureNavigation_IsCurrentlyNavigating(CTCCreatureNavigation* self)
{
    if (self->cur >= 0) {
        helperA();
        if (helperB() - self->cur <= 1)
            return 1;
    }
    return 0;
}
int main(){
    CTCCreatureNavigation o;
    o.cur = -5;
    if (CTCCreatureNavigation_IsCurrentlyNavigating(&o)) { std::printf("FAIL neg\n"); return 1; }
    o.cur = 10; g_b = 10;
    if (!CTCCreatureNavigation_IsCurrentlyNavigating(&o)) { std::printf("FAIL eq\n"); return 1; }
    o.cur = 10; g_b = 11;
    if (!CTCCreatureNavigation_IsCurrentlyNavigating(&o)) { std::printf("FAIL one\n"); return 1; }
    o.cur = 10; g_b = 12;
    if (CTCCreatureNavigation_IsCurrentlyNavigating(&o)) { std::printf("FAIL two\n"); return 1; }
    std::printf("CTCCreatureNavigation_006cb8f0_TEST PASS\n");
    return 0;
}