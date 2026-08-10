struct Sub { int x; };
struct T {
    char pad[0xa8];
    Sub sub;
};

extern void __fastcall Sub_release(Sub* s);
extern void __fastcall T_finish(T* self);

void __fastcall DestVal_A8_I(T* self)
{
    Sub_release(&self->sub);
    T_finish(self);
}