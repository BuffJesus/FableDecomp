#include <stdio.h>

struct CThing {
    char pad[0x91];
    unsigned char flags;   // +0x91
};

void __fastcall SetAsGameStatePersisted(CThing* self, void* /*edx*/, bool persisted)
{
    self->flags ^= ((self->flags ^ (unsigned char)((unsigned char)persisted << 3)) & 0x08);
}

// __fastcall shim: this in ecx, one stack arg (bool)
static void call_setpersist(CThing* self, bool v)
{
    unsigned char arg = v ? 1 : 0;
    __asm {
        movzx eax, arg
        push  eax
        mov   ecx, self
        call  SetAsGameStatePersisted
    }
}

int main()
{
    CThing t;
    t.flags = 0xF7;              // 11110111 : bit3 clear, others set
    call_setpersist(&t, true);
    if (t.flags != 0xFF) { printf("FAIL set-true got %02X\n", t.flags); return 1; }

    call_setpersist(&t, false);
    if (t.flags != 0xF7) { printf("FAIL set-false got %02X\n", t.flags); return 1; }

    t.flags = 0x08;
    call_setpersist(&t, true);
    if (t.flags != 0x08) { printf("FAIL idem-true got %02X\n", t.flags); return 1; }

    t.flags = 0x00;
    call_setpersist(&t, false);
    if (t.flags != 0x00) { printf("FAIL idem-false got %02X\n", t.flags); return 1; }

    t.flags = 0x00;
    call_setpersist(&t, true);
    if (t.flags != 0x08) { printf("FAIL truthy got %02X\n", t.flags); return 1; }

    printf("OK_0x004c7770\n");
    return 0;
}