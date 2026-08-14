#include <stdio.h>
#include <string.h>

// Standalone behaviour model of AddGossip: copies the by-value CCharString argument
// and forwards it by value to an inner routine that records it.

static int   g_innerCalls = 0;
static char  g_innerLast[64];

struct CCharString {
    char buf[64];
    CCharString() { buf[0] = 0; }
    CCharString(const CCharString& o) { strcpy(buf, o.buf); }
    ~CCharString() {}
};

static void Inner(CCharString byval) {
    g_innerCalls++;
    strcpy(g_innerLast, byval.buf);
}

static void AddGossip(CCharString name) {
    Inner(name);
}

int main() {
    CCharString g;
    strcpy(g.buf, "TIQUATL_GOSSIP");
    AddGossip(g);
    // Original must be untouched; Inner must have received a faithful copy.
    if (g_innerCalls == 1 &&
        strcmp(g_innerLast, "TIQUATL_GOSSIP") == 0 &&
        strcmp(g.buf, "TIQUATL_GOSSIP") == 0) {
        printf("PARITY_OK\n");
    } else {
        printf("FAIL\n");
    }
    return 0;
}