// _Dest_val<allocator<pair<long,CMusicManager::CRegisteredMusicEntry>>,pair<long,CMusicManager::CRegisteredMusicEntry>>
// Retail bytes: e8ae0be5ff 8bc8 e9c7a5e4ff
//   call helper(pair*) -> eax        (call rel32)
//   mov ecx, eax                    (set up this for tailcall)
//   jmp realDtor                    (tailcall rel32, thiscall void(void) on ecx)
//
// This matches the standard MSVC7.1 STL shape for _Dest_val<_Ty>(allocator<_Ty>&, _Ty*):
// it forwards through a helper that resolves the actual sub-object needing
// non-trivial destruction (CRegisteredMusicEntry's payload member) and then
// tail-calls into that member's destructor with the resolved pointer as `this`.

struct CRegisteredMusicEntry; // opaque - only used by pointer

// Helper that returns (in eax) the pointer to the sub-object that actually
// needs destruction. Modeled as a free function taking the pair* in ecx
// (fastcall) so the call site matches "call rel32" with no argument setup.
extern "C" void* __fastcall Dest_val_music_resolve(void* pairPtr);

// Real destructor for the resolved sub-object; called via `this` in ecx,
// tailcalled (no further work after it in our function, so codegen turns
// the trailing call into a jmp).
extern "C" void __fastcall Dest_val_music_dtor(void* self);

extern "C" void __fastcall Dest_val_pair_music(void* pairPtr, void* alloc)
{
    (void)alloc;
    void* target = Dest_val_music_resolve(pairPtr);
    Dest_val_music_dtor(target);
}