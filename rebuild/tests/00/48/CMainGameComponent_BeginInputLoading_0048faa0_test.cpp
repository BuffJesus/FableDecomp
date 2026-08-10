#include <cstdio>
// Behavioural model of BeginInputLoading: forwarder that calls target with
// mode=0x47 and the wide-string pointer, cleaning 0xC bytes of stack.
static int g_calls=0; static void* g_ws=0; static int g_mode=-1;
static void Target(void* ws, int mode){ g_calls++; g_ws=ws; g_mode=mode; }
static void Model(void* self, void* ws){ (void)self; Target(ws, 0x47); }
int main(){
    int dummy=0; void* self=&dummy; int ws=1234; void* wsp=&ws;
    Model(self, wsp);
    if(g_calls!=1){ printf("FAIL calls\n"); return 1; }
    if(g_ws!=wsp){ printf("FAIL ws\n"); return 2; }
    if(g_mode!=0x47){ printf("FAIL mode\n"); return 3; }
    // second invocation, null ws branch
    Model(self, 0);
    if(g_calls!=2 || g_ws!=0 || g_mode!=0x47){ printf("FAIL2\n"); return 4; }
    printf("OK_0048faa0\n");
    return 0;
}