#pragma optimize("s",on)
// Byte-exact reconstruction of CDrawMiniMap::GetRingAlpha @ 0x00643e52
// __fastcall (ecx=this) returning unsigned char.
// 0x4348d0 = subobject method returning float in st0 (this-call).
// 0xbfea70 = __ftol helper (reads st0, returns eax) -> emitted by float->int cast.
// [0x1230014] = global float scale.

// subobject method at 0x4348d0: __fastcall, returns float in st0
extern float __fastcall Ring_GetValue(void* self);

extern const float g_alphaScale; // [0x1230014]

// flags at +0x100 and +0x10c are bytes; subobjects at +0x100 and +0x138.
unsigned char __fastcall GetRingAlpha(char* self)
{
    if (*(unsigned char*)(self + 0x10c) == 0 &&
        *(unsigned char*)(self + 0x100) != 0)
    {
        int v = (int)(Ring_GetValue(self + 0x138) * g_alphaScale);
        return (unsigned char)(0xff - v);
    }
    return (unsigned char)(int)(Ring_GetValue(self + 0x100) * g_alphaScale);
}