// _Dest_val<std::allocator<CEditLevelMerger::CConflict>,CEditLevelMerger::CConflict> @ 00403510
//
// STL _Dest_val destructor helper for a CEditLevelMerger::CConflict. __fastcall/
// __fastcall member: the object pointer arrives in ECX. The compiler-synthesised
// CConflict destructor runs the sub-object destructors in declaration-reverse
// order over the members, then tail-calls the leading member's destructor.
//
// Two masked sub-destructor targets (call/jmp rel32 operands are relocation-
// masked in parity):
//   dtorA (0x598000) on members at +0xbc, +0xb8, +0x94 and the tail +0x0c
//   dtorB (0x59b5d0) on members at +0x30 and +0x2c
//
// Mirrors the already-byte-matched sibling _Dest_val<allocator<CMacro>,CMacro>
// @ 004047a0: a straight sequence of __fastcall member-destructor calls, the
// last of which cl /O2 /Oy turns into the `pop esi ; jmp dtorA` tail.

struct MemberA { char pad[4]; };   // destroyed by dtorA (0x598000)
struct MemberB { char pad[4]; };   // destroyed by dtorB (0x59b5d0)

extern "C" void __fastcall dtorA(MemberA* p);   // 0x598000
extern "C" void __fastcall dtorB(MemberB* p);   // 0x59b5d0

struct Host {
    char     pad0[0x0c];   // 0x00..0x0b
    MemberA  m_0c;         // 0x0c  (dtorA, tail-called)
    char     pad1[0x2c - 0x10];
    MemberB  m_2c;         // 0x2c  (dtorB)
    MemberB  m_30;         // 0x30  (dtorB)
    char     pad2[0x94 - 0x34];
    MemberA  m_94;         // 0x94  (dtorA)
    char     pad3[0xb8 - 0x98];
    MemberA  m_b8;         // 0xb8  (dtorA)
    MemberA  m_bc;         // 0xbc  (dtorA)
};

void __fastcall Dest_val(Host* self)
{
    dtorA(&self->m_bc);
    dtorA(&self->m_b8);
    dtorA(&self->m_94);
    dtorB(&self->m_30);
    dtorB(&self->m_2c);
    dtorA(&self->m_0c);
}