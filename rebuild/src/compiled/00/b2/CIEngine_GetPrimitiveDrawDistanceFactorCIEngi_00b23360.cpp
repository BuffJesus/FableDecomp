// ?GetPrimitiveDrawDistanceFactor@CIEngine@@UBEMXZ @ 0x00b23360
// Retail: d9 05 d8 de 22 01 c3
//   fld DWORD PTR ds:0x122ded8
//   ret
//
// Virtual const accessor that returns a global float. The 'this' pointer
// (ecx) is unused: the value is a module-global loaded straight into st(0).
// Modeled as __fastcall (this in ecx, byte-identical to __fastcall for a
// this-only accessor). VC7.1 emits `fld DWORD PTR ds:<addr>; ret` for a
// function that simply returns a global float value.
//
// The address 0x122ded8 is relocation-masked in parity, so the global need
// not resolve to that exact address; declaring it extern lets the linker
// place it while the compiler still emits the fld/ret shape.

extern float g_PrimitiveDrawDistanceFactor;

float __fastcall CIEngine__GetPrimitiveDrawDistanceFactor(void *this_ptr)
{
    (void)this_ptr;
    return g_PrimitiveDrawDistanceFactor;
}