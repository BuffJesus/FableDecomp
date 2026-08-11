#include <cstdio>

struct CThing {
    char pad[0x15d];
    unsigned char flags;
};

void __fastcall CThing_SetAsGameStatePersisted(CThing* self, int edx, bool persisted);

int main()
{
    CThing t;
    int fails = 0;

    // start with all bits set except we track bit1
    t.flags = 0xFD; // bit1 clear, others set
    CThing_SetAsGameStatePersisted(&t, 0, true);
    if (t.flags != 0xFF) { printf("F1 %02x\n", t.flags); fails++; }

    CThing_SetAsGameStatePersisted(&t, 0, false);
    if (t.flags != 0xFD) { printf("F2 %02x\n", t.flags); fails++; }

    t.flags = 0x00;
    CThing_SetAsGameStatePersisted(&t, 0, true);
    if (t.flags != 0x02) { printf("F3 %02x\n", t.flags); fails++; }

    CThing_SetAsGameStatePersisted(&t, 0, true); // idempotent
    if (t.flags != 0x02) { printf("F4 %02x\n", t.flags); fails++; }

    CThing_SetAsGameStatePersisted(&t, 0, false);
    if (t.flags != 0x00) { printf("F5 %02x\n", t.flags); fails++; }

    // other bits preserved
    t.flags = 0xA1;
    CThing_SetAsGameStatePersisted(&t, 0, true);
    if (t.flags != 0xA3) { printf("F6 %02x\n", t.flags); fails++; }
    CThing_SetAsGameStatePersisted(&t, 0, false);
    if (t.flags != 0xA1) { printf("F7 %02x\n", t.flags); fails++; }

    if (fails == 0) printf("PERSIST_OK\n");
    return fails ? 1 : 0;
}