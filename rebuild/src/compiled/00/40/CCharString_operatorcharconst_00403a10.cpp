// CCharString::operator const char*() const @ 0x00403A10.
//
// Genuine C++ reconstruction (real statements only, no inline assembly).
//
// __fastcall accessor (this in ecx). It loads the string representation
// pointer from offset 0 of the object (mov eax,[ecx]). When that pointer is
// null it returns the address of the shared empty-string sentinel, whose
// absolute address 0x0129AAF4 is relocation-masked in the parity check
// (mov eax,0x129aaf4; ret). Otherwise it returns rep->data, which is the
// first member of the representation block, hence a single indirect load
// (mov eax,[eax]; ret).
//
// Spelling notes that pin the exact retail bytes:
//  * `if (r == 0) return sentinel;` places the null-path return inline (the
//    fall-through) so VC7.1 emits `test eax,eax` + `jne <tail>` (0x75) and
//    the deref tail after it -- matching the retail 75 06 / mov eax,imm / ret
//    / mov eax,[eax] / ret layout. A `!= 0` phrasing would invert the branch.
//  * The sentinel is returned as the ADDRESS of a global object, so it lowers
//    to a `mov eax,imm32` of that address, not an indirect load.
//
// OVER-CAPTURE: the 106-byte oracle slice holds three functions separated by
// int3 padding. This is only the leading one (offsets 0x00..0x0E, ret at
// 0x0E). real_len = 15. The two trailers (mov eax,[ecx+0x60];ret at 0x10 and
// the char-fetch routine at 0x20) are NOT reproduced here.

struct StringRep { char* data; long len; };
struct CCharString { StringRep* rep; };

extern char g_EmptyCharString_0129AAF4[1];

const char* __fastcall CCharString_operator_char_const(const CCharString* self)
{
    StringRep* r = self->rep;
    if (r == 0)
        return g_EmptyCharString_0129AAF4;
    return r->data;
}