// CTCVillage::OnInitialActivate @ 0x00a73c80
struct Obj;

// callee 0xa73b20: __fastcall(ecx=field0, edx=field4, stack byte)
extern void __fastcall SubCall(Obj* self, void* arg2, unsigned char flag);

struct CTCVillage {
    Obj*          field0;   // +0x00 -> ecx
    void*         field4;   // +0x04 -> edx
    unsigned char field8;
    unsigned char field9;
    unsigned char fieldA;
    unsigned char fieldB;
    unsigned char fieldC;   // +0x0c
    unsigned char fieldD;   // +0x0d
};

void __fastcall OnInitialActivate(CTCVillage* self)
{
    if (self->fieldD) {
        SubCall(self->field0, self->field4, self->fieldC);
        self->fieldD = 0;
    }
}