// CTCStealth::OnCreate  retail 0x00c5a1d0
// xor eax,eax; [ecx+0x1e8]=0x1000; [ecx+0x1ec]=0; [ecx+0xd8]=0; jmp base::OnCreate

struct CTCStealth {
    char pad_d8[0xd8];
    unsigned int f_d8;      // +0xd8
    char pad2[0x1e8 - 0xd8 - 4];
    unsigned int f_1e8;     // +0x1e8
    unsigned int f_1ec;     // +0x1ec
};

// base-class OnCreate, tail-called
void __fastcall CTCStealth_base_OnCreate(void* self);

void __fastcall CTCStealth_OnCreate(CTCStealth* self)
{
    self->f_1e8 = 0x1000;
    self->f_1ec = 0;
    self->f_d8 = 0;
    CTCStealth_base_OnCreate(self);
}