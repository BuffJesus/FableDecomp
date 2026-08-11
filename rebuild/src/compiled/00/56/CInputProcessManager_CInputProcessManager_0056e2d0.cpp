#pragma optimize("s",on)
// CInputProcessManager::~CInputProcessManager  @ 0x0056e2d0
// Pattern: set own vtable, destruct member subobject at +0xa4, tail-call base dtor.

// A member subobject whose destructor is an out-of-line call (0x00a01c10).
struct MemberObj {
    void* p;
    ~MemberObj();   // out-of-line
};

// Base class whose destructor is an out-of-line call (0x0056e0e0), tail-called.
struct BaseProc {
    // implicit vtable pointer at +0 (from virtual dtor)
    char pad[0xa0];        // fills to +0xa4 in the derived layout
    virtual ~BaseProc();
};

// Derived class. Member subobject lands at +0xa4 (after base's vtbl+pad = 0xa4).
struct CInputProcessManager : public BaseProc {
    MemberObj member;      // at +0xa4
    virtual ~CInputProcessManager();
};

CInputProcessManager::~CInputProcessManager()
{
    // compiler: [this] = derived vtable; then ~member (ecx=this+0xa4);
    // then chain to ~BaseProc (tail jmp).
}