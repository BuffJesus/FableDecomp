#include <cstdio>

struct Sub28 { char pad[4]; };

struct CDraw2DText {
    char pad0[0x18];
    unsigned char f18;
    char pad19[0x28-0x19];
    Sub28 f28;
};

static int g_helper_called = 0;
static int g_base_called = 0;
static Sub28* g_helper_arg = 0;
static CDraw2DText* g_base_arg = 0;

void __fastcall helper_440770(Sub28* self){ g_helper_called++; g_helper_arg = self; }
void __fastcall base_644627(CDraw2DText* self){ g_base_called++; g_base_arg = self; }

void __fastcall CDraw2DText_ClearPrimitivesIfNecessary(CDraw2DText* self);

int main(){
    CDraw2DText o;
    // case: f18 == 0 -> helper runs, base runs
    o.f18 = 0;
    g_helper_called=g_base_called=0;
    CDraw2DText_ClearPrimitivesIfNecessary(&o);
    bool ok1 = (g_helper_called==1) && (g_base_called==1)
        && (g_helper_arg == &o.f28) && (g_base_arg == &o);

    // case: f18 != 0 -> helper skipped, base still runs
    o.f18 = 5;
    g_helper_called=g_base_called=0;
    CDraw2DText_ClearPrimitivesIfNecessary(&o);
    bool ok2 = (g_helper_called==0) && (g_base_called==1) && (g_base_arg == &o);

    if(ok1 && ok2) printf("00646567_TEST PASS\n");
    else printf("FAIL h=%d b=%d\n", g_helper_called, g_base_called);
    return 0;
}