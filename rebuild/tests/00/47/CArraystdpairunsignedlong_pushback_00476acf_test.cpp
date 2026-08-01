#include <cstdio>

// Behavior test for CArray<pair<unsigned_long,...> >::push_back @ 0x00476acf.
// Element stride is 0x4c (76 bytes).
//
// The two masked callees are member-style helpers: the container `this` stays in
// ECX and every argument is PUSHED on the stack (the retail asm pushes 2 dwords
// before HelperA and 3 before HelperB, exactly like the landed _Fill_n_0047b6c0
// idiom). The source declares them `extern "C" __fastcall Helper@8/@12` purely so
// the relocation-masked call bytes match retail -- the decorated names @8/@12 are
// frozen. A plain __fastcall(void*,void*) stub would take its args from ECX/EDX
// and clean 0 stack bytes, so it would both read garbage AND leave the pushed
// dwords on the stack (imbalance -> crash on return). We therefore define the
// stubs as NAKED functions that keep the frozen @8/@12 names, read the arguments
// from the stack where the asm actually put them, and `ret 8`/`ret 12` to clean
// up -- mirroring tests\00\47\..._Filln..._0047b6c0_test.cpp.

static int          g_AHits;
static int          g_BHits;
static void*        g_ASlot;
static void*        g_ALast;
static void*        g_BLast;
static void*        g_BDelta;
static void*        g_BValue;

// HelperA: retail pushes  push last(ebx); push slot(esi); call.
//   stack @entry: [esp+4]=slot, [esp+8]=last. ECX=container. cleans 8.
extern "C" __declspec(naked) void __fastcall
CArray_pair_ul_push_back_00476acf_HelperA(void* /*slot*/, void* /*last*/)
{
    __asm
    {
        mov  eax, dword ptr [esp + 4]      ; slot
        mov  g_ASlot, eax
        mov  eax, dword ptr [esp + 8]      ; last
        mov  g_ALast, eax
        inc  dword ptr g_AHits
        ret  8
    }
}

// HelperB: retail pushes  push value; push delta(esi); push last(ebx); call.
//   stack @entry: [esp+4]=last, [esp+8]=delta, [esp+0xC]=value. ECX=container. cleans 12.
extern "C" __declspec(naked) void __fastcall
CArray_pair_ul_push_back_00476acf_HelperB(void* /*last*/, void* /*delta*/, void* /*value*/)
{
    __asm
    {
        mov  eax, dword ptr [esp + 4]      ; last
        mov  g_BLast, eax
        mov  eax, dword ptr [esp + 8]      ; delta
        mov  g_BDelta, eax
        mov  eax, dword ptr [esp + 0Ch]    ; value
        mov  g_BValue, eax
        inc  dword ptr g_BHits
        ret  0Ch
    }
}

extern "C" void __fastcall
CArray_pair_ul_push_back_00476acf(void* thisptr, void* edx, void* arg0, void* arg1);

struct TestArray_00476acf
{
    unsigned char* first;   // +0x00 _Myfirst
    unsigned char* last;    // +0x04 _Mylast
};

static void Reset()
{
    g_AHits = 0; g_BHits = 0;
    g_ASlot = 0; g_ALast = 0;
    g_BLast = 0; g_BDelta = 0; g_BValue = 0;
}

int main()
{
    const unsigned long STRIDE = 0x4cUL;   // 76
    static unsigned char storage[STRIDE * 8];

    TestArray_00476acf arr;
    arr.first = storage;
    arr.last  = storage + STRIDE * 3;   // size == 3

    unsigned char value[STRIDE];
    int failures = 0;

    // Retail dispatch (jae taken == index >= size):
    //   index <  size -> HelperA(first + index*0x4c, _Mylast)   (fall-through)
    //   index >= size -> HelperB(_Mylast, index - size, value)  (jae grow block)

    // Path A: index < size. index=1, size=3 -> slot = first + 1*0x4c, last = _Mylast.
    Reset();
    CArray_pair_ul_push_back_00476acf(&arr, 0, (void*)(unsigned)1, value);
    if (g_AHits != 1 || g_BHits != 0 ||
        g_ASlot != (void*)(storage + STRIDE * 1) ||
        g_ALast != (void*)(storage + STRIDE * 3))
    {
        ++failures;
    }

    // Path B: index > size. index=5, size=3 -> delta = 5 - 3 = 2, value passed through.
    Reset();
    CArray_pair_ul_push_back_00476acf(&arr, 0, (void*)(unsigned)5, value);
    if (g_BHits != 1 || g_AHits != 0 ||
        g_BLast  != (void*)(storage + STRIDE * 3) ||
        g_BDelta != (void*)(unsigned)(5 - 3) ||
        g_BValue != value)
    {
        ++failures;
    }

    // Boundary: index == size -> jae taken -> HelperB with delta 0.
    Reset();
    CArray_pair_ul_push_back_00476acf(&arr, 0, (void*)(unsigned)3, value);
    if (g_BHits != 1 || g_AHits != 0 ||
        g_BLast  != (void*)(storage + STRIDE * 3) ||
        g_BDelta != (void*)(unsigned)0 ||
        g_BValue != value)
    {
        ++failures;
    }

    if (failures != 0)
    {
        std::printf("CARRAY_PAIR_UL_PUSH_BACK_00476ACF_TEST FAIL count=%d a=%d b=%d\n",
                    failures, g_AHits, g_BHits);
        return 1;
    }

    std::printf("CARRAY_PAIR_UL_PUSH_BACK_00476ACF_TEST PASS\n");
    return 0;
}