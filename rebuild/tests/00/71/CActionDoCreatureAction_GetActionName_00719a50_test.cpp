#include <cstdio>

struct CCharString {
    void* p;
    CCharString(const char* s, int n);
};
struct CActionDoCreatureAction {
    CCharString GetActionName();
};

static const char* g_s; static int g_n; static int g_calls;
CCharString::CCharString(const char* s, int n){ g_s=s; g_n=n; g_calls++; p=(void*)s; }

int main(){
    CActionDoCreatureAction obj;
    CCharString r = obj.GetActionName();
    if (g_calls==1 && g_s==(const char*)0x123a514 && g_n==-1 && r.p==(void*)0x123a514)
        printf("00719a50_TEST PASS\n");
    else
        printf("FAIL calls=%d s=%p n=%d\n", g_calls, g_s, g_n);
    return 0;
}