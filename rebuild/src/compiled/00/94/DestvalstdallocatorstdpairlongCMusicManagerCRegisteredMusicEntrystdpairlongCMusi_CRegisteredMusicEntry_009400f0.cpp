struct Sub {
    void Dtor();
};
struct Obj {
    char pad[0xa8];
    Sub sub;
    void Base();
    void DestVal_A8_G();
};

// out-of-line so calls become real call/jmp with relocation targets
__declspec(noinline) void Obj::DestVal_A8_G()
{
    this->sub.Dtor();   // ecx = this+0xa8, call
    this->Base();       // ecx = this, tail jmp
}