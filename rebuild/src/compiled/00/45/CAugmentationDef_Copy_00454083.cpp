struct CAugmentationDef;

extern void __fastcall AugmentationDef_CopyImpl(CAugmentationDef* self, const CAugmentationDef* src);

void __fastcall CAugmentationDef_Copy(CAugmentationDef* self, const CAugmentationDef* src)
{
    AugmentationDef_CopyImpl(self, src);
}