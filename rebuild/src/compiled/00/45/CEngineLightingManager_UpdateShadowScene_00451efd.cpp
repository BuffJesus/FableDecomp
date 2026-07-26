#pragma optimize("s",on)
// CEngineLightingManager::UpdateShadowScene @ 00451efd
struct GuardT { void Do(int handler, int arg); };
struct HandlerB { void Do(int arg); };

struct CEngineLightingManager {
    char pad00[0x18];
    int  mode;         // +0x18
    char pad1c[0x24-0x1c];
    HandlerB* handlerA; // +0x24
    HandlerB* handlerB; // +0x28
};

extern "C" void __stdcall LockScope(int id);

void __fastcall CEngineLightingManager_UpdateShadowScene(CEngineLightingManager* self, void* /*edx*/, int* arg1)
{
    LockScope(0x122d70e);
    int guard = 0;
    switch (self->mode) {
    case 2:
        ((GuardT*)&guard)->Do((int)self->handlerA, (int)arg1);
        break;
    case 3:
        self->handlerB->Do(*arg1);
        break;
    }
}