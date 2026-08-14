// CHeroSuitDef::Copy @ 0x004e54ad
struct CDefClassBase {
    void Copy(CDefClassBase const* src);   // @ 0x431f10, this in ecx, param on stack
};
struct SubDef {
    void Copy(SubDef const* src);          // @ 0x4e54cc, this in ecx, param on stack
};
struct CHeroSuitDef {
    char basepad[0x28];
    SubDef sub;   // at +0x28
};

void __fastcall CHeroSuitDef_Copy(CHeroSuitDef* self, void* edx, CDefClassBase const* src)
{
    ((CDefClassBase*)self)->Copy(src);
    self->sub.Copy((SubDef const*)((char const*)src + 0x28));
}