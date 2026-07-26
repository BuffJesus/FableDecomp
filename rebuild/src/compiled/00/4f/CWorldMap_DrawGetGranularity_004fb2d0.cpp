int __fastcall CWorldMap_DrawGetGranularity(const unsigned char* self)
{
    return *reinterpret_cast<const int*>(self + 76);
}