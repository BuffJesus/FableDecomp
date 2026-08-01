// CCharString::operator const char*() const @ 0x0099E4C0.
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
// OVER-CAPTURE: the 51-byte oracle slice holds two functions separated by an
// int3 padding byte at offset 0x0F. This authors only the leading one
// (offsets 0x00..0x0E, ret at 0x0E, real_len = 15). The trailing virtual-call
// forwarder (starting at offset 0x10, ends with ret 0xC) is a distinct
// function and is NOT reproduced here.

struct StringRep { char* data; long len; };
struct CCharString { StringRep* rep; };

extern char g_EmptyCharString_0129AAF4[1];

const char* __fastcall CCharString_operator_char_const_ptr_b(const CCharString* self)
{
    StringRep* r = self->rep;
    if (r == 0)
        return g_EmptyCharString_0129AAF4;
    return r->data;
}