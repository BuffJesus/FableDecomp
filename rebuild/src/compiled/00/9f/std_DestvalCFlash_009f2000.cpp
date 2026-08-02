// std::_Dest_val<std::allocator<NCreatureHitNotification::CFlash>, NCreatureHitNotification::CFlash>
// Retail bytes: 68 70aa3c01 | ff15 0cfe4301 | c3
// = push 0x013caa70 (literal data address); call [IAT 0x0143fe0c] (CRT dtor/free thunk,
// __stdcall so the callee pops its own single argument); ret (plain, no thiscall cleanup --
// both logical params (allocator*, T*) arrive purely in ecx/edx and are unused, so there is
// no stack-passed argument left for this function to clean up).

typedef void (__stdcall *DestroyFlashFn)(void*);

extern "C" DestroyFlashFn __imp__DestroyFlashObj; // IAT slot at 0x0143fe0c
extern "C" char g_flashInstance_013caa70[1];       // literal object address 0x013caa70

struct CFlashAllocatorT;
struct CFlashObjT;

void __fastcall Dest_val_CFlash(CFlashAllocatorT* self, CFlashObjT* val)
{
    __imp__DestroyFlashObj(g_flashInstance_013caa70);
}