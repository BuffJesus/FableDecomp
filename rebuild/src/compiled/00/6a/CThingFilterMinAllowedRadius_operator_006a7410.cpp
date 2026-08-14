// Byte-exact reconstruction of CThingFilter_MinAllowedRadius::operator()
// retail 0x006a7410  bool __fastcall (self /*ecx*/, CThing* thing /*[esp+4]*/)

struct CThingSub {
    char pad[0x34];
    float radius;      // +0x34
};

struct CThing {
    char pad[0x60];
    CThingSub* sub;    // +0x60
};

struct CThingFilter_MinAllowedRadius {
    float minAllowedRadius; // +0x00
    bool operator()(CThing* thing);
};

// Real member -> genuine thiscall: this in ecx, thing on stack at [esp+4].
bool CThingFilter_MinAllowedRadius::operator()(CThing* thing)
{
    return thing->sub->radius > this->minAllowedRadius;
}