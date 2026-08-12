#include <stdio.h>

struct CLandscapeBackgroundPatch { int tag; };

static int g_dtor_calls = 0;
static int g_delete_calls = 0;
static void* g_last_deleted = 0;

void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* self)
{
    g_dtor_calls++;
    self->tag = -1;
}
void operator_delete_stub(void* p)
{
    g_delete_calls++;
    g_last_deleted = p;
}

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(
        CLandscapeBackgroundPatch* self, int edx, unsigned int flags);

int main()
{
    CLandscapeBackgroundPatch a; a.tag = 7;
    // flags & 1 == 0 -> no delete
    void* r1 = CLandscapeBackgroundPatch_vector_deleting_destructor(&a, 0, 0);
    // flags & 1 == 1 -> delete
    CLandscapeBackgroundPatch b; b.tag = 9;
    void* r2 = CLandscapeBackgroundPatch_vector_deleting_destructor(&b, 0, 3);

    int ok = (r1 == &a) && (r2 == &b)
          && (g_dtor_calls == 2) && (g_delete_calls == 1)
          && (g_last_deleted == &b) && (a.tag == -1) && (b.tag == -1);
    if (ok) printf("VDD_OK_0048b6a0\n");
    else printf("VDD_FAIL d=%d del=%d\n", g_dtor_calls, g_delete_calls);
    return ok ? 0 : 1;
}