struct Inner { char pad[0x78]; bool flag; };
struct Mid { char pad[0x1964]; Inner* inner; };
struct Outer { char pad[0x18]; Mid* mid; };
struct Self { char pad[4]; Outer* outer; };

bool __fastcall CGameScriptInterface_IsRadialBlurFadeActive(const Self* self)
{
    return self->outer->mid->inner->flag;
}