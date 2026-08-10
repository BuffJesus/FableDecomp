#include <cstdio>
struct CThing;
struct CThingFilter_HasScriptSupervision { bool m_wanted; };
static int g_ret = 0;
extern "C" bool __fastcall Thing_IsUnderScriptSupervision(CThing* thing){ return g_ret != 0; }
bool __fastcall CThingFilter_HasScriptSupervision__op(CThingFilter_HasScriptSupervision* self, void* edx, CThing* thing);
int main(){
    CThingFilter_HasScriptSupervision f;
    f.m_wanted = true;  g_ret = 1; bool a = CThingFilter_HasScriptSupervision__op(&f,0,(CThing*)0x1234);
    f.m_wanted = true;  g_ret = 0; bool b = CThingFilter_HasScriptSupervision__op(&f,0,(CThing*)0x1234);
    f.m_wanted = false; g_ret = 0; bool c = CThingFilter_HasScriptSupervision__op(&f,0,(CThing*)0x1234);
    f.m_wanted = false; g_ret = 1; bool d = CThingFilter_HasScriptSupervision__op(&f,0,(CThing*)0x1234);
    if (a && !b && c && !d) printf("OK_006a7530\n");
    else printf("FAIL a=%d b=%d c=%d d=%d\n", a,b,c,d);
    return 0;
}