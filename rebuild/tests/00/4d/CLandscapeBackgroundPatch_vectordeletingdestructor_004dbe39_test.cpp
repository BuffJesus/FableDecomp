#include <stdio.h>
typedef unsigned int uint;

static int g_dtor_calls = 0;
static int g_delete_calls = 0;
static void* g_last_dtor_self = 0;
static void* g_last_delete_self = 0;

struct CLandscapeBackgroundPatch { char pad[8]; void* sub; };

void __fastcall Sub_dtor(void* subself) { g_dtor_calls++; g_last_dtor_self = subself; }
void __cdecl op_delete(void* p) { g_delete_calls++; g_last_delete_self = p; }

void* __fastcall vector_deleting_destructor(CLandscapeBackgroundPatch* self, int, uint flags)
{
    Sub_dtor((char*)self + 8);
    if (flags & 1)
        op_delete(self);
    return self;
}

int main()
{
    CLandscapeBackgroundPatch obj;
    g_dtor_calls = g_delete_calls = 0;
    void* r = vector_deleting_destructor(&obj, 0, 0);
    if (r != &obj) { printf("FAIL ret0\n"); return 1; }
    if (g_dtor_calls != 1) { printf("FAIL dtor0\n"); return 1; }
    if (g_delete_calls != 0) { printf("FAIL del0\n"); return 1; }
    if (g_last_dtor_self != (char*)&obj + 8) { printf("FAIL sub0\n"); return 1; }
    g_dtor_calls = g_delete_calls = 0;
    r = vector_deleting_destructor(&obj, 0, 1);
    if (r != &obj) { printf("FAIL ret1\n"); return 1; }
    if (g_dtor_calls != 1) { printf("FAIL dtor1\n"); return 1; }
    if (g_delete_calls != 1) { printf("FAIL del1\n"); return 1; }
    if (g_last_delete_self != &obj) { printf("FAIL delself\n"); return 1; }
    g_delete_calls = 0;
    vector_deleting_destructor(&obj, 0, 2);
    if (g_delete_calls != 0) { printf("FAIL del2\n"); return 1; }
    printf("BEHAVIOUR_OK_004dbe39\n");
    return 0;
}