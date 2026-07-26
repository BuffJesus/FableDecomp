#include <cstdio>

struct CTarget { char pad[0x10]; unsigned char kind; };
struct CTCPerceiveThing {
    char pad0[0xc];
    char sub[0x21];
    unsigned char flag;
};

static CTarget* g_ret = 0;
extern "C" CTarget* __fastcall Perceiver_GetTarget(void* p) { (void)p; return g_ret; }

int __fastcall CTCPerceiveThing_IsPlayerExtendedLineOfSight(CTCPerceiveThing* self)
{
    void* sub = (void*)(self->sub);
    CTarget* t = Perceiver_GetTarget(sub);
    if (t) {
        CTarget* t2 = Perceiver_GetTarget(sub);
        if (t2->kind == 2) {
            if (self->flag)
                return 1;
        }
    }
    return 0;
}

int main() {
    CTCPerceiveThing obj;
    CTarget tgt;

    // null target -> false
    g_ret = 0;
    obj.flag = 1;
    if (CTCPerceiveThing_IsPlayerExtendedLineOfSight(&obj)) { std::printf("FAIL null\n"); return 1; }

    // kind != 2 -> false
    tgt.kind = 1; g_ret = &tgt; obj.flag = 1;
    if (CTCPerceiveThing_IsPlayerExtendedLineOfSight(&obj)) { std::printf("FAIL kind\n"); return 1; }

    // kind==2 but flag 0 -> false
    tgt.kind = 2; obj.flag = 0;
    if (CTCPerceiveThing_IsPlayerExtendedLineOfSight(&obj)) { std::printf("FAIL flag\n"); return 1; }

    // kind==2 and flag -> true
    tgt.kind = 2; obj.flag = 1;
    if (!CTCPerceiveThing_IsPlayerExtendedLineOfSight(&obj)) { std::printf("FAIL true\n"); return 1; }

    std::printf("CTCPerceiveThing_00744830_TEST PASS\n");
    return 0;
}