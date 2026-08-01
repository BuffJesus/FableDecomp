#include <stdio.h>
#include <string.h>

// ---- Mock list node/container: circular doubly-linked, value at +0x8 ----
struct OpinionNode
{
    OpinionNode* next;      // +0x0
    OpinionNode* prev;      // +0x4
    unsigned char value[0x18]; // +0x8 .. +0x20 (0x18 bytes transferred per node)
};

struct OpinionList
{
    OpinionNode* head;      // sentinel; [list] = head, [head] = first
};

// ---- Mock stream with a vtable whose slot [+0x10] (index 4) is the writer ----
//
// The naked target dispatches via `call dword ptr [eax+0x10]` after setting
// ecx=stream and pushing (src, 0x18).  That is a __fastcall.  VC7.1 rejects the
// __fastcall keyword on a free function pointer, so we model the same register
// layout with __fastcall + an explicit (ignored) edx slot:
//   ecx = self, edx = <unused>, [esp] = src, [esp+4] = len.
struct StreamVtbl
{
    void* slot0;
    void* slot1;
    void* slot2;
    void* slot3;
    void  (__fastcall* transferBytes)(void* self, void* edx, void* src, int len); // +0x10
};

struct MockStream
{
    StreamVtbl* vptr;       // +0x0
};

namespace
{
    const int      MAX_REC = 8;
    void*          g_recSrc[MAX_REC];
    int            g_recLen[MAX_REC];
    int            g_recCount;
    unsigned int   g_lastCount;
    void*          g_sizeThis;
    void*          g_putThis;

    void Reset()
    {
        memset(g_recSrc, 0, sizeof(g_recSrc));
        memset(g_recLen, 0, sizeof(g_recLen));
        g_recCount = 0;
        g_lastCount = 0;
        g_sizeThis = 0;
        g_putThis = 0;
    }

    unsigned int g_fakeSize;

    void __fastcall TransferBytesImpl(void* self, void* edx, void* src, int len)
    {
        (void)self;
        (void)edx;
        if (g_recCount < MAX_REC)
        {
            g_recSrc[g_recCount] = src;
            g_recLen[g_recCount] = len;
        }
        ++g_recCount;
    }
}

// ---- Stubs for the two relocation-masked rel32 callees ----
extern "C" unsigned int __fastcall
CPersistTraits_OpinionList_Size_0047931e(void* list_ecx)
{
    g_sizeThis = list_ecx;
    return g_fakeSize;
}

// PutCount: source.obj references this as fastcall @8 (void*, u32), so the C
// signature MUST stay (void* stream_ecx, unsigned int count) for name matching.
// BUT the retail/naked call site is really __fastcall: `push eax` puts count on
// the STACK (not edx) with this=ecx, and the outer function's `ret 8` epilogue
// only balances if this callee pops that pushed dword.  A plain fastcall(void*,
// u32) body would `ret 0` and leak 4 bytes -> outer stack skew -> crash.  So we
// author a naked body that reads count from [esp+4] and returns with `ret 4`.
//   stack on entry:  [esp] = return addr, [esp+4] = pushed count (eax)
//   ecx = stream ("this")
extern "C" __declspec(naked) void __fastcall
CPersistTraits_OpinionStream_PutCount_0047931e(void* stream_ecx, unsigned int count)
{
    (void)stream_ecx; (void)count;
    __asm
    {
        mov  eax, dword ptr [esp+4]   // pushed count
        mov  g_lastCount, eax
        mov  g_putThis, ecx           // this = stream
        ret  4                        // balance the caller's push eax
    }
}

extern "C" void __fastcall
CPersistTraits_OpinionList_TransferBinaryOut_0047931e(
    void* self_ecx, void* edx, void* stream, void* list);

int main()
{
    StreamVtbl vt;
    memset(&vt, 0, sizeof(vt));
    vt.transferBytes = &TransferBytesImpl;
    MockStream stream;
    stream.vptr = &vt;

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