struct CHeroMarriageDef;
extern void __fastcall CHeroMarriageDef_CopyImpl(CHeroMarriageDef* self, void* other);

void __fastcall CHeroMarriageDef_Copy(CHeroMarriageDef* self, void* other)
{
    CHeroMarriageDef_CopyImpl(self, other);
}