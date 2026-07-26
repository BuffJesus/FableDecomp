#include "rebuild_abi.h"
#include <stdio.h>

struct CLandscapeBackgroundPatch { void *vtbl; };

static int g_dtor_called = 0;
static int g_delete_called = 0;

extern "C" void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch *thisptr) {
    (void)thisptr; g_dtor_called = 1;
}
extern "C" void __cdecl sub_operator_delete(void *p) {
    (void)p; g_delete_called = 1;
}

void * __fastcall CLandscapeBackgroundPatch_vector_deleting_dtor(
        CLandscapeBackgroundPatch *thisptr, int edx, unsigned int flags);

int main() {
    CLandscapeBackgroundPatch obj;
    obj.vtbl = 0;

    g_dtor_called = 0; g_delete_called = 0;
    void *r1 = CLandscapeBackgroundPatch_vector_deleting_dtor(&obj, 0, 0);

    g_dtor_called = 0; g_delete_called = 0;
    void *r2 = CLandscapeBackgroundPatch_vector_deleting_dtor(&obj, 0, 1);

    if (r1 == &obj && r2 == &obj && g_dtor_called == 1 && g_delete_called == 1) {
        printf("PASS_459aa2_OK\n");
    } else {
        printf("FAIL_459aa2\n");
    }
    return 0;
}