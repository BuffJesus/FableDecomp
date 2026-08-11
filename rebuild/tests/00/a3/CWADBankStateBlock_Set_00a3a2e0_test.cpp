#include <cstdio>

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
    (void)src;
}

int main()
{
    CWADBankStateBlock blk;
    blk.m_state = 0x1234;

    CBankStateBlock src;
    src.m_data = 0x99;

    // Empty method: state must be unchanged after the call.
    blk.Set(&src);

    if (blk.m_state == 0x1234)
    {
        std::printf("SET_NOOP_OK\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}