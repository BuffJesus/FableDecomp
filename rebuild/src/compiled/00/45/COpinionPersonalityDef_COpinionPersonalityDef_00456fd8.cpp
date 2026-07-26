#pragma optimize("s",on)
// ~COpinionPersonalityDef
extern void __cdecl free_field(void* p);           // call 0x7a7b44 (operator delete / free, __cdecl 1-arg)
extern void __fastcall member_dtor(void* self);    // dtor of member at +0x44 (__fastcall)
extern void __fastcall base_dtor(void* self);      // base class dtor (tail jmp)
extern const int g_vtbl;                            // 0x1233f8c vtable

struct COpinionPersonalityDef {
    void* vtbl;        // +0x00
    char pad[0x3c-4];  // +0x04 .. +0x3b
    void* ptr;         // +0x3c  freed field
    char member[8];    // +0x44  subobject (dtor'd)
};

void __fastcall COpinionPersonalityDef_dtor(COpinionPersonalityDef* self)
{
    self->vtbl = (void*)&g_vtbl;
    free_field(self->ptr);
    self->ptr = 0;
    member_dtor((char*)self + 0x44);
    base_dtor(self);
}