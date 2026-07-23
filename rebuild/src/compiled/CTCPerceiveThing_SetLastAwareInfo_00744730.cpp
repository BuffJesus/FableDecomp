struct Vec3 { float x, y, z; };

struct CObj34 { char pad[0x44]; float f44; };

struct CTCPerceiveThing {
    char pad0[4];      // +0x00
    int field4;        // +0x04
    char pad8[4];      // +0x08
    char field_c[0xc]; // +0x0c getter subobject (address taken)
    float f18;         // +0x18
    char pad1c[4];     // +0x1c
    Vec3 v20;          // +0x20 (12 bytes -> +0x2c)
    char pad2c[8];     // +0x2c
    CObj34* p34;       // +0x34
};

struct GObj {
    void NotifyA(int arg);   // 0xffcf31c0
    void NotifyB(int arg);   // 0xffcf31d0
};

extern "C" void* __fastcall Getter_2bd420(void* self);   // 0x2bd420
extern "C" Vec3* __fastcall Getter_d82ca0(void* self);   // 0xffd82ca0
extern float g_const_122dedc;                            // ds:0x122dedc
extern GObj* g_obj_13b8790;                              // ds:0x13b8790

void __fastcall CTCPerceiveThing_SetLastAwareInfo(CTCPerceiveThing* self)
{
    void* sub = Getter_2bd420(self->field_c);
    Vec3* v = Getter_d82ca0(sub);
    self->v20 = *v;

    float val = self->p34->f44;
    self->f18 = val;

    if (val != g_const_122dedc) {
        if (Getter_2bd420(self->field_c)) {
            g_obj_13b8790->NotifyA(self->field4);
        } else {
            g_obj_13b8790->NotifyB(self->field4);
        }
    }
}