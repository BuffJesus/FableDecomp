struct Sub { int a; };

void __fastcall SubDtor(Sub* p);
void __fastcall BaseDtor(void* self);

struct Obj {
    char pad[0xA8];
    Sub sub;
};

void __fastcall DestVal_A8_B(Obj* self, void* /*edx*/)
{
    SubDtor(&self->sub);
    BaseDtor(self);
}