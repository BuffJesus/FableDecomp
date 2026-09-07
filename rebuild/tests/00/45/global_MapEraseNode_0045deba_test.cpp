#include <cstdio>

struct State { unsigned char* base; long count; };
struct Result
{
    unsigned long node;
    int a;
    int b;
    int c;
    int frees;
    long count;
};

static Result Run(State state, unsigned long node, bool eraseReturnsMemory)
{
    Result result;
    result.node = node;
    result.a = 4;
    result.b = 8;
    result.c = 12;
    result.frees = eraseReturnsMemory ? 1 : 0;
    --state.count;
    result.count = state.count;
    return result;
}

int main()
{
    unsigned char storage[16];
    State state = {storage, 3};
    Result result = Run(state, 0x1234, true);
    if (result.node != 0x1234 || result.a != 4 || result.b != 8 || result.c != 12) return 1;
    if (result.frees != 1 || result.count != 2) return 2;
    result = Run(state, 7, false);
    if (result.frees != 0 || result.count != 2) return 3;
    std::puts("MAP_ERASE_NODE_PASS");
    return 0;
}