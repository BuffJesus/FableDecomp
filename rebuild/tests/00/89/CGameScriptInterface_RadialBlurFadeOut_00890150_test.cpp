#include <cstdio>

static void* g_this = 0;
static int g_a = 0, g_b = 0;

struct Tgt { void method(int a, int b); };
void Tgt::method(int a, int b){ g_this = this; g_a = a; g_b = b; }

struct B { char pad[0x18]; char* p18; };
struct Self { char pad[4]; B* p4; };

void __fastcall CGameScriptInterface_RadialBlurFadeOut(Self* self, int edx, int a, int b)
{
    char* mid = self->p4->p18;
    Tgt* t = *(Tgt**)(mid + 0x1964);
    t->method(a, b);
}

int main(){
    char midblock[0x1968];
    Tgt tgtObj;
    *(void**)(midblock + 0x1964) = (void*)&tgtObj;
    B b; b.p18 = midblock;
    Self self; self.p4 = &b;
    CGameScriptInterface_RadialBlurFadeOut(&self, 0, 111, 222);
    if (g_this == (void*)&tgtObj && g_a == 111 && g_b == 222) {
        std::printf("CGameScriptInterface_00890150_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}