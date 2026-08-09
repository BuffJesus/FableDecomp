struct Sub { char pad[4]; };

void __fastcall Sub_Update(Sub* self);

struct Inner {
    char pad[0x1ec];
    Sub sub;
};

struct CEngineResourceManager {
    char pad[0x10];
    Inner* field10;
};

void __fastcall CEngineResourceManager_WorldFrameUpdate(CEngineResourceManager* self, int /*edx*/, bool b)
{
    Inner* p = self->field10;
    if (p) {
        Sub_Update(&p->sub);
    }
}