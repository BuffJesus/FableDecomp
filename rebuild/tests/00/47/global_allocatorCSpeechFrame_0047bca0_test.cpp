#include <stdio.h>

// ------------------------------------------------------------------------
// Behaviour test for _Destroy_range<CSpeechFrame> (retail 0x0047bca0).
//
// The routine under test lives in source_cpp; per the verifier's link model
// the test.obj-alone link intentionally fails (the target is an external),
// and the lander then links source.obj + test.obj together.  This test:
//   * DECLARES the target with the SAME __fastcall signature source_cpp
//     defines, so the C++ decorated name matches and the fallback link
//     resolves it (it must NOT be redefined here).
//   * DEFINES the single masked callee source_cpp references,
//     CCharString::~CCharString (__fastcall, ecx=this).  It is the ONLY
//     out-of-line symbol the source emits a relocation for, so defining it
//     here (and NOWHERE in source_cpp) means no duplicate symbol and the
//     stack balances (member dtor cleans its own `this`).
// ------------------------------------------------------------------------

// ---- element / member layout mirrors the reconstruction ----
struct CCharString {
    char* _ptr;
    ~CCharString();   // defined out-of-line below (the masked-callee stub)
};

struct CSpeechFrame {
    long         _id;
    CCharString  text;
    long         _data;
};

// ---- masked-callee stub: counts destructions and records the last `this` ----
static int          g_dtorCount = 0;
static CCharString* g_lastThis  = 0;

CCharString::~CCharString()
{
    ++g_dtorCount;
    g_lastThis = this;
}

// Target under test -- declared with source_cpp's exact __fastcall signature,
// NOT redefined (resolved from source.obj by the fallback link).
void __fastcall _Destroy_range_CSpeechFrame(CSpeechFrame* _First, CSpeechFrame* _Last);

int main(void)
{
    // Raw storage so the array elements are NOT auto-destroyed by C++ scope exit
    // (destruction is invoked explicitly through the routine under test).
    static char buf[3 * sizeof(CSpeechFrame)];
    CSpeechFrame* a = (CSpeechFrame*)buf;
    int i;
    for (i = 0; i < 3; ++i) {
        a[i]._id       = i;
        a[i].text._ptr = 0;
        a[i]._data     = i * 10;
    }

    // Empty range: entry guard (_First == _Last) -> nothing destroyed.
    g_dtorCount = 0;
    g_lastThis  = 0;
    _Destroy_range_CSpeechFrame(&a[1], &a[1]);
    int emptyOk = (g_dtorCount == 0 && g_lastThis == 0);

    // Full range: three sub-objects destroyed, last `this` == &a[2].text,
    // which also proves the +4 sub-object offset and 12-byte stride.
    g_dtorCount = 0;
    g_lastThis  = 0;
    _Destroy_range_CSpeechFrame(&a[0], &a[3]);
    CCharString* expLast = &a[2].text;

    if (emptyOk && g_dtorCount == 3 && g_lastThis == expLast)
        printf("DESTROY_RANGE_CSPEECHFRAME_TEST PASS\n");
    else
        printf("FAIL empty=%d count=%d last=%p exp=%p\n",
               emptyOk, g_dtorCount, (void*)g_lastThis, (void*)expLast);
    return 0;
}