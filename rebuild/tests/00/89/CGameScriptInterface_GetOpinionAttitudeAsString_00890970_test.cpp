#include <cstdio>

int g_attTable[8] = {10,20,30,40,50,60,70,80};

struct StrDest {
    int captured;
    void ctor(const int* e);
};

void StrDest::ctor(const int* e) { captured = *e; }

void __stdcall CGameScriptInterface_GetOpinionAttitudeAsString(int attitude, StrDest* dest)
{
    dest->ctor(&g_attTable[attitude]);
}

int main() {
    StrDest d;
    d.captured = -1;
    CGameScriptInterface_GetOpinionAttitudeAsString(3, &d);
    if (d.captured != 40) { std::printf("FAIL got %d\n", d.captured); return 1; }
    CGameScriptInterface_GetOpinionAttitudeAsString(0, &d);
    if (d.captured != 10) { std::printf("FAIL got %d\n", d.captured); return 1; }
    std::printf("CGameScriptInterface_00890970_TEST PASS\n");
    return 0;
}