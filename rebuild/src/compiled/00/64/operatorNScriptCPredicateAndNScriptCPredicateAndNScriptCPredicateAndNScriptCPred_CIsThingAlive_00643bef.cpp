// operator&& for nested NScript::CPredicate_And predicates.
// Retail 00c0dea0:
//   push esi ; push edx ; mov esi,ecx ; call 0xc0dcf0 ; mov eax,esi ; pop esi ; ret
// ecx = self (the result / return buffer), edx = pointer arg; the arg is pushed
// as the single stack argument to the __fastcall helper at 0xc0dcf0, ecx=self.
// Function returns self (eax = esi).

struct CIsThingAlive;

struct CPredicate_And {
    // __fastcall helper at 0xc0dcf0: this=ecx, one stack arg.
    void helper(CIsThingAlive* arg);
};

// operator&&: __fastcall  self in ecx, arg in edx.
CPredicate_And* __fastcall operator_And(CPredicate_And* self, CIsThingAlive* arg) {
    self->helper(arg);
    return self;
}