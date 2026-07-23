#include <cstdio>
struct CActionInputControl { int x; };
struct Vec { CActionInputControl* begin; CActionInputControl* end; };
struct CUserProfileManager {
    char pad[0x54];
    Vec inputs;
};
static int g_called = 0;
void __fastcall Populate(CUserProfileManager* self) { g_called = 1; }

Vec* __fastcall CUserProfileManager_GetAssignedInputsPassive(CUserProfileManager* self)
{
    Vec* v = &self->inputs;
    if (v->begin == v->end)
        Populate(self);
    return v;
}

int main() {
    CUserProfileManager m;
    CActionInputControl arr[2];
    // non-empty case: begin != end, no populate
    m.inputs.begin = &arr[0];
    m.inputs.end = &arr[2];
    g_called = 0;
    Vec* r = CUserProfileManager_GetAssignedInputsPassive(&m);
    if (r != &m.inputs) { std::printf("FAIL ptr\n"); return 1; }
    if (g_called != 0) { std::printf("FAIL called nonempty\n"); return 1; }
    // empty case: begin == end, populate called
    m.inputs.begin = &arr[0];
    m.inputs.end = &arr[0];
    g_called = 0;
    r = CUserProfileManager_GetAssignedInputsPassive(&m);
    if (r != &m.inputs) { std::printf("FAIL ptr2\n"); return 1; }
    if (g_called != 1) { std::printf("FAIL not called empty\n"); return 1; }
    std::printf("CUserProfileManager_004088e0_TEST PASS\n");
    return 0;
}