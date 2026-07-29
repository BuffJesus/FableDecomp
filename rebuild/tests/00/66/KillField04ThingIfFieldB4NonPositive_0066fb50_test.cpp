#include <stdio.h>
#include <string.h>

struct CThing_0066fb50
{
    unsigned char prefix[0xb4];
    float field_b4;

    void Kill(bool immediate);
};

struct Object_0066fb50
{
    unsigned char prefix[4];
    CThing_0066fb50* thing;
};

static int g_kill_calls;
static bool g_last_immediate;
static CThing_0066fb50* g_last_thing;

void CThing_0066fb50::Kill(bool immediate)
{
    ++g_kill_calls;
    g_last_immediate = immediate;
    g_last_thing = this;
}

extern "C" void __fastcall
KillField04ThingIfFieldB4NonPositive_0066fb50(
    Object_0066fb50* self);

static void ResetCalls()
{
    g_kill_calls = 0;
    g_last_immediate = false;
    g_last_thing = 0;
}

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
    Object_0066fb50 object;
    CThing_0066fb50 thing;
    unsigned long nan_bits = 0x7fc00001;

    memset(&object, 0, sizeof(object));
    memset(&thing, 0, sizeof(thing));

    ResetCalls();
    object.thing = 0;
    KillField04ThingIfFieldB4NonPositive_0066fb50(&object);
    ok &= Check(g_kill_calls == 0,
                "null thing must not be killed");

    ResetCalls();
    object.thing = &thing;
    thing.field_b4 = 1.0f;
    KillField04ThingIfFieldB4NonPositive_0066fb50(&object);
    ok &= Check(g_kill_calls == 0,
                "positive field must not kill the thing");

    ResetCalls();
    thing.field_b4 = 0.0f;
    KillField04ThingIfFieldB4NonPositive_0066fb50(&object);
    ok &= Check(g_kill_calls == 1 &&
                g_last_immediate &&
                g_last_thing == &thing,
                "zero field must call Kill(true) on the thing");

    ResetCalls();
    thing.field_b4 = -1.0f;
    KillField04ThingIfFieldB4NonPositive_0066fb50(&object);
    ok &= Check(g_kill_calls == 1 &&
                g_last_immediate &&
                g_last_thing == &thing,
                "negative field must call Kill(true) on the thing");

    ResetCalls();
    memcpy(&thing.field_b4, &nan_bits, sizeof(nan_bits));
    KillField04ThingIfFieldB4NonPositive_0066fb50(&object);
    ok &= Check(g_kill_calls == 0,
                "unordered field must not kill the thing");

    if (!ok)
        return 1;

    puts("PASS conditional CThing::Kill helper 0x0066FB50");
    return 0;
}
