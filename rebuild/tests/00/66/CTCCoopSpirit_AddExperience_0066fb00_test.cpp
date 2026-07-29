#include <stdio.h>
#include <string.h>

struct CTCCoopSpirit_0066fb00
{
    unsigned char prefix[0x10];
    long experience;
    unsigned long suffix;
};

extern "C" void __fastcall
CTCCoopSpirit_AddExperience_0066fb00(
    CTCCoopSpirit_0066fb00* self, void*, long amount);

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
    CTCCoopSpirit_0066fb00 spirit;

    memset(&spirit, 0x5a, sizeof(spirit));
    spirit.experience = 100;

    CTCCoopSpirit_AddExperience_0066fb00(&spirit, 0, 25);
    ok &= Check(spirit.experience == 125,
                "positive experience must be added");

    CTCCoopSpirit_AddExperience_0066fb00(&spirit, 0, -40);
    ok &= Check(spirit.experience == 85,
                "negative experience must be added");

    CTCCoopSpirit_AddExperience_0066fb00(&spirit, 0, 0);
    ok &= Check(spirit.experience == 85,
                "zero amount must preserve experience");

    ok &= Check(spirit.prefix[0] == 0x5a &&
                spirit.prefix[0x0f] == 0x5a &&
                spirit.suffix == 0x5a5a5a5a,
                "update must preserve adjacent object state");

    if (!ok)
        return 1;

    puts("PASS CTCCoopSpirit::AddExperience 0x0066FB00");
    return 0;
}
