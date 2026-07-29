#include <stdio.h>
#include <string.h>

struct CTCCoopSpirit_0066fb10
{
    unsigned char prefix[0x10];
    long experience;
    unsigned long suffix;
};

extern "C" void __fastcall
CTCCoopSpirit_AddExperience_0066fb10(
    CTCCoopSpirit_0066fb10* self, void*, long amount);

static int Check(int condition, const char* message)
{
    if (!condition)
    {
        printf("FAIL: %s\n", message);
        return 0;
    }
    return 1;
}

int main()
{
    int ok = 1;
    CTCCoopSpirit_0066fb10 spirit;

    memset(&spirit, 0xa5, sizeof(spirit));
    spirit.experience = -20;

    CTCCoopSpirit_AddExperience_0066fb10(&spirit, 0, 70);
    ok &= Check(spirit.experience == 50,
                "positive amount must update experience");

    CTCCoopSpirit_AddExperience_0066fb10(&spirit, 0, -75);
    ok &= Check(spirit.experience == -25,
                "negative amount must update experience");

    CTCCoopSpirit_AddExperience_0066fb10(&spirit, 0, 0);
    ok &= Check(spirit.experience == -25,
                "zero amount must preserve experience");

    ok &= Check(spirit.prefix[0] == 0xa5 &&
                spirit.prefix[0x0f] == 0xa5 &&
                spirit.suffix == 0xa5a5a5a5,
                "update must preserve adjacent object state");

    if (!ok)
        return 1;

    puts("PASS CTCCoopSpirit::AddExperience 0x0066FB10");
    return 0;
}
