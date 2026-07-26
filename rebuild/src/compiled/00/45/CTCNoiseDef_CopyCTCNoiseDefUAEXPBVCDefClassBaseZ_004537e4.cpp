struct CDefClassBase;
struct CTCNoiseDef;
extern void __fastcall CTCNoiseDef_Copy_impl(CTCNoiseDef* self, void* edx, const CDefClassBase* other);
void __fastcall CTCNoiseDef_Copy(CTCNoiseDef* self, void* edx, const CDefClassBase* other)
{
    CTCNoiseDef_Copy_impl(self, edx, other);
}