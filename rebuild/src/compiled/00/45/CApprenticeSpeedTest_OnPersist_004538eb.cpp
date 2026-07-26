#pragma optimize("s",on)
struct CApprenticeSpeedTest { int dummy; };

extern void __stdcall Persist_Serialize(int id, float* pf, unsigned char* pb);

CApprenticeSpeedTest* __fastcall CApprenticeSpeedTest_OnPersist(CApprenticeSpeedTest* self)
{
    float f = 0.0f;
    unsigned char b;
    Persist_Serialize(5, &f, &b);
    return self;
}