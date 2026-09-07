#include "engine/CTexture.h"  // retyped onto the PDB layout; byte parity re-verified
struct IDirect3DTexture9 { void (__stdcall **vtbl)(IDirect3DTexture9*, unsigned long); };
struct CTexture_Methods : CTexture {
    void Unlock(unsigned long v);
};
void CTexture_Methods::Unlock(unsigned long v){ IDirect3DTexture9* p=this->PD3DTexture; p->vtbl[0x50/4](p, v); }