#include <stdio.h>
struct Sub { int x; };
struct TavernServer {
    char pad[0x34];
    Sub sub;
    int Act(int a, int b);
};
extern "C" int __fastcall WanderWithinTavern(TavernServer* self);

static Sub* g_sub;
static TavernServer* g_self;
static int g_a, g_b;

extern "C" int __fastcall Sub_Get(Sub* s){ g_sub = s; return 0x1234; }
int TavernServer::Act(int a, int b){ g_self = this; g_a = a; g_b = b; return 0x99; }

int main(){
    TavernServer ts;
    g_sub = 0; g_self = 0; g_a = 0; g_b = 0;
    int rv = WanderWithinTavern(&ts);
    int ok = (g_sub == &ts.sub) && (g_self == &ts) && (g_a == 3) && (g_b == 0x1234) && (rv == 0x99);
    if (ok) printf("OK_00966ad0\n");
    else printf("FAIL sub=%p self=%p a=%d b=%d rv=%d\n", (void*)g_sub, (void*)g_self, g_a, g_b, rv);
    return 0;
}