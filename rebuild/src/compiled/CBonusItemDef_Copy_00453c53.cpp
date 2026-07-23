struct CBonusItemDef;
extern void __fastcall CBonusItemDef_CopyImpl(CBonusItemDef* self, const CBonusItemDef* src);

void __fastcall CBonusItemDef_Copy(CBonusItemDef* self, const CBonusItemDef* src)
{
    CBonusItemDef_CopyImpl(self, src);
}