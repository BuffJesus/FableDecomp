struct Inner {
    unsigned char _pad[0x91];
    unsigned char flags; // +0x91
};
struct CGameScriptInterface {
    Inner* (__fastcall **p0)(void*); // +0x0 : points at a vtable; slot 0x2c returns Inner*
};

bool __stdcall CGameScriptInterface_IsEntityInLimbo(CGameScriptInterface* self)
{
    Inner* (__fastcall **vt)(void*) = self->p0;
    Inner* e = (*(Inner* (__fastcall **)(void*))((char*)vt + 0x2c))(self);
    if (e == 0)
        return false;
    unsigned char f = e->flags;
    if (!(f & 1))
        return (f & 0x20) ? true : false;
    return false;
}