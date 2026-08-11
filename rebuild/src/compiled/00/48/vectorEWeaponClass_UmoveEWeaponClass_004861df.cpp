#pragma optimize("s",on)
// _Umove_EWeaponClass @ 004861df
// __fastcall forwarder: this in ecx (unused), one stack arg.
// body: bool tmp=false; return helper(arg, &tmp);

extern "C" void* __stdcall helper_486189(void* arg, bool* pflag);

// this-in-ECX method modeled as __fastcall(self, edx, arg)
#pragma optimize("y", off)
void* __fastcall _Umove_EWeaponClass(void* self, int /*edx*/, void* arg)
{
    bool tmp = false;
    return helper_486189(arg, &tmp);
}