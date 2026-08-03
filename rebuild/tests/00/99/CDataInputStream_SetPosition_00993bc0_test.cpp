
#include <cstdio>

class CDataInputStream
{
public:
    void SetPosition(unsigned long pos);

    unsigned long m_unused0;
    unsigned long m_cursor;
    unsigned long m_unused1;
    unsigned long m_window;
    unsigned long m_srcbase;
    unsigned long m_remaining;
};

void CDataInputStream::SetPosition(unsigned long pos)
{
    if (m_window != 0 && pos >= m_srcbase && pos <= m_cursor + m_remaining)
    {
        m_remaining = (m_cursor + m_remaining) - pos;
        m_window = (m_window - m_cursor) + pos;
        m_cursor = pos;
    }
    else
    {
        m_window = 0;
        m_srcbase = 0;
        m_remaining = 0;
        m_cursor = pos;
    }
}

int main()
{
    // Case 1: window nonzero, pos inside [srcbase, cursor+remaining] -> adjust branch
    CDataInputStream s1;
    s1.m_unused0 = 0;
    s1.m_cursor = 100;
    s1.m_window = 5000;
    s1.m_srcbase = 50;
    s1.m_remaining = 40; // valid range end = 140

    s1.SetPosition(120);
    // expected: remaining = (100+40)-120 = 20 ; window = (5000-100)+120 = 5020 ; cursor = 120
    if (s1.m_remaining != 20 || s1.m_window != 5020 || s1.m_cursor != 120)
    {
        printf("FAIL_CASE1\n");
        return 1;
    }

    // Case 2: window is zero -> else branch
    CDataInputStream s2;
    s2.m_unused0 = 0;
    s2.m_cursor = 100;
    s2.m_window = 0;
    s2.m_srcbase = 50;
    s2.m_remaining = 40;

    s2.SetPosition(999);
    if (s2.m_window != 0 || s2.m_srcbase != 0 || s2.m_remaining != 0 || s2.m_cursor != 999)
    {
        printf("FAIL_CASE2\n");
        return 1;
    }

    // Case 3: window nonzero but pos < srcbase -> else branch
    CDataInputStream s3;
    s3.m_unused0 = 0;
    s3.m_cursor = 100;
    s3.m_window = 5000;
    s3.m_srcbase = 50;
    s3.m_remaining = 40;

    s3.SetPosition(10);
    if (s3.m_window != 0 || s3.m_srcbase != 0 || s3.m_remaining != 0 || s3.m_cursor != 10)
    {
        printf("FAIL_CASE3\n");
        return 1;
    }

    // Case 4: window nonzero but pos > cursor+remaining -> else branch
    CDataInputStream s4;
    s4.m_unused0 = 0;
    s4.m_cursor = 100;
    s4.m_window = 5000;
    s4.m_srcbase = 50;
    s4.m_remaining = 40; // valid range end = 140

    s4.SetPosition(200);
    if (s4.m_window != 0 || s4.m_srcbase != 0 || s4.m_remaining != 0 || s4.m_cursor != 200)
    {
        printf("FAIL_CASE4\n");
        return 1;
    }

    // Case 5: pos exactly at boundary (pos == cursor+remaining) -> adjust branch (inclusive <=)
    CDataInputStream s5;
    s5.m_unused0 = 0;
    s5.m_cursor = 100;
    s5.m_window = 5000;
    s5.m_srcbase = 50;
    s5.m_remaining = 40; // valid range end = 140

    s5.SetPosition(140);
    if (s5.m_remaining != 0 || s5.m_window != 5040 || s5.m_cursor != 140)
    {
        printf("FAIL_CASE5\n");
        return 1;
    }

    printf("SETPOSITION_OK\n");
    return 0;
}