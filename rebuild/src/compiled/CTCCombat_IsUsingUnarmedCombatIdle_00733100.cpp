extern "C" unsigned char g_flag;

struct CObj {
    CObj* Get(int key);
    CObj* Get2();
    bool  Check3();
    bool  Check();
    unsigned char buf[0x300];
};

struct CTCCombat {
    void*  pad0;
    CObj*  obj;
    char   pad2[0x14];
    int    count;
};

bool __fastcall CTCCombat_IsUsingUnarmedCombatIdle(CTCCombat* self)
{
    if (self->obj->Get(0x1b)->Check())
        return false;
    if (self->obj->Get(0x2f)->Check())
        return false;
    if (self->obj->Check3())
        return false;
    if (self->obj->Get2()->buf[0x217])
        return true;
    if (g_flag)
        return false;
    return self->count > 0;
}