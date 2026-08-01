// CWideString::operator=(const CWideString&) @ 0x0099B7D0.
//
// Genuine C++ reconstruction (real statements only, no inline assembly).
//
// CWideString is a refcounted-string wrapper (mirror of CCharString's
// operator=, but the shared representation's refcount lives at +0x0C
// instead of CCharString's layout). The object itself is a single pointer
// to a shared CWideStringData block; operator= implements classic
// copy-on-write refcount transfer:
//
//   if (&rhs == this) return *this;              // self-assign guard
//   if (rhs.data != this->data) {
//       if (this->data != 0) this->Release();    // drop old share
//       if (rhs.data != 0) {
//           this->data = rhs.data;                // take new share
//           ++this->data->refCount;                // refCount at data+0x0C
//       }
//   }
//   return *this;
//
// Release() is a private thiscall member (ecx=this) that itself inspects
// this->data and frees/decrements as needed -- the retail call site loads
// ecx from esi (this), not from the data pointer in eax, so Release is
// modeled as a method on CWideString, not on the data block.
//
// Retail codegen notes that pin this exact spelling:
//  * The `this` pointer is cached into esi and the rhs pointer into edi at
//    entry (`mov edi,[esp+0xC]` / `mov esi,ecx`) -- both are used repeatedly
//    across the three comparisons/branches, matching /O2 register caching
//    of `this` and `&rhs` rather than re-deriving them.
//  * The self-assign check compares the two POINTERS (edi vs esi), i.e.
//    `&rhs != this`, not a data-pointer comparison.
//  * The data-identity check compares `*edi` (rhs.data) against a value
//    loaded once into eax (`mov eax,[esi]` = this->data), i.e.
//    `rhs.data != this->data`.
//  * Release() is only called when the OLD data pointer (eax, captured
//    before the call) is non-null -- test eax,eax / je skip.
//  * After the (possible) Release call, the code RE-LOADS `eax = [edi]`
//    (rhs.data) fresh rather than reusing a stale register, matching a
//    second dereference of rhs.data post-call.
//  * The store `this->data = rhs.data` and the refcount increment
//    `++rhs.data->refCount` (at +0x0C) only happen when rhs.data != 0.
//  * Return value is `this` (mov eax,esi before epilogue), and the callee
//    pops the single stack argument (`ret 4`), consistent with thiscall
//    taking a const-ref stack argument.

class CWideStringData
{
public:
    unsigned short* Chars;      // +0x00 (opaque here)
    unsigned long Capacity;     // +0x04 (opaque padding to align RefCount)
    unsigned long Length;       // +0x08 (opaque padding to align RefCount)
    unsigned long RefCount;     // +0x0C
};

class CWideString
{
public:
    CWideString& operator=(const CWideString& rhs);

private:
    void Release();             // 0x0099B7D0-adjacent helper; masked callee

    CWideStringData* data;      // +0x00
};

CWideString& CWideString::operator=(const CWideString& rhs)
{
    if (&rhs != this)
    {
        CWideStringData* oldData = data;
        if (rhs.data != oldData)
        {
            if (oldData != 0)
                Release();
            CWideStringData* newData = rhs.data;
            if (newData != 0)
            {
                data = newData;
                ++newData->RefCount;
            }
        }
    }
    return *this;
}