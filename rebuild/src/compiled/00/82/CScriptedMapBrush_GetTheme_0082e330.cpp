// CScriptedMapBrush::GetTheme  retail 0x0082e330  (77 bytes)
// __thiscall member lifted as a free __fastcall(self in ecx).
// Signature: unsigned long GetTheme(long x, long y, unsigned long layer) const  -> ret 0xc
//
// Retail body (annotated):
//   edx = x                     ; [esp+4]
//   if (x <  self->minX) fail   ; [ecx+0x38]
//   if (x >= self->maxX) fail   ; [ecx+0x40]
//   eax = y                     ; [esp+8]
//   if (y <  self->minY) fail   ; [ecx+0x3c]
//   if (y >= self->maxY) fail   ; [ecx+0x44]
//   eax = (y - minY)*(maxX - minX) - minX + x
//   edx = layer + eax*5         ; layer = [esp+0xc]
//   return ((u32*)self->cells)[edx + 1]   ; self->cells = [ecx+8]
//   fail: return 0
//
// The retail stream keeps all four bounds checks flowing into ONE shared body
// with a single shared xor-eax fail tail; the previously-landed nested-if source
// factored the compare block differently.  This shape reproduces the exact
// mov/cmp/jl/jge sequence and the shared fail tail.

struct CScriptedMapBrush {
    char        pad00[0x8];
    unsigned long* cells;   // +0x08
    char        pad0c[0x38 - 0x0c];
    long        minX;       // +0x38
    long        minY;       // +0x3c
    long        maxX;       // +0x40
    long        maxY;       // +0x44
};

unsigned long __fastcall CScriptedMapBrush_GetTheme(
    const CScriptedMapBrush* self, int /*edx*/, long x, long y, unsigned long layer)
{
    if (x >= self->minX && x < self->maxX &&
        y >= self->minY && y < self->maxY)
    {
        long idx = (y - self->minY) * (self->maxX - self->minX) - self->minX + x;
        return self->cells[layer + (unsigned long)idx * 5 + 1];
    }
    return 0;
}
