#include <cstdio>
#include <cstring>

struct Event { unsigned char bytes[40]; };
struct Processed
{
    unsigned char player;
    unsigned char unused01[3];
    long type;
    Event events[4];
    unsigned char count;
};

static void Add(Processed& out, const Event& event)
{
    unsigned char index = out.count;
    out.events[index] = event;
    out.type = 1;
    out.events[out.count].bytes[4] = out.player;
    ++out.count;
}

int main()
{
    Processed out;
    std::memset(&out, 0xCC, sizeof(out));
    out.player = 7;
    out.count = 2;
    Event event;
    for (int i = 0; i != 40; ++i) event.bytes[i] = static_cast<unsigned char>(i + 1);
    Event before0 = out.events[0];
    Event before1 = out.events[1];
    Add(out, event);
    if (out.type != 1 || out.count != 3) return 1;
    if (std::memcmp(&out.events[0], &before0, 40) != 0) return 2;
    if (std::memcmp(&out.events[1], &before1, 40) != 0) return 3;
    if (out.events[2].bytes[4] != 7) return 4;
    for (int i = 0; i != 40; ++i)
        if (i != 4 && out.events[2].bytes[i] != event.bytes[i]) return 5;
    std::puts("PROCESSED_INPUT_ADD_GAME_EVENT_PASS");
    return 0;
}