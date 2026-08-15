#pragma optimize("s",on)
// Reconstruction of Alloc @ 0x00471c79
#include <new>

struct CScriptDataBase;

// Subobject at +8 whose constructor takes the long arg (native thiscall, value-returning).
struct SubObj {
    SubObj(long a);   // extern ctor
    int pad[6];
};

CScriptDataBase* __stdcall Alloc(long a) {
    void* base = ::operator new(0x20);
    SubObj* p = (SubObj*)((char*)base + 8);
    if (p) {
        new (p) SubObj(a);   // placement construct -> native thiscall, arg on stack
    }
    return (CScriptDataBase*)base;
}