#include "engine/CNetworkClient.h"
#include <stdio.h>
#include <string.h>


void __fastcall Update(CNetworkClient *self, void * /*edx*/, int a, int b, int c)
{
    if (self->LocalGame) {
        self->LocalFrame = a;
        self->Checksum1 = b;
        self->Checksum2 = c;
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
    obj.LocalGame = 0;
    obj.LocalFrame = obj.Checksum1 = obj.Checksum2 = 0x11111111;
    call_update(&obj, 1, 2, 3);
    if (obj.LocalFrame != 0x11111111 || obj.Checksum1 != 0x11111111 || obj.Checksum2 != 0x11111111) {
        printf("FAIL disabled path wrote\n");
        return 1;
    }

    // enabled == 1 : must write all three
    obj.LocalGame = 1;
    obj.LocalFrame = obj.Checksum1 = obj.Checksum2 = 0;
    call_update(&obj, 0xAAAA0001, 0xBBBB0002, 0xCCCC0003);
    if (obj.LocalFrame != (int)0xAAAA0001 || obj.Checksum1 != (int)0xBBBB0002 || obj.Checksum2 != (int)0xCCCC0003) {
        printf("FAIL enabled path wrong a=%08x b=%08x c=%08x\n", obj.LocalFrame, obj.Checksum1, obj.Checksum2);
        return 1;
    }

    printf("OK_0x004ae9d0\n");
    return 0;
}