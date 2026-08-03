struct CV_MurderTwistScript
{
    int unk_00[5];
    int stateA;
    int stateB;
    int stateC;

    void Init_b();
};

void CV_MurderTwistScript::Init_b()
{
    stateA = 0;
    stateB = 0;
    stateC = 0;
}

#include <cstdio>

int main()
{
    CV_MurderTwistScript obj;
    for (int i = 0; i < 5; ++i) obj.unk_00[i] = 0x11111111;
    obj.stateA = 0x7;
    obj.stateB = 0x8;
    obj.stateC = 0x9;

    obj.Init_b();

    if (obj.stateA == 0 && obj.stateB == 0 && obj.stateC == 0)
    {
        printf("INIT_B_OK\n");
    }
    else
    {
        printf("INIT_B_FAIL\n");
    }
    return 0;
}