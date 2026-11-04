// CWideString::UnassignString @ 0x0099B4D0 (FableWin ?UnassignString@CWideString@@AAEXXZ).
//
// Drops this string's reference on its shared data; the last owner frees the
// text buffer (CRT free) and the data block (operator delete). The handle is
// cleared either way.

#include "fable_string.h"

#include <stdlib.h>

// Retail inlines this into the string units that use it (the operator+
// units in build_bootstrap.ps1 carry the same inline body as a pick-any
// COMDAT), so it is defined inline here too and these objects link together.
// The use below (link scaffolding, not retail code, kept a call by noinline)
// makes this unit emit the out-of-line copy retail keeps at 0x0099B4D0.
inline __declspec(noinline) void CWideString::UnassignString()
{
    CWideStringData* shared = storage_;
    if (shared != 0)
    {
        if (--shared->owners <= 0)
        {
            CWideStringData* storage = storage_;
            if (storage != 0)
            {
                if (storage->text != 0)
                    free(storage->text);
                delete storage;
            }
        }
        storage_ = 0;
    }
}

// Link scaffolding (not retail code): an explicit instantiation may name a
// private member, so this takes UnassignString's address and forces the
// COMDAT out without a friend declaration.
template<void (CWideString::*Member)()>
struct CEmitWideStringMember
{
    static void (CWideString::*const Address)();
};

template<void (CWideString::*Member)()>
void (CWideString::*const CEmitWideStringMember<Member>::Address)() = Member;

template struct CEmitWideStringMember<&CWideString::UnassignString>;
