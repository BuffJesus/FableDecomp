extern void __fastcall CVillagePeopleDef_CopyImpl(void* self, void* other);

void __fastcall CVillagePeopleDef_Copy(void* self, void* other)
{
    CVillagePeopleDef_CopyImpl(self, other);
}