// CBasicStringChar::operator==  (0x004115a0)
// Trimmed to the operator== body only (retail oracle over-captures a following
// destroy-N loop at 0x00411600 which belongs to a different function; per the
// manifest note the true boundary is the first ~88 bytes ending at the third
// "ret 4", right before the int3 padding run).

struct CBasicStringChar
{
    char* m_str;

    bool operator==(const char* other) const;
};

bool CBasicStringChar::operator==(const char* other) const
{
    const char* p = other;
    const char* base = m_str - (int)p;

    int result;

    for (;;)
    {
        char a = base[(int)p];

        if (a == 0 && *p == 0)
        {
            result = 0;
            break;
        }

        char b = *p;

        if (a < b)
        {
            result = -1;
            break;
        }
        if (a > b)
        {
            result = 1;
            break;
        }

        ++p;
    }

    return result == 0;
}