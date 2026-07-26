struct CActionUseDef;
extern void __fastcall CActionUseDef_Copy_impl(CActionUseDef* self, void* edx_unused, const CActionUseDef* other);

void __fastcall CActionUseDef_Copy(CActionUseDef* self, void* edx_unused, const CActionUseDef* other)
{
    CActionUseDef_Copy_impl(self, edx_unused, other);
}