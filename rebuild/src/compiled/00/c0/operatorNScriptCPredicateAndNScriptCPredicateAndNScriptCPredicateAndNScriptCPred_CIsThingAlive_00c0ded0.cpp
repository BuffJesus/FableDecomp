// operator&&<std ...>  value-return this predicate wrapper @ 00c0ded0
// void* __fastcall f(void* self, double d) { helper(self, d); return self; }

struct Obj;

// helper at 0xc0dd60: __fastcall (ecx=self, double on stack)
void __fastcall helper_c0dd60(Obj* self, double d);

Obj* __fastcall operator_andand_predicate(Obj* self, double d)
{
    helper_c0dd60(self, d);
    return self;
}