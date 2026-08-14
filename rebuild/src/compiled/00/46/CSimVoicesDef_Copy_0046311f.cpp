// Byte-exact reconstruction of CSimVoicesDef::Copy @ 0x0046311f
// __fastcall void CSimVoicesDef::Copy(CDefClassBase const* src)
// Authored as a real member function so VC7.1 emits __fastcall:
//   this in ecx, src on the stack, ret 4.
// Callees are also members => __fastcall (this=ecx, arg on stack).

struct CDefClassBase {
    // base Copy @ 0x431ed0 : __fastcall CDefClassBase::Copy(src)
    void Copy(CDefClassBase const* src);
};

// A sub-object type at this+0x3c with its own non-virtual Copy @ 0x46313e.
struct SubVoices {
    void Copy(SubVoices const* src);
};

struct CSimVoicesDef {
    char pad_base[0x3c];  // base region
    SubVoices sub;        // at +0x3c

    void Copy(CDefClassBase const* src);
};

void CSimVoicesDef::Copy(CDefClassBase const* src)
{
    ((CDefClassBase*)this)->Copy(src);
    SubVoices const* srcSub = (SubVoices const*)((char const*)src + 0x3c);
    this->sub.Copy(srcSub);
}