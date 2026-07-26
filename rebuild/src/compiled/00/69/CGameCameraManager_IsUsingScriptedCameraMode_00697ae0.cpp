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