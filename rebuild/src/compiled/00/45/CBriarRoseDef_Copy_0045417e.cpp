struct CBriarRoseDef;

extern void __fastcall CBriarRoseDef_CopyImpl(CBriarRoseDef* self, const CBriarRoseDef* src);

void __fastcall CBriarRoseDef_Copy(CBriarRoseDef* self, const CBriarRoseDef* src)
{
    CBriarRoseDef_CopyImpl(self, src);
}