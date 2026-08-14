struct Param;

void __fastcall Param_method(Param* self);         // 0xc37930
void __cdecl free_param(Param* p);                 // 0xbfe9bc

struct Param {
    char pad[0x10];
    int refcount;   // +0x10
};

struct CEngineResourceManager { char x; };

void __fastcall Render(CEngineResourceManager* self, int edx, Param* p)
{
    (void)self;
    if (--p->refcount == 0) {
        Param_method(p);
        free_param(p);
    }
}