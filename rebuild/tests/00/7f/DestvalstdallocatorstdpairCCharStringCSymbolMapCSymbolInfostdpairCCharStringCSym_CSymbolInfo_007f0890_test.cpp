#include <stdio.h>

struct Pair;

static int g_calls = 0;
static void* g_last = 0;

extern "C" void __fastcall Dtor_first(Pair* self){ g_calls++; g_last = (void*)self; }
extern "C" void __fastcall Dtor_second(Pair* self){ g_calls++; g_last = (void*)self; }
extern "C" void __fastcall Dtor_node(Pair* self){ g_calls++; g_last = (void*)self; }

void __fastcall Dest_val(Pair* self);

int main(){
    int dummy = 0;
    Pair* p = (Pair*)&dummy;
    Dest_val(p);
    if (g_calls == 3 && g_last == (void*)p)
        printf("OK_007f0890\n");
    else
        printf("FAIL calls=%d\n", g_calls);
    return 0;
}