#include <cstdio>

struct MsgEventList { void* p; };

static int g_baseCalls = 0;
static void* g_freed = (void*)-1;

extern "C" void __fastcall BaseDtor_C0BE20(MsgEventList* self) { g_baseCalls++; (void)self; }

typedef void (__cdecl *FreeFn)(void*);
static void __cdecl myFree(void* x) { g_freed = x; }

static FreeFn g_freeStore = myFree;
#define g_free (*(FreeFn*)&g_freeStore)

extern "C" void __fastcall DtorMsgEventList_C(MsgEventList* self)
{
    BaseDtor_C0BE20(self);
    g_free(self->p);
}

int main()
{
    MsgEventList m;
    int sentinel = 0;
    m.p = &sentinel;
    DtorMsgEventList_C(&m);
    if (g_baseCalls == 1 && g_freed == &sentinel) {
        printf("OK_00c0c950\n");
        return 0;
    }
    printf("FAIL\n");
    return 1;
}