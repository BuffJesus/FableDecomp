#include <cstdio>

static int g_ensure_calls = 0;

struct CharBuf {
    unsigned char pad[0xc];
    unsigned char flags;
};

struct CCharString {
    CharBuf* buf;
};

static void EnsureUnique(CCharString*) { g_ensure_calls++; }

static void SetUseFastExtend(CCharString* self, bool value)
{
    EnsureUnique(self);
    CharBuf* b = self->buf;
    unsigned char cur = b->flags;
    unsigned char x = (unsigned char)(cur ^ (unsigned char)value);
    x &= 1;
    b->flags = (unsigned char)(b->flags ^ x);
}

int main()
{
    CharBuf b;
    CCharString s; s.buf = &b;

    bool ok = true;

    // Start with flags = 0xF0 (bit0 clear), set true -> bit0 set, rest kept.
    b.flags = 0xF0;
    SetUseFastExtend(&s, true);
    if (b.flags != 0xF1) ok = false;

    // set true again -> idempotent
    SetUseFastExtend(&s, true);
    if (b.flags != 0xF1) ok = false;

    // set false -> bit0 clear, rest kept
    b.flags = 0xF1;
    SetUseFastExtend(&s, false);
    if (b.flags != 0xF0) ok = false;

    // set false when already clear -> unchanged
    b.flags = 0xF0;
    SetUseFastExtend(&s, false);
    if (b.flags != 0xF0) ok = false;

    // odd byte set true
    b.flags = 0xAA; // bit0 clear
    SetUseFastExtend(&s, true);
    if (b.flags != 0xAB) ok = false;

    if (g_ensure_calls != 5) ok = false;

    if (ok) printf("BEHAVIOUR_PASS_099ef70\n");
    else    printf("BEHAVIOUR_FAIL flags=%02x calls=%d\n", b.flags, g_ensure_calls);
    return 0;
}