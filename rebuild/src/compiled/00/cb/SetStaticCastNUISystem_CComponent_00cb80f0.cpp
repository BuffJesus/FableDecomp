// Byte-exact reconstruction of SetStaticCast<NUISystem::CComponent> @ 0x00cb80f0
// Disasm shape:
//   push esi; mov esi,ecx
//   mov eax,[esi+4]; mov ecx,[esi]
//   push eax; push ecx
//   mov ecx,esi; call inner(a,b)
//   mov ecx,esi; pop esi; jmp tail()

struct CComponent {};
struct CList {};

// The container 'self': two raw pointer members.
struct Holder
{
    CList*      m_first;   // [esi+0]
    CComponent* m_second;  // [esi+4]

    // callee @ 0xcbbda0 : takes (m_first, m_second)
    void Assign(CList* a, CComponent* b);
    // callee @ 0xcbc660 : no args (this only) -> tail jmp
    void Finalize();

    void SetStaticCast();
};

void Holder::SetStaticCast()
{
    Assign(m_first, m_second);
    Finalize();
}