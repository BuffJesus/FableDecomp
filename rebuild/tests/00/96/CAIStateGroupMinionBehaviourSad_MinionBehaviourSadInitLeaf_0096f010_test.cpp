#include <string.h>
#include <stdio.h>

class CAIStateGroup_MinionBehaviourSad
{
public:
    char pad_0000[0x3c];
    unsigned char m_flag3c;
    unsigned char m_flag3d;
    unsigned char pad_3e[2];
    unsigned long m_field40;

    void InitLeaf();
};

void CAIStateGroup_MinionBehaviourSad::InitLeaf()
{
    m_flag3c = 0;
    m_flag3d = 0;
    m_field40 = 0;
}

int main()
{
    char buf[0x60];
    memset(buf, 0xAB, sizeof(buf));
    CAIStateGroup_MinionBehaviourSad* obj = (CAIStateGroup_MinionBehaviourSad*)buf;
    obj->InitLeaf();

    unsigned char* raw = (unsigned char*)buf;
    int ok = 1;
    if (raw[0x3c] != 0) ok = 0;
    if (raw[0x3d] != 0) ok = 0;
    if (raw[0x40] != 0 || raw[0x41] != 0 || raw[0x42] != 0 || raw[0x43] != 0) ok = 0;
    if (raw[0x3e] != 0xAB || raw[0x3f] != 0xAB) ok = 0;
    if (raw[0x00] != 0xAB) ok = 0;

    if (ok)
        printf("PASS_INITLEAF_9A3F\n");
    else
        printf("FAIL_INITLEAF\n");

    return 0;
}