#include "fable_string.h"

// Retail's copy constructor is inline: every unit that copies a CWideString
// emits it as a pick-any COMDAT (the operator+ units inline it and emit one
// too), and the linker kept this copy. Defining it inline here as well lets
// these objects link together; the use below (link scaffolding, not retail
// code, kept a call by noinline) makes this unit emit it.
inline __declspec(noinline) CWideString::CWideString(const CWideString& other)
    : storage_(0)
{
    ++g_CWideStringInstanceCount_013BCA20;

    if (&other != this && other.storage_ != storage_)
    {
        if (storage_ != 0)
            UnassignString();

        if (other.storage_ != 0)
        {
            storage_ = other.storage_;
            ++storage_->owners;
        }
    }
}

namespace
{
    CWideString EmitCopyConstructor(const CWideString& text)
    {
        return text;
    }
}
