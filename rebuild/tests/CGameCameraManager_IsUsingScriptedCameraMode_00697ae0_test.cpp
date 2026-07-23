#include <cstdio>

struct Sub {
    struct Vtbl {
        void* s0;
        void* s1;
        bool (__fastcall* s2)(Sub*);
    };
    Vtbl* vptr;
};

struct CGameCameraManager {
    char pad[0xd4];
    Sub* obj_d4;
    char pad2[4];
    int  flag_dc;
};

bool __fastcall CGameCameraManager_IsUsingScriptedCameraMode(const CGameCameraManager* self) {
    if (self->flag_dc == 0) {
        Sub* s = self->obj_d4;
        if (s == 0)
            return false;
        if (s->vptr->s2(s))
            return false;
    }
    return true;
}

static bool __fastcall vc_true(Sub*) { return true; }
static bool __fastcall vc_false(Sub*) { return false; }

static Sub::Vtbl vt_true = { 0, 0, vc_true };
static Sub::Vtbl vt_false = { 0, 0, vc_false };

int main() {
    CGameCameraManager m;
    Sub s;
    /* case 1: flag_dc set -> true */
    m.flag_dc = 1; m.obj_d4 = 0;
    if (CGameCameraManager_IsUsingScriptedCameraMode(&m) != true) { std::printf("FAIL c1\n"); return 1; }
    /* case 2: flag_dc 0, obj null -> false */
    m.flag_dc = 0; m.obj_d4 = 0;
    if (CGameCameraManager_IsUsingScriptedCameraMode(&m) != false) { std::printf("FAIL c2\n"); return 1; }
    /* case 3: flag_dc 0, vcall true -> false */
    s.vptr = &vt_true; m.flag_dc = 0; m.obj_d4 = &s;
    if (CGameCameraManager_IsUsingScriptedCameraMode(&m) != false) { std::printf("FAIL c3\n"); return 1; }
    /* case 4: flag_dc 0, vcall false -> true */
    s.vptr = &vt_false; m.flag_dc = 0; m.obj_d4 = &s;
    if (CGameCameraManager_IsUsingScriptedCameraMode(&m) != true) { std::printf("FAIL c4\n"); return 1; }
    std::printf("CGameCameraManager_00697ae0_TEST PASS\n");
    return 0;
}