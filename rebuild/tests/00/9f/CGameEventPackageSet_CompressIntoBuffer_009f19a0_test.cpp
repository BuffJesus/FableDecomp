#include <cstdio>
#include <cstring>

struct Event { unsigned long type; unsigned char player, data[32], endPos, valid, replacement; };
struct Package { unsigned long sequence, count; Event events[40]; };
struct Set { unsigned long count; Package packages[50]; };

static long Compress(const Set& set, unsigned char* out)
{
    unsigned char* start = out;
    *out++ = static_cast<unsigned char>(set.count);
    for (unsigned long p = 0; p < set.count; ++p)
    {
        const Package& package = set.packages[p];
        *out++ = static_cast<unsigned char>(package.count);
        std::memcpy(out, &package.sequence, 4); out += 4;
        for (unsigned long e = 0; e < package.count; ++e)
        {
            const Event& event = package.events[e];
            unsigned short type = static_cast<unsigned short>(event.type);
            if (event.replacement) type |= 0x8000;
            std::memcpy(out, &type, 2); out[2] = event.player; out[3] = event.endPos;
            std::memcpy(out + 4, event.data, event.endPos);
            out += 4 + event.endPos;
        }
    }
    return static_cast<long>(out - start);
}

int main()
{
    Set set; std::memset(&set, 0, sizeof(set)); unsigned char out[128];
    if (Compress(set, out) != 1 || out[0] != 0) return 1;
    set.count = 2; set.packages[0].sequence = 0x44332211; set.packages[0].count = 2;
    Event& a = set.packages[0].events[0]; a.type = 0x1234; a.player = 7; a.endPos = 5;
    for (int i=0;i<5;++i) a.data[i] = static_cast<unsigned char>(0xA0+i);
    Event& b = set.packages[0].events[1]; b.type = 0x2345; b.player = 9; b.replacement = 1;
    set.packages[1].sequence = 0x88776655; set.packages[1].count = 0;
    long size = Compress(set, out);
    if (size != 24 || out[0] != 2 || out[1] != 2) return 2;
    if (out[2] != 0x11 || out[5] != 0x44) return 3;
    if (out[6] != 0x34 || out[7] != 0x12 || out[8] != 7 || out[9] != 5) return 4;
    for (int i=0;i<5;++i) if (out[10+i] != static_cast<unsigned char>(0xA0+i)) return 5;
    if (out[15] != 0x45 || out[16] != 0xA3 || out[17] != 9 || out[18] != 0) return 6;
    if (out[19] != 0 || out[20] != 0x55 || out[23] != 0x88) return 7;
    std::puts("PACKAGE_SET_COMPRESS_PASS"); return 0;
}