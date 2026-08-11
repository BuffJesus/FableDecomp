#include <cstdio>

struct PersistWriter
{
    void* last;
    void Write(void* p);
};

struct CPersistTraits_long
{
    unsigned int flags;   // +0
    int pad;              // +4
    PersistWriter writer; // +8
    void TransferOut(void* arg);
};

// local stand-in for the external 0xc1bb60 helper
void PersistWriter::Write(void* p) { last = p; }

void CPersistTraits_long::TransferOut(void* arg)
{
    writer.Write(arg);
    flags |= 0x100;
}

int main()
{
    CPersistTraits_long t;
    t.flags = 0x2A;
    t.pad = 0;
    t.writer.last = 0;
    int payload = 7;
    t.TransferOut(&payload);

    bool ok = (t.writer.last == &payload) && (t.flags == (0x2Au | 0x100u));
    if (ok) printf("TRANSFEROUT_OK\n");
    else    printf("FAIL flags=%x last=%p\n", t.flags, t.writer.last);
    return ok ? 0 : 1;
}