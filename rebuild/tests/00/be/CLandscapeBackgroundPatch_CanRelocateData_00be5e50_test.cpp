#include "engine/CLandscapeBackgroundPatch.h"
#include <cstdio>
struct Inner { char pad[0x5c]; unsigned char flag; };
struct CLandscapeBackgroundTreeNode { char pad[0xa4]; Inner* inner; };

bool __fastcall CanRelocateData(CLandscapeBackgroundPatch* self, int /*edx*/, long a) {
    (void)a;
    return self->Parent->inner->flag == 0;
}

int main() {
    Inner in0; in0.flag = 0;
    Inner in1; in1.flag = 1;
    CLandscapeBackgroundTreeNode m0; m0.inner = &in0;
    CLandscapeBackgroundTreeNode m1; m1.inner = &in1;
    CLandscapeBackgroundPatch p0; p0.Parent = &m0;
    CLandscapeBackgroundPatch p1; p1.Parent = &m1;
    if (CanRelocateData(&p0, 0, 7L) != true) { printf("FAIL0\n"); return 1; }
    if (CanRelocateData(&p1, 0, 7L) != false) { printf("FAIL1\n"); return 2; }
    printf("OK_00be5e50\n");
    return 0;
}