extern "C" void __cdecl my_free(void* p);

struct CLocalDetailCacheMap {
    char pad[0x10];
    void* f10;
};

void __fastcall CloseStaticMap(CLocalDetailCacheMap* self)
{
    my_free(self->f10);
    self->f10 = 0;
}