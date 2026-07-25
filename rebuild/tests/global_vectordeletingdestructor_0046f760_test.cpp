#include "rebuild_abi.h"
#include <cstdio>

struct CLandscapeBackgroundPatch;

extern "C" void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* thisptr) { (void)thisptr; }
extern "C" void __cdecl operator_delete_thunk(void* p) { (void)p; }

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_dtor(
        CLandscapeBackgroundPatch* thisptr, int edx, unsigned int flags);

int main()
{
    char buf[8];
    void* r = CLandscapeBackgroundPatch_vector_deleting_dtor(
                  (CLandscapeBackgroundPatch*)buf, 0, 0u);
    if (r == (void*)buf)
        printf("OK_0046f760\n");
    else
        printf("BAD_0046f760\n");
    return 0;
}