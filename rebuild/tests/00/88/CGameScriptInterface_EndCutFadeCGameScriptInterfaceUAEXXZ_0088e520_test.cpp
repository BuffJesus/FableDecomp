#include <cstdio>

static int g_a = 0;
static int g_b = 0;

struct CGameScriptInterface {
    void helperA() { g_a = 1; }
    void helperB(int f) { g_b = f; }
    void EndCutFade();
};

void CGameScriptInterface::EndCutFade() {
    this->helperA();
    this->helperB(0x3f000000);
}

int main() {
    CGameScriptInterface o;
    o.EndCutFade();
    if (g_a == 1 && g_b == 0x3f000000) {
        printf("OK_0088e520\n");
        return 0;
    }
    printf("FAIL\n");
    return 1;
}