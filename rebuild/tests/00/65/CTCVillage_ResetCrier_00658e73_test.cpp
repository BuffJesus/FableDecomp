#include <cstdio>

struct CTCVillage {
    char pad[0xb0];
    char m_b0;
    char m_b1;
    char ComputeB0();
    char ComputeB1();
    void ResetCrier();
};

char CTCVillage::ComputeB0() { return (char)0x37; }
char CTCVillage::ComputeB1() { return (char)0x5A; }

void CTCVillage::ResetCrier()
{
    this->m_b0 = this->ComputeB0();
    this->m_b1 = this->ComputeB1();
}

int main()
{
    CTCVillage v;
    v.m_b0 = 0;
    v.m_b1 = 0;
    v.ResetCrier();
    if (v.m_b0 == (char)0x37 && v.m_b1 == (char)0x5A) {
        printf("RESETCRIER_OK\n");
        return 0;
    }
    printf("RESETCRIER_FAIL\n");
    return 1;
}