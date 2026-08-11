#include <cstdio>

struct CThing {
    char pad[0x92];
    unsigned char flags;
    void SetAsGameStatePersisted(bool persisted);
};

void CThing::SetAsGameStatePersisted(bool persisted)
{
    this->flags ^= (unsigned char)((this->flags ^ (persisted << 5)) & 0x20);
}

int main()
{
    CThing t;
    t.flags = 0xFF;
    t.SetAsGameStatePersisted(false);
    if (t.flags != 0xDF) { printf("FAIL a %02X\n", t.flags); return 1; }
    t.SetAsGameStatePersisted(true);
    if (t.flags != 0xFF) { printf("FAIL b %02X\n", t.flags); return 1; }

    t.flags = 0x00;
    t.SetAsGameStatePersisted(true);
    if (t.flags != 0x20) { printf("FAIL c %02X\n", t.flags); return 1; }
    t.SetAsGameStatePersisted(false);
    if (t.flags != 0x00) { printf("FAIL d %02X\n", t.flags); return 1; }

    t.flags = 0x41;
    t.SetAsGameStatePersisted(true);
    if (t.flags != 0x61) { printf("FAIL e %02X\n", t.flags); return 1; }

    printf("PERSIST_OK\n");
    return 0;
}