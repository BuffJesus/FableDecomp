// CGameScriptInterface::MsgIsQuestionAnsweredYesOrNo @ 0x00894300  (111 bytes)
// VC7.1 / MSVC 2003, /O2 /Oy /W3, x86-32.
//
// Retail prologue reads `this` from ecx (__fastcall model), calls the two
// address-distinct GetMaxWorldFrameForMessages entry points at 0x006e7510 and
// 0x006e7530 in that order, then dispatches the bound IsOfType /
// CanBeSeenOrHeard filter through [[this+4]+0x60].  Message type 0x10 denotes
// the question-answer event.  The answer field at result+0x3c is returned
// verbatim only for {0,1,2}; a missing or invalid answer returns -1.
//
// All engine callees are extern (relocation-masked in parity).  The dispatcher
// member call is __fastcall (this in ecx, one stack arg); VC7.1 rejects the
// __fastcall keyword in a typedef, so it is modeled as a default (__fastcall)
// non-static member function of a placeholder class.
//
// PARITY NOTE: the only remaining `<>` region was the store-scheduling order of
// two INDEPENDENT stack writes at the block base -- retail emits
//   mov [block+4],edi   (r1 = Gsi_A result)   BEFORE
//   mov [block+0],0x10  (len)
// while the earlier source assigned `a.len` first, so VC7.1 scheduled the len
// store first.  Assigning `a.r1` BEFORE `a.len` restores retail's emission
// order; all other stores already matched.

// --- inlined local layouts -------------------------------------------------

// The argument block the engine builds on the stack for the query dispatcher.
// Field offsets mirror the retail stack writes exactly:
//   +0x00 len   = 0x10        (mov [esp+0xc],0x10)
//   +0x04 r1    = Gsi_A(this) (mov [esp+0x10],edi)
//   +0x08 r2    = Gsi_B(this) (mov [esp+0x10]->eax earlier: [esp+0x10],eax)
//   +0x0C self  = &block      (mov [esp+0x14],edx = lea [esp+0x8])
//   +0x10 arg   = &r1         (mov [esp+0x1c],eax = lea [esp+0xc])
struct GsiArgs {
    int   len;      // +0x00
    void* r1;       // +0x04
    void* r2;       // +0x08
    void* selfPtr;  // +0x0C
    void* argPtr;   // +0x10
};

// The query result object; answer verb lives at +0x3c (mov eax,[eax+0x3c]).
struct GsiResult { char pad[0x3c]; int answer; };

// --- engine externs (addresses masked by parity) --------------------------

// Code-generation aliases for the two address-distinct
// GetMaxWorldFrameForMessages entry points.  Both are this-only calls:
// __fastcall single-arg => ecx only, edx untouched (matches retail).
extern "C" void* __fastcall Gsi_A(void* thisptr);   // call 0x...53210
extern "C" void* __fastcall Gsi_B(void* thisptr);   // call 0x...53230

// The dispatcher object at [[this+4]+0x60]; its member is __fastcall by default
// (this in ecx, argBlock pushed) -> matches retail `push edx; call rel32`.
struct GsiDispatcher {
    GsiResult* Query(void* argBlock);   // call rel32 @ 0x40
};

// --- the reconstructed function -------------------------------------------
//
// long __fastcall (this in ecx).  Returns the answer verb (0/1/2) or -1.

extern "C" long __fastcall
CGameScriptInterface_MsgIsQuestionAnsweredYesOrNo(void* self, void* /*edx*/)
{
    void* r1 = Gsi_A(self);   // edi = A(this)
    void* r2 = Gsi_B(self);   // eax = B(this)

    // ecx = *(*(self+4) + 0x60)
    GsiDispatcher* dispatch =
        *(GsiDispatcher**)((char*)(*(void**)((char*)self + 4)) + 0x60);

    GsiArgs a;
    a.r1      = r1;           // mov [block+4],edi  -- emitted FIRST (retail order)
    a.len     = 0x10;         // mov [block+0],0x10
    a.r2      = r2;
    a.selfPtr = &a;
    a.argPtr  = &a.r1;

    GsiResult* res = dispatch->Query(&a.selfPtr);
    if (res != 0) {
        int v = res->answer;              // mov eax,[eax+0x3c]
        if (v == 1) return 1;             // cmp eax,1; je -> ret 1
        if (v == 0) return 0;             // test eax,eax; je -> ret 0
        if (v == 2) return 2;             // cmp eax,2; je -> ret 2
    }
    return -1;                            // or eax,-1; ret  (shared miss exit)
}
