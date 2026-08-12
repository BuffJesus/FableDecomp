#include <cstdio>

extern "C" int __cdecl EndDeflateStream(void* strm);

struct CCompressorZlib {
    char pad[0x1c];
    char zstream[0x38];
    char active;
    void FinishCompressStream();
};

static int g_ended = 0;
static void* g_last = 0;
extern "C" int __cdecl EndDeflateStream(void* strm) {
    g_ended++;
    g_last = strm;
    return 0;
}

int main() {
    CCompressorZlib c;
    // Case 1: active -> helper called, flag cleared, arg = self+0x1c
    c.active = 1;
    g_ended = 0; g_last = 0;
    c.FinishCompressStream();
    bool ok1 = (g_ended == 1) && (c.active == 0) &&
               (g_last == (void*)&c.zstream);

    // Case 2: inactive -> nothing happens
    c.active = 0;
    g_ended = 0; g_last = 0;
    c.FinishCompressStream();
    bool ok2 = (g_ended == 0) && (c.active == 0);

    if (ok1 && ok2) {
        printf("FINISH_STREAM_OK\n");
        return 0;
    }
    printf("FAIL o1=%d o2=%d\n", ok1, ok2);
    return 1;
}