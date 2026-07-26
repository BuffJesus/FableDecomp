#include <cstdio>

struct Vec3 { float x, y, z; };
struct CObj34 { char pad[0x44]; float f44; };
struct CTCPerceiveThing {
    char pad0[4];
    int field4;
    char pad8[4];
    char field_c[0xc];
    float f18;
    char pad1c[4];
    Vec3 v20;
    char pad2c[8];
    CObj34* p34;
};

static int g_a_called = 0, g_a_arg = 0;
static int g_b_called = 0, g_b_arg = 0;

struct GObj {
    void NotifyA(int arg) { g_a_called++; g_a_arg = arg; }
    void NotifyB(int arg) { g_b_called++; g_b_arg = arg; }
};

static Vec3 g_src = { 1.5f, 2.5f, 3.5f };
static void* g_sub = (void*)1;
float g_const_122dedc = 7.0f;
GObj g_obj_inst;
GObj* g_obj_13b8790 = &g_obj_inst;

extern "C" void* __fastcall Getter_2bd420(void*) { return g_sub; }
extern "C" Vec3* __fastcall Getter_d82ca0(void*) { return &g_src; }

void __fastcall CTCPerceiveThing_SetLastAwareInfo(CTCPerceiveThing* self)
{
    void* sub = Getter_2bd420(self->field_c);
    Vec3* v = Getter_d82ca0(sub);
    self->v20 = *v;

    float val = self->p34->f44;
    self->f18 = val;

    if (val != g_const_122dedc) {
        if (Getter_2bd420(self->field_c)) {
            g_obj_13b8790->NotifyA(self->field4);
        } else {
            g_obj_13b8790->NotifyB(self->field4);
        }
    }
}

int main() {
    CObj34 o34; o34.f44 = 42.0f;
    CTCPerceiveThing t;
    t.field4 = 99;
    t.p34 = &o34;

    // val == const -> no call, but vec+f18 still copied
    o34.f44 = 7.0f;   // equals g_const
    g_a_called = g_b_called = 0;
    CTCPerceiveThing_SetLastAwareInfo(&t);
    if (!(t.v20.x==1.5f && t.v20.y==2.5f && t.v20.z==3.5f)) { std::printf("FAIL vec\n"); return 1; }
    if (t.f18 != 7.0f) { std::printf("FAIL f18\n"); return 1; }
    if (g_a_called != 0 || g_b_called != 0) { std::printf("FAIL branchNone\n"); return 1; }

    // val != const, getter nonzero -> Notify_A
    o34.f44 = 42.0f;
    g_sub = (void*)1;
    g_a_called = g_b_called = 0;
    CTCPerceiveThing_SetLastAwareInfo(&t);
    if (g_a_called != 1 || g_a_arg != 99 || g_b_called != 0) { std::printf("FAIL branchA\n"); return 1; }

    // val != const, getter zero -> Notify_B
    g_sub = 0;
    g_a_called = g_b_called = 0;
    CTCPerceiveThing_SetLastAwareInfo(&t);
    if (g_b_called != 1 || g_b_arg != 99 || g_a_called != 0) { std::printf("FAIL branchB\n"); return 1; }

    std::printf("CTCPerceiveThing_00744730_TEST PASS\n");
    return 0;
}