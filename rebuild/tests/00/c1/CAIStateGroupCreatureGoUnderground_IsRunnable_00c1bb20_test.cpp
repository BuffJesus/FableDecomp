#include <cstdio>

struct CAIStateGroup_CreatureGoUnderground {
    char m_flag;
    char IsRunnable(float t);
};

char CAIStateGroup_CreatureGoUnderground::IsRunnable(float t)
{
    return this->m_flag = (char)(long)(t * 0.5f);
}

int main()
{
    CAIStateGroup_CreatureGoUnderground o;
    o.m_flag = 0x7E;
    // 5.0 * 0.5 = 2.5 -> (long)2.5 = 2 -> (char)2
    char r = o.IsRunnable(5.0f);
    // -3.0 * 0.5 = -1.5 -> (long)-1.5 = -1 -> stored 0xFF
    char r2 = o.IsRunnable(-3.0f);
    if (r == 2 && o.m_flag == (char)-1 && r2 == (char)-1)
        printf("VERIFY_OK r=%d flag=%d r2=%d\n", (int)r, (int)o.m_flag, (int)r2);
    else
        printf("VERIFY_FAIL r=%d flag=%d r2=%d\n", (int)r, (int)o.m_flag, (int)r2);
    return 0;
}