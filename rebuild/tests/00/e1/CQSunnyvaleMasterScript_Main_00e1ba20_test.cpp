#include <cstdio>

struct IStep {
    struct VT { void* pad[7]; void (__fastcall *slot1c)(IStep*); };
    VT* vtbl;
};
struct CQ_SunnyvaleMasterScript {
    void* field0;
    IStep* field4;
};

static int g_calls = 0;
static int g_advance_left = 3;

void __fastcall CQ_SunnyvaleMasterScript_Advance_impl(CQ_SunnyvaleMasterScript*);
bool __fastcall CQ_SunnyvaleMasterScript_Advance(CQ_SunnyvaleMasterScript* self);

static void __fastcall step_fn(IStep*) { g_calls++; }

bool __fastcall CQ_SunnyvaleMasterScript_Advance(CQ_SunnyvaleMasterScript*) {
    // return false, false, true  -> loop exits after 3rd body run
    return --g_advance_left <= 0;
}

void __fastcall Main(CQ_SunnyvaleMasterScript* self);

int main() {
    IStep::VT vt;
    vt.slot1c = step_fn;
    IStep step;
    step.vtbl = &vt;
    CQ_SunnyvaleMasterScript o;
    o.field0 = 0;
    o.field4 = &step;
    Main(&o);
    // Advance returns false twice then true -> body runs 3 times
    if (g_calls == 3) printf("00e1ba20_TEST PASS\n");
    else printf("FAIL calls=%d\n", g_calls);
    return 0;
}