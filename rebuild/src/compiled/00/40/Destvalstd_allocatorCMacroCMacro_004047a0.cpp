// _Dest_val<std::allocator<CMacro>,CMacro> @ 004047a0
struct MemberA { char pad[4]; };
struct MemberB { char pad[4]; };
struct Base { char pad[4]; };

extern "C" void __fastcall dtorA(MemberA* p);   // call at +0x14
extern "C" void __fastcall dtorB(MemberB* p);   // 0x59a340, +0x10 and +0xc
extern "C" void __fastcall dtorBase(Base* p);   // 0x595b40 tail

struct Host {
    char pad0[0xc];   // 0x00..0x0b
    MemberB m_c;      // 0x0c
    MemberB m_10;     // 0x10
    MemberA m_14;     // 0x14
};

void __fastcall Dest_val(Host* self)
{
    dtorA(&self->m_14);
    dtorB(&self->m_10);
    dtorB(&self->m_c);
    dtorBase((Base*)self);
}