struct GSISub { char pad0[0xbc]; unsigned char b_bc; char pad1[0xc]; unsigned char b_c9; };
struct CGameScriptInterface { char pad0[0xc]; GSISub* sub; };

int __fastcall CGameScriptInterface_IsScreenFadingOut(const CGameScriptInterface* self)
{
    const GSISub* s = self->sub;
    if (s->b_bc) {
        if (s->b_c9) {
            return 1;
        }
    }
    return 0;
}