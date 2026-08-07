struct S;
extern int __fastcall re_call(S*);
struct S { char pad[0x24]; int result; };
void __fastcall OnReplace(S* self)
{
    self->result = re_call(self);
}