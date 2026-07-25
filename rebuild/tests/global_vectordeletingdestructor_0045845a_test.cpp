#include "rebuild_abi.h"
#include <cstdio>

struct CLandscapeBackgroundPatch { int destroyed; int freed; };

static CLandscapeBackgroundPatch* g_obj;

extern "C" void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* thisptr) {
    thisptr->destroyed = 1;
}
extern "C" void __cdecl engine_operator_delete(void* p) {
    ((CLandscapeBackgroundPatch*)p)->freed = 1;
}

void* __fastcall LandscapeBackgroundPatch_vector_deleting_dtor(
    CLandscapeBackgroundPatch* thisptr, int, unsigned int flag);

int main() {
    CLandscapeBackgroundPatch obj; obj.destroyed = 0; obj.freed = 0;
    // flag=0: dtor runs, no delete
    void* r0 = LandscapeBackgroundPatch_vector_deleting_dtor(&obj, 0, 0);
    // flag=1: dtor runs, delete runs
    CLandscapeBackgroundPatch obj2; obj2.destroyed = 0; obj2.freed = 0;
    void* r1 = LandscapeBackgroundPatch_vector_deleting_dtor(&obj2, 0, 1);

    if (r0 == &obj && obj.destroyed == 1 && obj.freed == 0 &&
        r1 == &obj2 && obj2.destroyed == 1 && obj2.freed == 1) {
        printf("OK_0045845a\n");
    } else {
        printf("FAIL_0045845a\n");
    }
    return 0;
}