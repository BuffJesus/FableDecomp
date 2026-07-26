#pragma optimize("s",on)
struct LMLocal { int v; void Fill(void* a, int arg); };
struct LMTarget {
    void Handle(int val);
    void Setup(int tag);
};

struct CEngineLightingManager {
    char pad[0x18];
    int mode;      // +0x18
    char pad2[0x24-0x1c];
    void* p24;     // +0x24
    LMTarget* p28; // +0x28
};

void __fastcall CEngineLightingManager_UpdateShadowScene(CEngineLightingManager* self, void* edx, int* arg)
{
    LMLocal local;
    ((LMTarget*)self)->Setup(0x122d70e);
    local.v = 0;
    switch (self->mode) {
    case 2:
        local.Fill(self->p24, (int)arg);
        break;
    case 3:
        self->p28->Handle(*arg);
        break;
    }
}