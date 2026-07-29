struct CIDrawEnvironment;

struct CWorld_0066fb20
{
    unsigned char prefix[0x10];
    CIDrawEnvironment* draw_environment;
};

extern "C" CIDrawEnvironment* __fastcall
CWorld_DrawGetEnvironment_0066fb20(CWorld_0066fb20* self)
{
    return self->draw_environment;
}
