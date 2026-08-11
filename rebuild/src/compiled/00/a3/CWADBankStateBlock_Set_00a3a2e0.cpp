// CWADBankStateBlock::Set - retail 0x00a3a2e0
// Retail body is a bare `ret 4`: a __fastcall void method taking one
// pointer parameter, with an empty body. The callee-clean `ret 4`
// pops the single stack argument; `this` arrives in ecx.

class CBankStateBlock
{
public:
    int m_data;
};

class CWADBankStateBlock
{
public:
    void Set(const CBankStateBlock *src);
    int m_state;
};

void CWADBankStateBlock::Set(const CBankStateBlock *src)
{
    // Intentionally empty - retail emits only `ret 4`.
    (void)src;
}