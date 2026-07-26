// Standalone behavioral test for CGameScriptInterface::MsgIsQuestionAnsweredYesOrNo @ 0x00894300
// VC7.1 model: member method = free __fastcall (this in ecx, unused edx, then stack args).
// The harness links ONLY this object, so this file DEFINES its own copy of the
// function plus recording stubs for all externs.

#include <cstdio>

// --- struct layouts (must match the source under test) ---------------------

struct GsiArgs {
    int   len;      // +0x00
    void* r1;       // +0x04
    void* r2;       // +0x08
    void* selfPtr;  // +0x0C
    void* argPtr;   // +0x10
};

struct GsiResult { char pad[0x3c]; int answer; };

// --- recording stubs for the engine externs --------------------------------

static void* g_self_seen_A = 0;
static void* g_self_seen_B = 0;
static int   g_A_calls = 0;
static int   g_B_calls = 0;

static void* g_ret_r1 = (void*)0xAAAA0001;
static void* g_ret_r2 = (void*)0xBBBB0002;

extern "C" void* __fastcall Gsi_A(void* thisptr) { g_A_calls++; g_self_seen_A = thisptr; return g_ret_r1; }
extern "C" void* __fastcall Gsi_B(void* thisptr) { g_B_calls++; g_self_seen_B = thisptr; return g_ret_r2; }

// Dispatcher: captures the arg block it was handed and returns a configurable result.
static GsiResult* g_query_result = 0;
static void*      g_query_argblock = 0;
static void*      g_query_this = 0;
static int        g_query_calls = 0;

struct GsiDispatcher {
    GsiResult* Query(void* argBlock);
};

// Captured (while the caller's stack frame is still live) copies of the block.
static int   g_cap_len = -1;
static void* g_cap_r1 = 0;
static void* g_cap_r2 = 0;
static bool  g_cap_selfPtr_ok = false;
static bool  g_cap_argPtr_ok = false;

GsiResult* GsiDispatcher::Query(void* argBlock)
{
    g_query_calls++;
    g_query_this = this;
    g_query_argblock = argBlock;

    // argBlock == &a.selfPtr; selfPtr == &a (block base). Capture now.
    void** pSelf = (void**)argBlock;   // -> a.selfPtr
    GsiArgs* blk = (GsiArgs*)pSelf[0]; // selfPtr == &a
    g_cap_len = blk->len;
    g_cap_r1  = blk->r1;
    g_cap_r2  = blk->r2;
    g_cap_selfPtr_ok = (blk->selfPtr == blk);
    g_cap_argPtr_ok  = (blk->argPtr  == &blk->r1);

    return g_query_result;
}

// --- the reconstructed function (copy of source under test) ----------------

extern "C" long __fastcall
CGameScriptInterface_MsgIsQuestionAnsweredYesOrNo(void* self, void* /*edx*/)
{
    void* r1 = Gsi_A(self);
    void* r2 = Gsi_B(self);

    GsiDispatcher* dispatch =
        *(GsiDispatcher**)((char*)(*(void**)((char*)self + 4)) + 0x60);

    GsiArgs a;
    a.r1      = r1;
    a.len     = 0x10;
    a.r2      = r2;
    a.selfPtr = &a;
    a.argPtr  = &a.r1;

    GsiResult* res = dispatch->Query(&a.selfPtr);
    if (res != 0) {
        int v = res->answer;
        if (v == 1) return 1;
        if (v == 0) return 0;
        if (v == 2) return 2;
    }
    return -1;
}

// --- harness ---------------------------------------------------------------

// Build a fake `this` whose [this+4] -> obj, obj[+0x60] -> dispatcher.
// The function under test reads pointers at raw byte offsets (self+4, inner+0x60),
// so we back both objects with char buffers and poke pointers at those exact
// offsets (works for both 32- and 64-bit pointer widths).
static GsiDispatcher g_dispatcher;

static char g_inner[0x60 + sizeof(void*) + 16];
static char g_self[4 + sizeof(void*) + 16];

static long run_case(int answerVerb, bool nullResult, void** outArg)
{
    // reset
    g_A_calls = g_B_calls = g_query_calls = 0;
    g_self_seen_A = g_self_seen_B = 0;
    g_query_argblock = 0;

    static GsiResult res;
    res.answer = answerVerb;
    g_query_result = nullResult ? 0 : &res;

    // inner object: at +0x60 holds pointer to dispatcher
    *(GsiDispatcher**)(g_inner + 0x60) = &g_dispatcher;

    // self: at +4 holds pointer to inner
    *(void**)(g_self + 4) = g_inner;

    long r = CGameScriptInterface_MsgIsQuestionAnsweredYesOrNo(g_self, 0);
    if (outArg) *outArg = g_query_argblock;
    return r;
}

int main()
{
    int fail = 0;

    // Verb 0 -> returns 0
    if (run_case(0, false, 0) != 0) { printf("FAIL verb0\n"); fail++; }
    // Verb 1 -> returns 1
    if (run_case(1, false, 0) != 1) { printf("FAIL verb1\n"); fail++; }
    // Verb 2 -> returns 2
    if (run_case(2, false, 0) != 2) { printf("FAIL verb2\n"); fail++; }
    // Other verb -> -1
    if (run_case(7, false, 0) != -1) { printf("FAIL verb7\n"); fail++; }
    // Null result -> -1
    if (run_case(1, true, 0) != -1) { printf("FAIL nullres\n"); fail++; }

    // Accessors and dispatcher must have been invoked once each.
    void* argp = 0;
    run_case(1, false, &argp);
    if (g_A_calls != 1) { printf("FAIL A calls=%d\n", g_A_calls); fail++; }
    if (g_B_calls != 1) { printf("FAIL B calls=%d\n", g_B_calls); fail++; }
    if (g_query_calls != 1) { printf("FAIL query calls=%d\n", g_query_calls); fail++; }
    if (g_query_this != &g_dispatcher) { printf("FAIL query this\n"); fail++; }

    (void)argp;
    // Block contents were captured inside Query while the frame was live.
    if (g_cap_len != 0x10)     { printf("FAIL len\n"); fail++; }
    if (g_cap_r1  != g_ret_r1) { printf("FAIL r1\n"); fail++; }
    if (g_cap_r2  != g_ret_r2) { printf("FAIL r2\n"); fail++; }
    if (!g_cap_selfPtr_ok)     { printf("FAIL selfPtr\n"); fail++; }
    if (!g_cap_argPtr_ok)      { printf("FAIL argPtr\n"); fail++; }

    if (fail == 0) {
        printf("FSE2_00894300_TEST PASS\n");
        return 0;
    }
    return 1;
}