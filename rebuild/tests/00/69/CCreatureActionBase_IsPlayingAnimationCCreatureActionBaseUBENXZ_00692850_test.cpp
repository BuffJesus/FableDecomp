#include <cstdio>

static int g_v6ret;

class CCreatureActionBase
{
public:
    virtual void v0() {}
    virtual void v1() {}
    virtual void v2() {}
    virtual void v3() {}
    virtual void v4() {}
    virtual void v5() {}
    virtual int  v6() { return g_v6ret; }
    char pad_04[0x1c - 0x04];
    int  m_1c;
    char pad_20[0x48 - 0x20];
    int  m_48;
public:
    virtual bool IsPlayingAnimation() const;
};

bool CCreatureActionBase::IsPlayingAnimation() const
{
    int end = ((CCreatureActionBase*)this)->v6() + m_48;
    return m_1c >= end;
}

int main()
{
    CCreatureActionBase d;
    g_v6ret = 10; d.m_48 = 5; d.m_1c = 20;   // 20 >= 15 -> true
    bool r1 = d.IsPlayingAnimation();
    g_v6ret = 10; d.m_48 = 5; d.m_1c = 14;   // 14 >= 15 -> false
    bool r2 = d.IsPlayingAnimation();
    g_v6ret = 3; d.m_48 = 2; d.m_1c = 5;     // 5 >= 5 -> true
    bool r3 = d.IsPlayingAnimation();

    if (r1 && !r2 && r3)
        printf("ISPLAYANIM_OK\n");
    else
        printf("FAIL r1=%d r2=%d r3=%d\n", r1, r2, r3);
    return (r1 && !r2 && r3) ? 0 : 1;
}