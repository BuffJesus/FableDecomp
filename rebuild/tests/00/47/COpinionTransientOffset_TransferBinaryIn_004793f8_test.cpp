#include <stdio.h>
#include <string.h>

// Self-contained /Od behavior harness for COpinionTransientOffset::TransferBinaryIn
// (retail 0x004793f8). Mirrors the reconstruction's stream layout and object,
// then drives the fast path (window has >= 0x18 bytes) and the slow path
// (window too small) and checks the observable effects.

struct CDataInputStream
{
    long           Pad00;                  // +0x00
    long           ReadPosition;           // +0x04
    long           Pad08;                  // +0x08
    unsigned char* Cursor;                 // +0x0C
    long           Pad10;                  // +0x10
    long           RemainingInSourceChunk; // +0x14

    void ReadWithSrcChunkOverflow(unsigned char* destination, long size);
};

struct COpinionTransientOffset
{
    unsigned char Bytes[0x18];

    void TransferBinaryIn(CDataInputStream* stream);
};

// Slow-path stub so the link resolves; record that it was hit + its args.
static int            g_slowHit = 0;
static unsigned char* g_slowDst = 0;
static long           g_slowSize = 0;
void CDataInputStream::ReadWithSrcChunkOverflow(unsigned char* destination, long size)
{
    g_slowHit = 1;
    g_slowDst = destination;
    g_slowSize = size;
}

int main(void)
{
    int ok = 1;

    // ---- fast path: window holds >= 0x18 bytes ----
    unsigned char srcbuf[0x40];
    for (int i = 0; i < 0x40; ++i) srcbuf[i] = (unsigned char)(i + 1);

    CDataInputStream s;
    memset(&s, 0, sizeof(s));
    s.ReadPosition           = 100;
    s.Cursor                 = srcbuf + 8;
    s.RemainingInSourceChunk = 0x40;

    COpinionTransientOffset dst;
    memset(dst.Bytes, 0xAA, sizeof(dst.Bytes));

    g_slowHit = 0;
    dst.TransferBinaryIn(&s);

    if (g_slowHit)                                    { printf("FAIL slow on fast\n"); ok = 0; }
    if (memcmp(dst.Bytes, srcbuf + 8, 0x18) != 0)     { printf("FAIL bytes\n"); ok = 0; }
    if (s.ReadPosition != 100 + 0x18)                 { printf("FAIL pos %ld\n", s.ReadPosition); ok = 0; }
    if (s.RemainingInSourceChunk != 0x40 - 0x18)      { printf("FAIL remain %ld\n", s.RemainingInSourceChunk); ok = 0; }
    if (s.Cursor != srcbuf + 8 + 0x18)                { printf("FAIL cursor\n"); ok = 0; }

    // ---- slow path: window too small (< 0x18), guard still passes ----
    CDataInputStream s2;
    memset(&s2, 0, sizeof(s2));
    s2.ReadPosition           = 0;
    s2.Cursor                 = srcbuf;
    s2.RemainingInSourceChunk = 0x10;   // < 0x18

    COpinionTransientOffset dst2;
    memset(dst2.Bytes, 0xBB, sizeof(dst2.Bytes));

    g_slowHit = 0; g_slowDst = 0; g_slowSize = 0;
    dst2.TransferBinaryIn(&s2);

    if (!g_slowHit)                 { printf("FAIL slow not hit\n"); ok = 0; }
    if (g_slowDst != dst2.Bytes)    { printf("FAIL slow dst\n"); ok = 0; }
    if (g_slowSize != 0x18)         { printf("FAIL slow size %ld\n", g_slowSize); ok = 0; }

    // ---- guard-fail path: position would exceed 0x7FFFFFFF, nothing happens ----
    CDataInputStream s3;
    memset(&s3, 0, sizeof(s3));
    s3.ReadPosition           = 0x7FFFFFF0;  // + 0x18 overflows the <= 0x7FFFFFFF bound
    s3.Cursor                 = srcbuf;
    s3.RemainingInSourceChunk = 0x40;

    COpinionTransientOffset dst3;
    memset(dst3.Bytes, 0xCC, sizeof(dst3.Bytes));

    g_slowHit = 0;
    dst3.TransferBinaryIn(&s3);

    if (g_slowHit)                          { printf("FAIL guard slow\n"); ok = 0; }
    if (s3.ReadPosition != 0x7FFFFFF0)      { printf("FAIL guard pos moved\n"); ok = 0; }
    if (dst3.Bytes[0] != 0xCC)              { printf("FAIL guard dst touched\n"); ok = 0; }

    if (ok) printf("OPINION_TRANSFER_BINARY_IN_004793F8_TEST PASS\n");
    else    printf("OPINION_TRANSFER_BINARY_IN_004793F8_TEST FAIL\n");
    return ok ? 0 : 1;
}