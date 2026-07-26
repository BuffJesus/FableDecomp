struct CHeroTitleDef;

extern void __fastcall CHeroTitleDef_Copy_impl(CHeroTitleDef* self, void* src);

void __fastcall CHeroTitleDef_Copy(CHeroTitleDef* self, void* src)
{
    CHeroTitleDef_Copy_impl(self, src);
}