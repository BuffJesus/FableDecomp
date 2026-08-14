// CTCVillage::OnInitialActivate @ 0x0081e1d0
struct Target;

struct CTCVillage {
    Target* obj;       // +0
    int     arg2;      // +4
    char    pad8[4];   // +8
    unsigned char b;   // +0xc  byte pushed
    unsigned char flag;// +0xd  guard/reset flag
};

// callee at 0x81e090: __fastcall(ecx=obj, edx=arg2, stack=byteval)
extern "C" void __fastcall Sub81E090(Target* obj, int arg2, unsigned char byteval);

void __fastcall OnInitialActivate(CTCVillage* self)
{
    if (self->flag) {
        Sub81E090(self->obj, self->arg2, self->b);
        self->flag = 0;
    }
}