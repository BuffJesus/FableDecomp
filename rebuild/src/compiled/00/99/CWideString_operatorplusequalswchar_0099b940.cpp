// CWideString::operator+=(wchar_t const*) @ 0x0099B940.
//
// Genuine C++ reconstruction (real statements only, no inline assembly).
//
// CWideString wraps a single CWideStringData* (this->storage_, offset +0x00).
// operator+= appends a NUL-terminated wide literal to the string:
//
//   if (storage_ != 0)
//   {
//       MakeOwnCopyOfData();                // detach/unshare owned buffer
//       CWideStringData* data = storage_;   // reload post-detach
//       size_t len = wcslen(text);          // scan appended literal's length
//       AppendRange(data, text, text, text + len, &text);
//   }
//   else if (text != 0 && *text != 0)
//   {
//       storage_ = AllocStringData(text, -1);
//   }
//   return *this;
//
// Retail codegen notes that pin this exact spelling:
//  * `this` is cached into esi at entry (`mov esi,ecx`) and used for the
//    rest of the function -- both branches read/write through esi, not ecx.
//  * The dispatch is a single `cmp dword ptr [esi],0` / `je` -- i.e. it
//    branches on whether storage_ is already allocated, not on whether the
//    appended text is empty.
//  * On the "already owns a buffer" path, MakeOwnCopyOfData() is called
//    BEFORE the text pointer argument is even reloaded from the stack --
//    i.e. detach happens unconditionally, even if text is null/empty
//    (retail loads `edx = [esp+8]` only after the call returns), and
//    storage_ is reloaded into ecx right after (`mov ecx,[esi]`), ahead of
//    the length scan -- both live registers are set up before the loop.
//  * The wide length scan is a classic open-coded wcslen (no call) with the
//    trip-count starting at the first character (retail:
//    `xor eax,eax; cmp [edx],ax; je done` then
//    `inc eax; cmp [edx+eax*2],0; jne loop`), padded to a 6-byte alignment
//    NOP (`lea ebx,[ebx+0]`) before the loop body.
//  * `edi` is pushed/restored around the append-helper call solely to hold
//    the ADDRESS of the (stack) text-pointer slot (`lea edi,[esp+0xc]`)
//    across the call -- i.e. the helper receives `&text`, not `text`, as
//    its last argument.
//  * The append helper is called with `data` (storage_, reloaded post-
//    detach) in ecx (fastcall 1st arg) and the ORIGINAL text pointer
//    (still live in edx from the length scan) in edx (fastcall 2nd arg);
//    edx's value is additionally pushed again as a genuine 3rd (stack)
//    argument, followed by the computed end-of-range pointer (`eax`) and
//    the `&text` address (`edi`) -- i.e. the helper's true signature takes
//    FIVE logical inputs: (data, text[fastcall], text[stack,redundant],
//    end, &text).
//  * On the "storage_ == 0" path, the text pointer is (re)loaded fresh from
//    the stack, and BOTH null and empty-string are checked
//    (`test eax,eax` / `je` then `cmp word ptr [eax],0` / `je`) before the
//    single AllocStringData(text, -1) call -- an empty/null literal leaves
//    storage_ untouched (still 0).
//  * Return value is `this` (mov eax,esi before epilogue); callee pops the
//    single stack argument (`ret 4`), consistent with a thiscall taking one
//    stack argument.

#include <wchar.h>

struct CWideStringData
{
    wchar_t* text;      // +0x00 (opaque here)
    unsigned long unknown04;   // +0x04
    unsigned long unknown08;   // +0x08
    long owners;               // +0x0C
};

// Opaque callee: appends the wide-char range [first,last) into the
// CWideStringData addressed by dataSelf, growing/reallocating storage_'s
// internal buffer as needed. `beginAgain` mirrors `first` (both carry the
// same value at the retail call site -- one via the fastcall edx slot, one
// pushed again on the stack) and ppSrcRoot is the address of the caller's
// local text-pointer slot (retail passes &text, not text).
extern "C" CWideStringData* __fastcall
FableWideStringAppendRange_0099C7E0(
    CWideStringData* dataSelf,
    const wchar_t* beginAgain,
    const wchar_t* first,
    const wchar_t* last,
    const wchar_t** ppSrcRoot);

class CWideString
{
public:
    const CWideString& operator+=(const wchar_t* text);

private:
    // Detaches/unshares storage_ so it is safe to mutate in place
    // (copy-on-write break). Matches retail CCharString/CWideString
    // sibling "MakeOwnCopyOfData" thiscall helpers (self-only, no stack
    // args) -- modeled as a real member so the retail call site's
    // `this` staying live in ecx across the call falls out of ordinary
    // codegen instead of an extra explicit argument push.
    void MakeOwnCopyOfData();

    CWideStringData* AllocStringData(const wchar_t* text, long length);

    CWideStringData* storage_;   // +0x00
};

const CWideString& CWideString::operator+=(const wchar_t* text)
{
    if (storage_ != 0)
    {
        MakeOwnCopyOfData();

        CWideStringData* data = storage_;

        // Retail open-codes wcslen here (no call), so the length scan is
        // written as an explicit manual index loop rather than a wcslen()
        // call.
        long i = 0;
        if (text[i] != 0)
        {
            do
            {
                ++i;
            } while (text[i] != 0);
        }

        FableWideStringAppendRange_0099C7E0(data, text, text, text + i, &text);
    }
    else if (text != 0 && *text != 0)
    {
        storage_ = AllocStringData(text, -1);
    }

    return *this;
}