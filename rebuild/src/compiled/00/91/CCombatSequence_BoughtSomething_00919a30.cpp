struct S;
extern int __fastcall re_call(S*);
struct S { char pad[0x14]; int result; };
void __fastcall BoughtSomething(S* self)
{
    self->result = re_call(self);
}