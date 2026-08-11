// Retail 0x008fd56a: _Dest_val<pair<long,CRegisteredMusicEntry>> style destroy helper.
// push esi; mov esi,ecx; lea ecx,[esi+0x34]; call 0xa01c10; mov ecx,esi; pop esi; jmp 0x8fd544
// VOID member: destroy the sub-object at this+0x34, then tail-call the base/other destroy on this.

extern "C" void __fastcall helper_a01c10(void* self);   // call 0xa01c10 (ecx = this+0x34)
extern "C" void __fastcall tail_8fd544(void* self);      // jmp  0x8fd544 (ecx = this)

extern "C" void __fastcall _Dest_val_pair_long_CRegisteredMusicEntry(void* self)
{
    helper_a01c10((char*)self + 0x34);
    tail_8fd544(self);
}