struct CSingletonTarget;
extern "C" void __fastcall Singleton_Method_4374b0(CSingletonTarget* self);
extern CSingletonTarget* g_singleton_13b8790;

struct CTCHeroCentre {
    char pad[0x10];
    int f10;
};

void __fastcall CTCHeroCentre__OnHeroCloseDoor(CTCHeroCentre* self)
{
    switch (self->f10) {
    case 1:
        self->f10 = 0;
        Singleton_Method_4374b0(g_singleton_13b8790);
        break;
    }
}