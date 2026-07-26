#pragma optimize("s",on)
struct SVec2 { int a; int b; };

struct CEngineLightingManager {
    char pad[0x18];
    int  mode;      // +0x18
    char pad2[0x24 - 0x1c];
    void* objA;     // +0x24
    void* objB;     // +0x28
};

#pragma optimize("y", off)

extern void __stdcall Marker(const char* s);

struct ObjA { int* Fetch(SVec2* out); };
struct ObjB { void Apply(SVec2* p); };

void __fastcall CEngineLightingManager_UpdateShadowScene(CEngineLightingManager* self, void* edx, SVec2* out)
{
    SVec2 tmp;
    Marker((const char*)0x122d70e);
    switch (self->mode) {
    case 2: {
        int* r = ((ObjA*)self->objA)->Fetch(&tmp);
        out->a = r[0];
        out->b = r[1];
        break;
    }
    case 3:
        ((ObjB*)self->objB)->Apply(out);
        break;
    }
}