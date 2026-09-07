#include <cstdio>

struct State
{
    bool initialised, host, updateEnabled;
    void* localPlayer;
    void* hostPlayer;
    unsigned long localFrame;
    void* component;
};
static bool Initialise(State& state, bool baseResult, void* component)
{
    if (!baseResult) return false;
    state.host = false;
    state.localPlayer = 0;
    state.hostPlayer = 0;
    state.localFrame = 0;
    state.component = component;
    state.updateEnabled = true;
    state.initialised = true;
    return true;
}
int main()
{
    int component;
    State failed = {false, true, false, &component, &component, 99, &component};
    if (Initialise(failed, false, 0)) return 1;
    if (!failed.host || failed.localFrame != 99 || failed.component != &component) return 2;
    State state = {false, true, false, &component, &component, 99, 0};
    if (!Initialise(state, true, &component)) return 3;
    if (!state.initialised || state.host || !state.updateEnabled) return 4;
    if (state.localPlayer || state.hostPlayer || state.localFrame != 0) return 5;
    if (state.component != &component) return 6;
    std::puts("INITIALISE_AS_LOCAL_PASS");
    return 0;
}