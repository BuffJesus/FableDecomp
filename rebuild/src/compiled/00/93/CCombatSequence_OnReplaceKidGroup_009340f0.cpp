struct S;
extern int __fastcall re_call(S*);
struct S { char pad[0x2C]; int result; };
void __fastcall OnReplaceKidGroup(S* self)
{
    self->result = re_call(self);
}