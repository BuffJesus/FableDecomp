#include <stdio.h>

struct C3DAnimationSequence
{
    char  pad0[0xe];
    short m_field_e;
    void InitialiseFromFileData(float t);
};

// Mirror of the source's scale literal so the test can predict the result.
static const float kScale = 1234.5f;

int main()
{
    C3DAnimationSequence s;
    s.m_field_e = 0x1234;

    s.InitialiseFromFileData(1.0f);
    short expect1 = (short)(long)(1.0f * kScale);
    if (s.m_field_e != expect1) { printf("FAIL a %d\n", (int)s.m_field_e); return 1; }

    s.InitialiseFromFileData(2.5f);
    short expect2 = (short)(long)(2.5f * kScale);
    if (s.m_field_e != expect2) { printf("FAIL b %d\n", (int)s.m_field_e); return 1; }

    s.InitialiseFromFileData(-3.0f);
    short expect3 = (short)(long)(-3.0f * kScale);
    if (s.m_field_e != expect3) { printf("FAIL c %d\n", (int)s.m_field_e); return 1; }

    printf("INITFILEDATA_OK %d %d %d\n", (int)expect1, (int)expect2, (int)expect3);
    return 0;
}