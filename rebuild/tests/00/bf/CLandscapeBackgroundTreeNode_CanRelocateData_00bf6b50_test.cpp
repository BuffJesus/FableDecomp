#include <stdio.h>
struct Inner { char pad[0x5c]; unsigned char flag; };
struct CLandscapeBackgroundTreeNode { char pad[0xa4]; struct Inner* inner; };
static int model(struct CLandscapeBackgroundTreeNode* self, long a) {
    (void)a;
    return self->inner->flag == 0;
}
int main(void) {
    struct Inner i0; struct Inner i1;
    struct CLandscapeBackgroundTreeNode n;
    int r0, r1;
    i0.flag = 0;
    i1.flag = 7;
    n.inner = &i0;
    r0 = model(&n, 123);
    n.inner = &i1;
    r1 = model(&n, -5);
    if (r0 == 1 && r1 == 0) { printf("OK_00bf6b50"); return 0; }
    printf("FAIL"); return 1;
}