#include <stdio.h>
#include <stdlib.h>

static int g_helper_called = 0;
static int g_freed = 0;

struct CTCMapwho {
    int marker;
};

extern "C" void __fastcall Helper_die(CTCMapwho* self)
{
    g_helper_called++;
}
extern "C" void __cdecl retail_free(void* p)
{
    g_freed++;
    free(p);
}

void __fastcall OnDie(CTCMapwho* self);

int main()
{
    OnDie(0);
    if (g_helper_called != 0 || g_freed != 0) { printf("FAIL null\n"); return 1; }

    CTCMapwho* m = (CTCMapwho*)malloc(sizeof(CTCMapwho));
    m->marker = 7;
    OnDie(m);
    if (g_helper_called != 1 || g_freed != 1) { printf("FAIL live\n"); return 1; }

    printf("OK_00812150\n");
    return 0;
}