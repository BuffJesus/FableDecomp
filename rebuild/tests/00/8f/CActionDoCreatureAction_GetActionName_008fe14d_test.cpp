#include <cstdio>

// Standalone behaviour model: GetActionName forwards to a lookup with a fixed
// name key and index -1, returning the same object pointer (this) it received.

struct CCharString { void* p; };
struct Action { int dummy; };

static const char* g_seenKey = 0;
static int g_seenIdx = 0;
static Action* g_seenSelf = 0;

static CCharString Lookup(Action* self, const char* key, int idx) {
    g_seenSelf = self;
    g_seenKey = key;
    g_seenIdx = idx;
    CCharString r; r.p = self; // callee builds result associated with self
    return r;
}

static const char* const kKey = (const char* const)0x128a268;

static CCharString GetActionName(Action* self) {
    return Lookup(self, kKey, -1);
}

int main() {
    Action a; a.dummy = 42;
    CCharString r = GetActionName(&a);
    if (g_seenSelf == &a && g_seenKey == kKey && g_seenIdx == -1 && r.p == &a) {
        printf("PARITY_OK\n");
    } else {
        printf("FAIL\n");
    }
    return 0;
}