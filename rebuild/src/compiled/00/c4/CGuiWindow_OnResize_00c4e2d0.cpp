// CGuiWindow::OnResize @ 0x00c4e2d0 (VC7.1 __fastcall reconstruction)

struct IThing {
    virtual void slot0(int a) = 0;
};

struct CGuiWindow {
    char pad[0xe4];
    IThing* thing;   // +0xe4
};

void __fastcall OnResize(CGuiWindow* self)
{
    IThing* t = self->thing;
    if (t != 0) {
        t->slot0(1);
        self->thing = 0;
    }
}