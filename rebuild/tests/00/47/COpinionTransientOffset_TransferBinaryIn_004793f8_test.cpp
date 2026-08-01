#include <stdio.h>
#include <string.h>

// Stream layout mirrored from the disasm.
struct Stream {
    long  pad0;      // +0x00
    long  cursor;    // +0x04
    long  pad8;      // +0x08
    void* src;       // +0x0C
    long  pad10;     // +0x10
    long  left;      // +0x14
};

extern "C" void __fastcall
COpinionTransientOffset_TransferBinaryIn_004793f8(void*, void*, void*);

// Slow-path stub so the link resolves; record whether it was hit.
static int g_slow_hit = 0;
extern "C" void __fastcall
OpinionTransferBinaryIn_slowread_0051a8a8(void*, void*, void*, long)
{
    g_slow_hit = 1;
}

int main()
{
    // ---- fast path: window has 0x18 bytes available ----
    unsigned char srcbuf[0x40];
    for (int i = 0; i < 0x40; ++i) srcbuf[i] = (unsigned char)(i + 1);

    Stream s;
    memset(&s, 0, sizeof(s));
    s.cursor = 100;
    s.src    = srcbuf + 8;   // read pointer sits 8 bytes in
    s.left   = 0x40;         // plenty remaining (>= 0x18)

    unsigned char dst[0x18];
    memset(dst, 0xAA, sizeof(dst));

    COpinionTransientOffset_TransferBinaryIn_004793f8(dst, 0, &s);

    if (g_slow_hit) { printf("FAIL: slow path taken on fast case\n"); return 1; }
    if (memcmp(dst, srcbuf + 8, 0x18) != 0) { printf("FAIL: bytes not copied\n"); return 1; }
    if (s.cursor != 100 + 0x18) { printf("FAIL: cursor not advanced (%ld)\n", s.cursor); return 1; }
    if (s.left != 0x40 - 0x18) { printf("FAIL: left not decremented (%ld)\n", s.left); return 1; }
    if (s.src != srcbuf + 8 + 0x18) { printf("FAIL: src not advanced\n"); return 1; }

    // ---- slow path: window too small ----
    Stream s2;
    memset(&s2, 0, sizeof(s2));
    s2.cursor = 0;
    s2.src    = srcbuf;
    s2.left   = 0x10;        // < 0x18 -> slow path
    unsigned char dst2[0x18];
    g_slow_hit = 0;
    COpinionTransientOffset_TransferBinaryIn_004793f8(dst2, 0, &s2);
    if (!g_slow_hit) { printf("FAIL: slow path not taken on small window\n"); return 1; }

    printf("OPINION_TRANSFER_BINARY_IN_TEST PASS\n");
    return 0;
}