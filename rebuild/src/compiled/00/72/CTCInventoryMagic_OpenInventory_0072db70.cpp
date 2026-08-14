struct MidObj;

struct GlobalObj {
    MidObj* Get(int idx);
};

struct MidObj {
    void Method();
};

struct Self {
    void Show();
};

extern GlobalObj* g_global;

void __fastcall OpenInventory(Self* self)
{
    g_global->Get(0xa)->Method();
    self->Show();
}