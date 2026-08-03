#include <stdio.h>
#include <string.h>

struct CNetworkClient {
    char _pad0[0x2662];
    unsigned char enabled;   // +0x2662
    char _pad1[0x266c - 0x2663];
    int  a;                  // +0x266c
    int  b;                  // +0x2670
    int  c;                  // +0x2674
};

void __fastcall Update(CNetworkClient *self, void * /*edx*/, int a, int b, int c)
{
    if (self->enabled) {
        self->a = a;
        self->b = b;
        self->c = c;
    }
}

// __fastcall shim: this in ecx, args pushed right-to-left, callee cleans (ret 0xc)
static void call_update(CNetworkClient *self, int a, int b, int c)
{
    __asm {
        push c
        push b
        push a
        mov  ecx, self
        call Update
    }
}

int main()
{
    CNetworkClient obj;
    memset(&obj, 0, sizeof(obj));

    // enabled == 0 : must NOT write
    obj.enabled = 0;
    obj.a = obj.b = obj.c = 0x11111111;
    call_update(&obj, 1, 2, 3);
    if (obj.a != 0x11111111 || obj.b != 0x11111111 || obj.c != 0x11111111) {
        printf("FAIL disabled path wrote\n");
        return 1;
    }

    // enabled == 1 : must write all three
    obj.enabled = 1;
    obj.a = obj.b = obj.c = 0;
    call_update(&obj, 0xAAAA0001, 0xBBBB0002, 0xCCCC0003);
    if (obj.a != (int)0xAAAA0001 || obj.b != (int)0xBBBB0002 || obj.c != (int)0xCCCC0003) {
        printf("FAIL enabled path wrong a=%08x b=%08x c=%08x\n", obj.a, obj.b, obj.c);
        return 1;
    }

    printf("OK_0x004ae9d0\n");
    return 0;
}