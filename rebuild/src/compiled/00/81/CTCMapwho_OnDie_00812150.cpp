struct CTCMapwho { void Cleanup(); };
extern "C" void __fastcall Helper_die(CTCMapwho* self);
extern "C" void __cdecl retail_free(void* p);
void __fastcall OnDie(CTCMapwho* self)
{
    if (self) {
        Helper_die(self);
        retail_free(self);
    }
}