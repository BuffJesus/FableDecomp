// CCharString::SetChar @ 0x0099f390  __fastcall void(long index, char ch)
struct CharStringData { char* buffer; };  // *data == buffer base pointer

struct CCharString {
    CharStringData* data;  // [this+0] : 0 means empty
};

// 0x99eaf0 : CopyOnWrite / detach helper (thiscall, no args)
extern "C" void __fastcall CCharString_Detach(CCharString* self);

// self in ecx, dummy in edx (unused), index+ch on stack -> matches thiscall stack layout, ret 8
void __fastcall SetChar(CCharString* self, int /*edx*/, long index, char ch)
{
    if (self->data != 0) {
        CCharString_Detach(self);
        char* base = *(char**)self->data;   // ecx = *(*this)
        base[index] = ch;                    // [eax+ecx] = dl
    }
}