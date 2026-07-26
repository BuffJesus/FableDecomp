#pragma optimize("s",on)
#include "rebuild_abi.h"

// 0x0046f191 CActiveFile::OnReadFinished (35 bytes)
// __fastcall modeled as free __fastcall(thisptr).
//
// Layout inferred from disasm:
//   this+0x0 -> a pointer (member object); reloaded twice.
//   this+0x4 -> a dword passed as edx to the first (fastcall) callee.
//
// First call: __fastcall(obj /*ecx*/, arg /*edx*/, char* /*stack*/)
//   ecx = *(this)          (the member object pointer)
//   edx = *(this+4)
//   pushed = &local byte
// Second call: single stack arg (esi), caller cleans (pop ecx) -> __cdecl,
//   invoked only if *(this) is non-null.

struct CActiveFile_obj;

extern "C" void __fastcall sub_first(CActiveFile_obj* self, unsigned int a2, char* out);
extern "C" void sub_second(CActiveFile_obj* p);

struct CActiveFile {
    CActiveFile_obj* m_obj;   // +0x00
    unsigned int     m_arg;   // +0x04
};

void __fastcall OnReadFinished_0046f191(CActiveFile* thisptr)
{
    char local;
    sub_first(thisptr->m_obj, thisptr->m_arg, &local);

    CActiveFile_obj* p = thisptr->m_obj;
    if (p != 0) {
        sub_second(p);
    }
}