#include <cstdio>
#include <cstring>

struct State
{
    long frame;
    unsigned long checksum1, checksum2;
    unsigned long localSequence, localCount;
    unsigned long outputSequence, outputCount;
    unsigned char payload[12];
};

static bool Run(State& state, unsigned char updateResult)
{
    if (updateResult != 1) return false;
    std::memcpy(state.payload + 0, &state.checksum1, 4);
    std::memcpy(state.payload + 4, &state.checksum2, 4);
    std::memcpy(state.payload + 8, &state.frame, 4);
    ++state.frame;
    ++state.localCount;
    state.localSequence = static_cast<unsigned long>(state.frame);
    state.outputCount = 1;
    state.outputSequence = state.localSequence;
    state.localCount = 0;
    return true;
}

int main()
{
    State state; std::memset(&state, 0xCC, sizeof(state));
    state.frame = 17; state.checksum1 = 0x11223344; state.checksum2 = 0x55667788;
    state.localSequence = 99; state.localCount = 0; state.outputCount = 7;
    State before = state;
    if (Run(state, 0)) return 1;
    if (std::memcmp(&state, &before, sizeof(state)) != 0) return 2;
    if (Run(state, 2)) return 3;
    if (std::memcmp(&state, &before, sizeof(state)) != 0) return 4;
    if (!Run(state, 1)) return 5;
    if (state.frame != 18 || state.localSequence != 18 || state.localCount != 0) return 6;
    if (state.outputCount != 1 || state.outputSequence != 18) return 7;
    unsigned long value;
    std::memcpy(&value, state.payload, 4); if (value != 0x11223344) return 8;
    std::memcpy(&value, state.payload + 4, 4); if (value != 0x55667788) return 9;
    std::memcpy(&value, state.payload + 8, 4); if (value != 17) return 10;
    std::puts("NETWORK_GET_LOCAL_PACKAGE_SET_PASS"); return 0;
}