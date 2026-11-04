// CDisplayEngine::~CDisplayEngine @ 0x004354C0  (105 bytes)
// Genuine C++ decompilation for VC7.1 (cl /c /O2 /Oy /W3).
//
// This is an ordinary MSVC derived-class destructor.  The retail bytes are the
// stock destructor codegen:
//   * on entry cl resets this->vptr to CDisplayEngine's own vftable
//     (mov dword ptr [esi],0x1231574  -- the vftable address is a masked abs op);
//   * runs the destructor body;
//   * tail-calls the base-class destructor (jmp CDisplayEngineBase::~..),
//     which is out-of-line here so cl emits a relocation-masked jmp with this in ecx.
//
// Body, one statement at a time, matching the retail i386:
//   this->m_renderTarget->Shutdown();          // virtual slot 3 -> mov eax,[ecx]; call [eax+0xc]
//   FableDisplayGetSubsystem(this->m_renderTarget)->Flush();  // __stdcall helper then __fastcall
//   if (this->m_ownerRef) {                     // counted-pointer control block release
//       if (--this->m_ownerRef->owners == 0) {
//           this->m_ownerRef->destroy(this->m_ownerRef->object);
//           FableDisplayFree(this->m_ownerRef);
//       }
//   }
//   this->m_ownerPtr = 0;
//   this->m_ownerRef = 0;
//
// The member is re-read from `this` (esi) at each mention because the interleaved
// masked calls / memory writes may alias `this`, so cl reloads rather than caches --
// exactly the retail reload pattern.

typedef signed int   fable_i32;
typedef unsigned int fable_u32;

// ---- the object whose subsystem-3 virtual is invoked (this->m_renderTarget) ----
struct CDisplayRenderTarget {
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void Shutdown();   // vtable slot 3 -> call dword ptr [eax+0xc]
};

// ---- subsystem returned by the __stdcall helper; Flush is a no-arg __fastcall ----
struct CDisplaySubsystem {
    void Flush();              // out-of-line __fastcall -> masked call, this in ecx
};

// masked helper: takes the render target on the stack, returns the subsystem.
// __stdcall => callee pops its one argument, so no add esp follows the call.
extern CDisplaySubsystem* __stdcall FableDisplayGetSubsystem_004354c0(CDisplayRenderTarget* rt);

// masked cdecl deallocator for the control block.
extern "C" void __cdecl FableDisplayFree_004354c0(void* block);

// counted-pointer reference-control block at this->m_ownerRef.
struct CDisplayOwnerControl {
    fable_i32 owners;                          // +0x0
    void (__fastcall *destroy)(void* object);  // +0x4  (this in ecx, from +0x8)
    void* object;                              // +0x8
};

// ---- base class: supplies the vptr and the tail-called destructor ----
struct CDisplayEngineBase {
    virtual ~CDisplayEngineBase();   // out-of-line -> masked base-dtor tail jmp
};

struct CDisplayEngine : public CDisplayEngineBase {
    char                 pad_04[0x2C - 0x04]; // reach offset 0x2C
    CDisplayRenderTarget* m_renderTarget;      // +0x2C
    char                 pad_30[0xE0 - 0x30]; // reach offset 0xE0
    void*                m_ownerPtr;           // +0xE0
    CDisplayOwnerControl* m_ownerRef;          // +0xE4

    virtual ~CDisplayEngine();
};

CDisplayEngine::~CDisplayEngine()
{
    m_renderTarget->Shutdown();
    FableDisplayGetSubsystem_004354c0(m_renderTarget)->Flush();

    if (m_ownerRef) {
        if (--m_ownerRef->owners == 0) {
            m_ownerRef->destroy(m_ownerRef->object);
            FableDisplayFree_004354c0(m_ownerRef);
        }
    }

    m_ownerPtr = 0;
    m_ownerRef = 0;
}