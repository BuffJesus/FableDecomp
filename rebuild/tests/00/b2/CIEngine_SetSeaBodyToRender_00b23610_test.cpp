#include <cstdio>

static int g_dtorCalls = 0;

class CCharString {
public:
    ~CCharString() { ++g_dtorCalls; }
private:
    void* _storage;
};

struct CIEngine {
    void SetSeaBodyToRender(CCharString bodyName);
};

// Mirrors the retail semantics under test: a thiscall member whose only
// by-value CCharString parameter is destructed at scope exit, with no other
// observable side effect.
void CIEngine::SetSeaBodyToRender(CCharString /*bodyName*/)
{
}

int main()
{
    CIEngine engine;
    CCharString arg;

    engine.SetSeaBodyToRender(arg);

    if (g_dtorCalls == 1)
    {
        std::printf("SEA_BODY_DTOR_OK\n");
        return 0;
    }

    std::printf("FAIL dtorCalls=%d\n", g_dtorCalls);
    return 1;
}