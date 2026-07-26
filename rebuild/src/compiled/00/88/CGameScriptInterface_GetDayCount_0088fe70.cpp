struct Inner2 { unsigned char _p[0x14]; long day; };
struct CMid { unsigned char _p[0x1c]; Inner2* p1c; };
struct CGameScriptInterface { void* vt; CMid* mid; };

long __fastcall CGameScriptInterface_GetDayCount(const CGameScriptInterface* self) {
    return self->mid->p1c->day;
}