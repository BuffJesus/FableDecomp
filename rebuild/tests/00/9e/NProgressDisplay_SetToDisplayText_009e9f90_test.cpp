// Behaviour of the NProgressDisplay forwarder at 0x009e9f90: no display installed ->
// nothing happens (IsActive false); installed -> exactly the matching virtual is
// called on the installed object with the caller's arguments.
#include "fable_progress_display.h"
#include <stdio.h>
#include <string.h>

CCountedPointer<CAProgressDisplay> NProgressDisplay::PProgressDisplay;

static char g_log[64]; static int g_n;
static const void* g_this; static const void* g_text; static float g_fraction; static bool g_b[6];
static bool g_active;

class CRecordingDisplay : public CAProgressDisplay
{
public:
    void StartProgress(bool a, bool b, bool c, bool d, bool e, bool f)
    { g_log[g_n++] = 'S'; g_this = this; g_b[0] = a; g_b[1] = b; g_b[2] = c; g_b[3] = d; g_b[4] = e; g_b[5] = f; }
    void StopProgress(bool, bool) { g_log[g_n++] = 'X'; }
    void DisplayProgress(const CCharString& text, float fraction, bool forceRender, bool isDebug)
    { g_log[g_n++] = 'D'; g_this = this; g_text = &text; g_fraction = fraction; g_b[0] = forceRender; g_b[1] = isDebug; }
    void InitialiseTextures(bool, bool) { g_log[g_n++] = 'I'; }
    void SetToDisplayText(bool displayText) { g_log[g_n++] = 'T'; g_this = this; g_b[0] = displayText; }
    bool IsActive() const { g_log[g_n++] = 'A'; g_this = this; return g_active; }
};

static void Clear() { memset(g_log, 0, sizeof(g_log)); g_n = 0; g_this = 0; g_text = 0; g_fraction = 0; memset(g_b, 0, sizeof(g_b)); }

int main()
{
    CRecordingDisplay display;
    unsigned char textStorage[16];
    const CCharString& text = *reinterpret_cast<const CCharString*>(textStorage);
    NProgressDisplay::PProgressDisplay.Data = 0;
    NProgressDisplay::PProgressDisplay.Info = 0;
    Clear();
    NProgressDisplay::SetToDisplayText(true);
    if (g_n != 0) { printf("FAIL none\n"); return 1; }
    NProgressDisplay::PProgressDisplay.Data = &display;
    Clear();
    NProgressDisplay::SetToDisplayText(true);
    if (strcmp(g_log, "T") != 0 || g_this != &display || g_b[0] != true) { printf("FAIL true\n"); return 2; }
    Clear(); g_b[0] = true;
    NProgressDisplay::SetToDisplayText(false);
    if (strcmp(g_log, "T") != 0 || g_b[0] != false) { printf("FAIL false\n"); return 3; }
    (void)text;
    NProgressDisplay::PProgressDisplay.Data = 0;
    printf("RB_9e9f90_OK\n");
    return 0;
}