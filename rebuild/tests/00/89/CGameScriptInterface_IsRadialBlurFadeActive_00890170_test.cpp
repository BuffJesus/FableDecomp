#include <cstdio>

struct Inner { char pad[0x78]; bool flag; };
struct Mid { char pad[0x1964]; Inner* inner; };
struct Outer { char pad[0x18]; Mid* mid; };
struct Self { char pad[4]; Outer* outer; };

bool __fastcall CGameScriptInterface_IsRadialBlurFadeActive(const Self* self)
{
    return self->outer->mid->inner->flag;
}

int main()
{
    Inner leaf;
    leaf.flag = true;
    Mid midv;
    midv.inner = (&leaf);
    Outer outv;
    outv.mid = (&midv);
    Self selfv;
    selfv.outer = (&outv);
    bool r = CGameScriptInterface_IsRadialBlurFadeActive((&selfv));
    leaf.flag = false;
    bool r2 = CGameScriptInterface_IsRadialBlurFadeActive((&selfv));
    if (r == true && r2 == false) {
        std::printf("CGameScriptInterface_00890170_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL r=%d r2=%d\n", (int)r, (int)r2);
    return 1;
}