struct S;
extern int __fastcall re_call(S*);
struct S { char pad[0x28]; int result; };
void __fastcall OnStart(S* self)
{
    self->result = re_call(self);
}