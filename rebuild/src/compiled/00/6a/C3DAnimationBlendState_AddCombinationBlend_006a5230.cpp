struct C3DAnimationBlendState;

struct BlendMgr {
    void Add(C3DAnimationBlendState* s);
};

extern "C" void __fastcall helper_6a5230(C3DAnimationBlendState* self);

extern BlendMgr* g_theMgr;

void __fastcall AddCombinationBlend(C3DAnimationBlendState* self)
{
    helper_6a5230(self);
    g_theMgr->Add(self);
}