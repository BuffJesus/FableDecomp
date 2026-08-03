#include "rebuild_abi.h"

// CThreadedFile::CThreadedFile() -- retail 0x0098dfd0
// Layout (from retail bytes):
//   push esi; mov esi,ecx
//   call BaseCtor(this)                  ; e8 18 c3 00 00 -> 0x0099a2f0
//   lea ecx,[esi+0xc]
//   mov dword ptr [esi], 0x0129a158      ; vtable ptr
//   call MemberCtor(ecx=this+0xc)        ; e8 ea ce 00 00 -> 0x0099aed0
//   xor al,al
//   mov [esi+0x18], al
//   mov [esi+0x19], al
//   mov dword ptr [esi+0x8], 0xffffffff
//   mov eax, esi
//   pop esi
//   ret

struct CThreadedFile
{
    unsigned char bytes[0x1C];
};

// Base object constructor -- separate compiled unit at 0x0099a2f0, treated as
// an opaque external symbol so it is called rather than inlined (matches the
// retail call instruction).
extern "C" void FABLE_FASTCALL
CThreadedFileBaseConstruct_0099A2F0(void* self);

// Member object constructor at offset 0xC -- external symbol at 0x0099aed0.
extern "C" void FABLE_FASTCALL
CThreadedFileMemberConstruct_0099AED0(void* self);

extern "C" CThreadedFile* FABLE_FASTCALL
CThreadedFile_ctor(CThreadedFile* this_)
{
    CThreadedFileBaseConstruct_0099A2F0(this_);

    *reinterpret_cast<unsigned long*>(this_->bytes) = 0x0129a158ul;
    CThreadedFileMemberConstruct_0099AED0(this_->bytes + 0xC);

    this_->bytes[0x18] = 0;
    this_->bytes[0x19] = 0;
    *reinterpret_cast<unsigned long*>(this_->bytes + 0x8) = 0xfffffffful;

    return this_;
}