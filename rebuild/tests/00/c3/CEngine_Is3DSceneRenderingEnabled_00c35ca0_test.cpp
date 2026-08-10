#include <stdio.h>

struct CEngine { unsigned char pad[0x10]; unsigned int flags; };

static bool model(CEngine* self)
{
    return (self->flags & 0x217) != 0;
}

int main()
{
    CEngine e;
    e.flags = 0;
    if (model(&e) != false) { printf("FAIL zero\n"); return 1; }
    e.flags = 0x217;
    if (model(&e) != true) { printf("FAIL all\n"); return 1; }
    e.flags = 0x001;
    if (model(&e) != true) { printf("FAIL bit0\n"); return 1; }
    e.flags = 0x200;
    if (model(&e) != true) { printf("FAIL bit9\n"); return 1; }
    e.flags = 0xE8;
    if (model(&e) != false) { printf("FAIL masked-out\n"); return 1; }
    e.flags = 0xFFFFFFFF;
    if (model(&e) != true) { printf("FAIL allbits\n"); return 1; }
    printf("OK_00c35ca0\n");
    return 0;
}