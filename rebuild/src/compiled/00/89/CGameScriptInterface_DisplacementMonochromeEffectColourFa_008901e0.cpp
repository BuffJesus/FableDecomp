/* CGameScriptInterface::DisplacementMonochromeEffectColourFadeOut @ 0x008901e0
 * 17-byte thiscall forwarder. Loads a chained pointer:
 *   eax = this->m_pWorld            ([this+0x04])
 *   ecx = m_pWorld->m_pEffectMgr    ([+0x18])
 *   ecx = m_pEffectMgr->m_pEffect   ([+0x1964])
 * then tail-calls the effect's ColourFadeOut(a,b,c). The tail callee returns a
 * pointer (return-value propagated) which pins eax as the first-load register and
 * yields the retail eax/ecx/ecx allocation with a direct jmp. */
extern void* __fastcall Effect_ColourFadeOut(void* self, void* edx, int a, int b, int c);

void* __fastcall CGameScriptInterface_DisplacementMonochromeEffectColourFadeOut(void* self, void* edx, int a, int b, int c)
{
    void* pWorld  = *(void**)((char*)self  + 0x4);
    void* pEffMgr = *(void**)((char*)pWorld + 0x18);
    void* pEffect = *(void**)((char*)pEffMgr + 0x1964);
    return Effect_ColourFadeOut(pEffect, edx, a, b, c);
}