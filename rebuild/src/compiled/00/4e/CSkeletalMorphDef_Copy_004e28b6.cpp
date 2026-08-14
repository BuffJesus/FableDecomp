// CSkeletalMorphDef::Copy 0x004e28b6
struct Base {
    void Copy(const void* src);   // 0x431f10, __fastcall(this, src)
};
struct Sub {
    void Copy(const void* src);   // 0x46078c, __fastcall(this, src)
};

struct CSkeletalMorphDef {
    char pad[0x28];
    Sub sub;
};

void __fastcall CSkeletalMorphDef_Copy(CSkeletalMorphDef* self, void* edx, const char* src)
{
    ((Base*)self)->Copy(src);
    self->sub.Copy(src + 0x28);
}