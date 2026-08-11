// CSoundAtmospheresDef::Transfer  @ retail 0x004EC153
// Disasm:
//   add ecx, 0x28
//   push ecx
//   mov ecx, [esp+8]      ; ctx (the CPersistContext&)
//   call 0x4331F9         ; CPersistContext::Transfer(void* member)  (__fastcall)
//   ret 4
//
// i.e.  ctx.Transfer( &this->m_member_at_0x28 );

struct CPersistContext
{
    // Non-inline external __fastcall method -> plain call, ctx in ecx.
    void Transfer(void* p);
};

struct CSoundAtmospheresDef
{
    char pad[0x28];
    int  m_member;   // transferable member at +0x28

    void Transfer(CPersistContext& ctx);
};

void CSoundAtmospheresDef::Transfer(CPersistContext& ctx)
{
    ctx.Transfer(&this->m_member);
}