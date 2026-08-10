#include <cstdio>
struct CCharString { char* p; };
static int g_called=0; static const char* g_arg=0; static int g_n=0;
struct Helper {
    void Do(const char* s, int n){ g_called=1; g_arg=s; g_n=n; }
};
extern "C" CCharString* __fastcall GetActionName_9(Helper* self)
{
    self->Do((const char*)0x123e6ec, -1);
    return (CCharString*)self;
}
int main(){
    Helper obj;
    CCharString* r = GetActionName_9(&obj);
    if((void*)r != (void*)&obj){ printf("FAIL ret\n"); return 1; }
    if(!g_called){ printf("FAIL call\n"); return 1; }
    if(g_arg != (const char*)0x123e6ec){ printf("FAIL arg\n"); return 1; }
    if(g_n != -1){ printf("FAIL n\n"); return 1; }
    printf("OK_004d66c3\n");
    return 0;
}