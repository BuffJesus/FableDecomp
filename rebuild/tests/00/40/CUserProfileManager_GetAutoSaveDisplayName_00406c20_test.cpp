#include <cstdio>
#include <cstring>

// Self-contained behavior model of GetAutoSaveDisplayName:
// selects a display-name source based on which manager global is non-null,
// falling through to a factory default when both are null.

struct WStr { const char* s; };

static WStr build(const char* fmt) { WStr r; r.s = fmt; return r; }

struct NodeA { char pad[0x14]; const char* label; };
struct NodeB { char pad[0x60]; const char* label; };

static NodeA* g_A;
static NodeB* g_B;

static const char* LIT_AB = "AUTOSAVE_FMT";
static const char* LIT_FACTORY = "AUTOSAVE_DEFAULT";

static WStr GetAutoSaveDisplayName()
{
    NodeA* a = g_A;
    if (a) {
        // formats LIT_AB via node A's object; model result as the node label.
        WStr fmt = build(LIT_AB);
        WStr r; r.s = a->label; (void)fmt;
        return r;
    }
    NodeB* b = g_B;
    if (b) {
        WStr fmt = build(LIT_AB);
        WStr r; r.s = b->label; (void)fmt;
        return r;
    }
    return build(LIT_FACTORY);
}

int main()
{
    NodeA na; na.label = "FROM_A";
    NodeB nb; nb.label = "FROM_B";

    // branch A dominates when g_A set
    g_A = &na; g_B = &nb;
    if (strcmp(GetAutoSaveDisplayName().s, "FROM_A") != 0) { printf("FAIL A\n"); return 1; }

    // branch B when only g_B set
    g_A = 0; g_B = &nb;
    if (strcmp(GetAutoSaveDisplayName().s, "FROM_B") != 0) { printf("FAIL B\n"); return 1; }

    // factory default when both null
    g_A = 0; g_B = 0;
    if (strcmp(GetAutoSaveDisplayName().s, LIT_FACTORY) != 0) { printf("FAIL DEF\n"); return 1; }

    printf("00406c20_TEST PASS\n");
    return 0;
}