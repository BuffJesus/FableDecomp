#include <cstdio>

class CTCCreditsUI
{
public:
    char pad0[0xC];
    char flagC;
    char pad2[0x1C - 0xC - 1];
    char flag1C;

    void Deactivate();
};

void CTCCreditsUI::Deactivate()
{
    if (flagC)
    {
        flag1C = 1;
    }
}

int main()
{
    unsigned char buf1[0x20];
    for (int i = 0; i < 0x20; ++i) buf1[i] = 0;
    CTCCreditsUI* obj1 = (CTCCreditsUI*)buf1;
    obj1->flagC = 1;
    obj1->flag1C = 0;
    obj1->Deactivate();
    int ok1 = (obj1->flag1C == 1);

    unsigned char buf2[0x20];
    for (int i = 0; i < 0x20; ++i) buf2[i] = 0;
    CTCCreditsUI* obj2 = (CTCCreditsUI*)buf2;
    obj2->flagC = 0;
    obj2->flag1C = 0;
    obj2->Deactivate();
    int ok2 = (obj2->flag1C == 0);

    if (ok1 && ok2)
        printf("DEACTIVATE_TEST_PASS\n");
    else
        printf("DEACTIVATE_TEST_FAIL\n");

    return 0;
}