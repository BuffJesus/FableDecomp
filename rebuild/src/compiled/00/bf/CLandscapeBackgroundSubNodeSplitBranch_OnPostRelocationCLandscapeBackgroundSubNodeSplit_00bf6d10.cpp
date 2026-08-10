// Reconstruction of CLandscapeBackgroundSubNodeSplitBranch::OnPostRelocation @ 0x00bf6d10

struct Sub {
    char pad[4];
};

// Helper at 0xbf6b70: a member taking ecx (this-in-ecx)
void __fastcall HelperRelocate(Sub* self);

struct CLandscapeBackgroundSubNodeSplitBranch {
    // vtable ptr occupies +0x00 (has virtual)
    Sub sub0;           // +0x04
    char pad1[0xe4];    // +0x08 .. 0xeb
    Sub sub1;           // +0xec
    virtual void OnPostRelocation();
};

void CLandscapeBackgroundSubNodeSplitBranch::OnPostRelocation()
{
    HelperRelocate(&this->sub0);
    HelperRelocate(&this->sub1);
}