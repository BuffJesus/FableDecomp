extern void __fastcall CContextSensitiveItemDef_CopyImpl(void* self, void* other);

void __fastcall CContextSensitiveItemDef_Copy(void* self, void* other)
{
    CContextSensitiveItemDef_CopyImpl(self, other);
}