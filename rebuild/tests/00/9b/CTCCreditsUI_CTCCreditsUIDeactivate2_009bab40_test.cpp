#include <cstdio>

struct CTCCreditsUI
{
    char pad[0x16];
    char flag16;
    char flag17;

    void Deactivate_2();
};

void CTCCreditsUI::Deactivate_2()
{
    if (flag16)
    {
        flag17 = 1;
    }
}

int main()
{
    CTCCreditsUI obj;
    for (int i = 0; i < (int)sizeof(obj); i++)
        ((char*)&obj)[i] = 0;

    obj.flag16 = 0;
    obj.flag17 = 0x7F;
    obj.Deactivate_2();
    if (obj.flag17 != 0x7F)
    {
        std::printf("FAIL_ZERO_CASE\n");
        return 1;
    }

    obj.flag16 = 1;
    obj.flag17 = 0;
    obj.Deactivate_2();
    if (obj.flag17 != 1)
    {
        std::printf("FAIL_SET_CASE\n");
        return 1;
    }

    obj.flag16 = (char)0xFF;
    obj.flag17 = 0;
    obj.Deactivate_2();
    if (obj.flag17 != 1)
    {
        std::printf("FAIL_NEG_CASE\n");
        return 1;
    }

    std::printf("PASS_CTCCreditsUI_DEACTIVATE2\n");
    return 0;
}