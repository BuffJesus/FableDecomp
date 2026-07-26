struct Inner2 { char _p[0x14]; long f14; };
struct Inner1 { char _p[0x1c]; Inner2* p1c; };
struct CGameScriptInterface { void* vt; Inner1* mid; };

long __fastcall CGameScriptInterface_GetDayOfWeek(const CGameScriptInterface* self)
{
    Inner1* m = self->mid;
    Inner2* x = m->p1c;
    return x->f14 % 7;
}