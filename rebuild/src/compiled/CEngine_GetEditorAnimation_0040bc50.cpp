struct C3DAnimationInfo;

struct CBank {
    int handle;
    void Load(int arg);
};

struct CEngine {
    char pad[0xc0];
    CBank* banks; // +0xc0
    C3DAnimationInfo* Resolve();
};

C3DAnimationInfo* __fastcall CEngine_GetEditorAnimation(CEngine* self, int dummy, int idx, int arg)
{
    self->banks[idx].Load(arg);
    return self->Resolve();
}