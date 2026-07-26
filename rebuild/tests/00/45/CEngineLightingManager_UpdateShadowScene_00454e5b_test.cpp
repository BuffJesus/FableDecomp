#include <cstdio>

struct Sink {
    int last;
    void Take(int v);
};
void Sink::Take(int v) { last = v; }

struct Local {
    int x;
    void Fill(int a, int* b);
};
static int g_fill_this_x; static int g_fill_a; static int g_fill_b;
void Local::Fill(int a, int* b) { g_fill_this_x = x; g_fill_a = a; g_fill_b = *b; }

class CEngineLightingManager {
public:
    char pad[0x18];
    int mode;
    char pad2[0x08];
    int obj24;
    Sink* sink28;
    void UpdateShadowScene(int* p);
};

static unsigned int g_prep_tag = 0;
void __stdcall Prep(unsigned int tag) { g_prep_tag = tag; }

void CEngineLightingManager::UpdateShadowScene(int* p)
{
    Local h;
    Prep(0x122d70eU);
    h.x = 0;
    switch (this->mode) {
    case 2:
        h.Fill(this->obj24, p);
        break;
    case 3:
        this->sink28->Take(*p);
        break;
    }
}

int main() {
    Sink s; s.last = -1;
    CEngineLightingManager sc;
    for (int i=0;i<(int)sizeof(sc);i++) ((char*)&sc)[i]=0;
    sc.obj24 = 77; sc.sink28 = &s;
    int val = 555;

    // case 3
    sc.mode = 3; s.last = -1; g_prep_tag = 0;
    sc.UpdateShadowScene(&val);
    if (s.last != 555) { std::printf("FAIL case3 %d\n", s.last); return 1; }
    if (g_prep_tag != 0x122d70eU) { std::printf("FAIL prep\n"); return 1; }

    // case 2 : Fill(this=&h with x==0, obj24, *p)
    sc.mode = 2; g_fill_this_x = -9; g_fill_a = 0; g_fill_b = 0;
    sc.UpdateShadowScene(&val);
    if (g_fill_this_x != 0 || g_fill_a != 77 || g_fill_b != 555) {
        std::printf("FAIL case2 %d %d %d\n", g_fill_this_x, g_fill_a, g_fill_b); return 1; }

    // default (mode 5) - no-op
    sc.mode = 5; s.last = -1;
    sc.UpdateShadowScene(&val);
    if (s.last != -1) { std::printf("FAIL default\n"); return 1; }

    std::printf("CEngineLightingManager_00454e5b_TEST PASS\n");
    return 0;
}