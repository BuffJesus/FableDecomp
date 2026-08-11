#include <cstdio>

struct CTCOracleMinigame {
    char pad[0xC0];
    int outputs[4];
    bool OutputsFiring();
};

bool CTCOracleMinigame::OutputsFiring()
{
    for (int i = 0; i < 4; i++)
    {
        if (this->outputs[i] != 0)
            return true;
    }
    return false;
}

int main()
{
    CTCOracleMinigame m;
    for (int i = 0; i < 4; i++) m.outputs[i] = 0;

    bool anyEmpty = (m.OutputsFiring() == false);

    m.outputs[2] = 7;
    bool detects = (m.OutputsFiring() == true);

    m.outputs[2] = 0;
    m.outputs[3] = -1;
    bool detectsLast = (m.OutputsFiring() == true);

    if (anyEmpty && detects && detectsLast)
        printf("OUTPUTSFIRING_OK\n");
    else
        printf("FAIL\n");
    return (anyEmpty && detects && detectsLast) ? 0 : 1;
}