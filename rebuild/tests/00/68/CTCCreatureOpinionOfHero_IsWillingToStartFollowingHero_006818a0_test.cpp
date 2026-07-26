#include <cstdio>

// Standalone behavioural test for
// CTCCreatureOpinionOfHero::IsWillingToStartFollowingHero @ 0x006818a0.
//
// We rebuild the exact naked function, provide the jump/index tables it needs,
// and drive its dispatch by controlling the state value returned by the stub
// state getter. The index/jump tables are wired so cases {1,2,4,5,7,8,10,11,
// 13,14,15} return true and {3,6,9,12} (and out-of-range) return false.

struct Manager { char pad[0x74]; void* dummy; };
static Manager g_mgr;
static Manager* g_mgrPtr;

static int g_state = 0;

extern "C" void* CTCCreatureOpinionOfHero_GetManagerOwner(void)
{
    // Return an object whose +0x74 field points at the manager the getter
    // is invoked on. We reuse g_mgr; its +0x74 slot points to itself.
    g_mgr.dummy = &g_mgr;
    g_mgrPtr = &g_mgr;
    return &g_mgr;
}

// Modelled __fastcall (this in ecx, one stack arg, callee-cleans). We use
// __stdcall so the callee pops the pushed argument, keeping the stack balanced.
int __stdcall CTCCreatureOpinionOfHero_GetCreatureState(void* /*arg*/)
{
    return g_state;
}

// index table: state-1 -> jump slot (0 = true, 1 = false)
unsigned char kIsWillingCaseIndex[15] = {
    /*st=1*/0, /*2*/0, /*3*/1, /*4*/0, /*5*/0,
    /*6*/1, /*7*/0, /*8*/0, /*9*/1, /*10*/0,
    /*11*/0, /*12*/1, /*13*/0, /*14*/0, /*15*/0
};
void* kIsWillingJumpTable[2];  // filled at runtime: [0]=true path, [1]=false path

// Standalone landing pads the jump table targets. They mirror the naked
// function's internal ltrue/lfalse blocks (mov al,1;ret / xor al,al;ret) and
// return directly to IsWilling's caller.
__declspec(naked) static void padTrue()
{
    __asm {
        mov al, 1
        ret
    }
}
__declspec(naked) static void padFalse()
{
    __asm {
        xor al, al
        ret
    }
}

__declspec(naked) bool __fastcall
CTCCreatureOpinionOfHero_IsWillingToStartFollowingHero(void* self)
{
    __asm {
        push esi
        mov  esi, dword ptr [ecx+4]
        call CTCCreatureOpinionOfHero_GetManagerOwner
        mov  eax, dword ptr [eax+0x74]
        push esi
        mov  ecx, eax
        call CTCCreatureOpinionOfHero_GetCreatureState
        dec  eax
        cmp  eax, 0x0e
        pop  esi
        ja   short lfalse
        movzx eax, byte ptr [eax + kIsWillingCaseIndex]
        jmp  dword ptr [eax*4 + kIsWillingJumpTable]
    ltrue:
        mov  al, 1
        ret
    lfalse:
        xor  al, al
        ret
    }
}

static void init_tables()
{
    kIsWillingJumpTable[0] = (void*)&padTrue;
    kIsWillingJumpTable[1] = (void*)&padFalse;
}

int main()
{
    init_tables();

    void* dummyThis;
    unsigned char thisbuf[16];
    // [this+4] = arg (esi); value is irrelevant to the boolean result.
    *(void**)(thisbuf + 4) = 0;
    dummyThis = thisbuf;

    bool ok = true;
    int trueCases[]  = {1,2,4,5,7,8,10,11,13,14,15};
    for (int i = 0; i < 11; ++i) {
        g_state = trueCases[i];
        bool r = CTCCreatureOpinionOfHero_IsWillingToStartFollowingHero(dummyThis);
        if (!r) { ok = false; std::printf("case %d expected true\n", trueCases[i]); }
    }
    int falseCases[] = {0,3,6,9,12,16,99,-1};
    for (int i = 0; i < 8; ++i) {
        g_state = falseCases[i];
        bool r = CTCCreatureOpinionOfHero_IsWillingToStartFollowingHero(dummyThis);
        if (r) { ok = false; std::printf("case %d expected false\n", falseCases[i]); }
    }

    if (ok) {
        std::printf("CTCCreatureOpinionOfHero_006818a0_TEST PASS\n");
        return 0;
    }
    std::printf("CTCCreatureOpinionOfHero_006818a0_TEST FAIL\n");
    return 1;
}