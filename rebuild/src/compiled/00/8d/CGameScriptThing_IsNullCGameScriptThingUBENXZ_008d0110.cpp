struct SThing {
    void* p0;
    int   v4;
};
struct CGameScriptThing {
    char pad[0x20];
    SThing* pThing;
};

extern SThing* g_nullThing;
extern "C" int __fastcall Thing_Compare(void* a, void* b);

bool __fastcall CGameScriptThing_IsNull(const CGameScriptThing* self)
{
    SThing* a = self->pThing;
    SThing* b = g_nullThing;
    if (a == b)
        return true;
    if (a != 0 && b != 0 && a->v4 == b->v4)
        return (bool)(Thing_Compare(a->p0, b->p0) == 0);
    return false;
}