#include <stdio.h>
#include <string.h>

// ---- Mock circular doubly-linked list node/container: value at +0x8 ----
struct OpinionNode
{
    OpinionNode* next;          // +0x00
    OpinionNode* prev;          // +0x04
    unsigned char value[0x18];  // +0x08
};

struct OpinionList
{
    OpinionNode* head;          // +0x00  sentinel pointer
};

// Recording state for verification.
namespace
{
    const int    MAX_REC = 8;
    const void*  g_recSrc[MAX_REC];
    int          g_recLen[MAX_REC];
    int          g_recCount;
    unsigned int g_lastCount;
    void*        g_sizeThis;
    void*        g_putThis;
    unsigned int g_fakeSize;

    void Reset()
    {
        memset(g_recSrc, 0, sizeof(g_recSrc));
        memset(g_recLen, 0, sizeof(g_recLen));
        g_recCount = 0;
        g_lastCount = 0;
        g_sizeThis = 0;
        g_putThis = 0;
    }
}

// ---- Stream must match the source's layout and NAME exactly: 5 virtuals
//      (TransferBytes = slot 4), plus a non-virtual PutCount.  source.obj leaves
//      OpinionStream::PutCount (?PutCount@OpinionStream@@QAEXI@Z) undefined and
//      mangles the target's OpinionStream* parameter as PAUOpinionStream@@; giving
//      this struct the identical name + members makes both the callee definition
//      and the target's mangled reference resolve on the fallback link. ----
struct OpinionStream
{
    virtual void v0() {}
    virtual void v1() {}
    virtual void v2() {}
    virtual void v3() {}
    virtual void TransferBytes(const void* src, int len)
    {
        if (g_recCount < MAX_REC)
        {
            g_recSrc[g_recCount] = src;
            g_recLen[g_recCount] = len;
        }
        ++g_recCount;
    }
    // Non-virtual member the source calls directly (reloc-masked rel32 call).
    // Declared here, DEFINED out-of-line below so it is always emitted into
    // test.obj (an inline body would be discarded because the test never calls it).
    void PutCount(unsigned int count);
};

// Out-of-line so this definition satisfies source.obj's undefined
// ?PutCount@OpinionStream@@QAEXI@Z even though the test never calls PutCount itself.
void OpinionStream::PutCount(unsigned int count)
{
    g_lastCount = count;
    g_putThis = this;
}

// ---- Reloc-masked helper stub (matches the source's extern "C" __fastcall decl:
//      undefined symbol @OpinionList_Size_0047931e@4) ----
extern "C" unsigned int __fastcall OpinionList_Size_0047931e(OpinionList* list)
{
    g_sizeThis = list;
    return g_fakeSize;
}

// The function under test is DEFINED in source_cpp, declared there WITHOUT
// extern "C", so it mangles as
//   ?CPersistTraits_OpinionList_TransferBinaryOut_0047931e@@YIXPAX0PAUOpinionStream@@PAUOpinionList@@@Z
// Declare it here with the IDENTICAL C++ signature (plain __fastcall, same struct
// names) so this reference produces that same mangled symbol and resolves to the
// source.obj definition on the fallback link.  (No extern "C" here -- that was the
// LINK_FAIL: it produced @...@16 which source.obj never defines.)
void __fastcall
CPersistTraits_OpinionList_TransferBinaryOut_0047931e(
    void* self_ecx, void* edx, OpinionStream* stream, OpinionList* list);

int main()
{
    OpinionStream stream;

    // Populated list: sentinel + two value nodes, circular.
    OpinionNode sentinel, a, b;
    memset(&sentinel, 0, sizeof(sentinel));
    memset(&a, 0, sizeof(a));
    memset(&b, 0, sizeof(b));
    a.value[0] = 0xAA;
    b.value[0] = 0xBB;
    sentinel.next = &a;   sentinel.prev = &b;
    a.next = &b;          a.prev = &sentinel;
    b.next = &sentinel;   b.prev = &a;
    OpinionList list;
    list.head = &sentinel;

    Reset();
    g_fakeSize = 2;
    CPersistTraits_OpinionList_TransferBinaryOut_0047931e(
        (void*)0xF00D, (void*)0, &stream, &list);

    bool okPopulated =
        g_sizeThis == &list &&
        g_putThis == &stream &&
        g_lastCount == 2 &&
        g_recCount == 2 &&
        g_recSrc[0] == a.value && g_recLen[0] == 0x18 &&
        g_recSrc[1] == b.value && g_recLen[1] == 0x18;

    // Empty list: sentinel points to itself.
    OpinionNode emptySentinel;
    memset(&emptySentinel, 0, sizeof(emptySentinel));
    emptySentinel.next = &emptySentinel;
    emptySentinel.prev = &emptySentinel;
    OpinionList emptyList;
    emptyList.head = &emptySentinel;

    Reset();
    g_fakeSize = 0;
    CPersistTraits_OpinionList_TransferBinaryOut_0047931e(
        (void*)0xF00D, (void*)0, &stream, &emptyList);

    bool okEmpty =
        g_sizeThis == &emptyList &&
        g_putThis == &stream &&
        g_lastCount == 0 &&
        g_recCount == 0;

    if (!okPopulated || !okEmpty)
    {
        printf("CPERSISTTRAITS_OPINION_TRANSFER_OUT_TEST FAIL\n");
        return 1;
    }

    printf("CPERSISTTRAITS_OPINION_TRANSFER_OUT_TEST PASS\n");
    return 0;
}