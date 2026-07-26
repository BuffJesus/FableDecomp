#pragma optimize("s",on)
struct SubA { void FinishA(); };
struct SubB { void FinishB(); };
struct CTCMoveBase { void FinishMove(); };

struct CTCAssassinRush {
    char pad0[0x3c];
    void* p3c;       // 0x3c
    char pad2[0x50 - 0x40];
    SubB b50;        // 0x50
    char pad3[0x5c - 0x54];
    void* p5c;       // 0x5c
    char pad4[0x84 - 0x60];
    SubA a84;        // 0x84
};

extern void __cdecl engine_free(void* p);

void __fastcall CTCAssassinRush_FinishMove(CTCAssassinRush* self)
{
    self->a84.FinishA();
    if (self->p5c)
        engine_free(self->p5c);
    self->b50.FinishB();
    if (self->p3c)
        engine_free(self->p3c);
    ((CTCMoveBase*)self)->FinishMove();
}