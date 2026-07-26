struct CDegradableDef;
extern void __fastcall CDegradableDef_Copy_impl(CDegradableDef* self, void* edx, const CDegradableDef* src);
void __fastcall CDegradableDef_Copy(CDegradableDef* self, void* edx, const CDegradableDef* src)
{
    CDegradableDef_Copy_impl(self, edx, src);
}