// CThingAICreature::UpdateBrain @ 006d5a90
// if(!g_flag) return false; else tail-call sub_88bcb0(this->f1a8, a, p)

extern unsigned char g_updateBrainEnabled; // [0x1375756]

struct CThingAICreature {
    char pad[0x1a8];
    void* f1a8;
};

// tail-call target: __fastcall on this->f1a8, same 2 stack args
extern "C" bool __fastcall sub_88bcb0(void* self, int /*edx*/, long a, void* p);

// this-in-ECX method modeled as __fastcall(self, edx, a, p)
bool __fastcall UpdateBrain(CThingAICreature* self, int edx, long a, void* p)
{
    if (!g_updateBrainEnabled)
        return false;
    // tail-call: ecx <- self->f1a8, edx passed through unchanged, same stack args
    return sub_88bcb0(self->f1a8, edx, a, p);
}